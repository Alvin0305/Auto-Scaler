#include "core/AutoScaler.hpp"
#include "VM.hpp"
#include "core/VMManager.hpp"
#include "core/VMMonitor.hpp"
#include "logger/Logger.hpp"
#include "model/Message.hpp"
#include "utils/utils.hpp"
#include <algorithm>
#include <libvirt/libvirt.h>
#include <stdexcept>
#include <string>
#include <utility>

void AutoScaler::listAllVMs() const {
    const auto vmNames = manager.getAllVMNames();
    LOG_INFO("{}\nVMs\n{}", std::string(50, '-'), std::string(50, '-'));
    for (const auto &vmName : vmNames) {
        LOG_INFO("{}", vmName);
    }
    LOG_INFO("{}", std::string(50, '-'));
}

void AutoScaler::printInfo() const {
    for (const auto &vm : vms) {
        virDomainInfo info = manager.getInfoOfVM(vm.getName());
        const auto memStats = manager.getMemInfoOfVM(vm.getName());
        LOG_INFO("{}", info);
        LOG_INFO("{}", memStatsToString(memStats));
    }
}

void AutoScaler::populateVMs() {
    const auto vmNames = manager.getAllVMNames();
    for (const auto &vmName : vmNames) {
        if (vmName == config.baseVMName) continue;
        if (vmName == config.server1VMName or vmName == config.server2VMName) {
            vms.push_back(manager.getVMByName(vmName).value());
        } else {
            LOG_WARN("There is a VM named {} which is neither the base VM nor the server 1 or 2", vmName);
        }
    }
}

void AutoScaler::scaleDown(const VM &vm) {
    if (vms.size() == 1) {
        LOG_DEBUG("Min Theshold reached but there is only one VM. Scale down NOT initiated");
        return;
    }

    LOG_INFO("Min Threshold reached and there are {} VMs. Scale down initiated...", vms.size());
    std::string destroyedVMIP = vm.waitAndgetIP();
    const std::string destroyedVMName = vm.getName();

    Message message = Message::destroy(destroyedVMName, destroyedVMIP);
    sender.send(message);

    destroyer.destroy(vm);
    auto it = std::find(vms.begin(), vms.end(), vm);
    if (it != vms.end()) {
        vms.erase(it);
    }

    LOG_INFO("VM destroyed");
}

void AutoScaler::scaleUp() {
    LOG_INFO("Reached MAX Threshold. Scale up initiated...");

    std::string cloneVMName = config.server2VMName;
    VM clonedVM = cloner.clone(config.baseVMName, cloneVMName);
    vms.push_back(std::move(clonedVM));
    manager.startVM(cloneVMName);

    LOG_INFO("VM {} Cloned to {} and started.", config.baseVMName, cloneVMName);
    LOG_INFO("State of {} is: {}", cloneVMName, VMStateToString(manager.getStateOfVM(cloneVMName)));

    const VM &newVM = vms.back();
    std::string ip = newVM.waitAndgetIP();
    if (ip.empty()) {
        LOG_ERROR("Failed to get IP in time for VM {}. Destroying it...", newVM.getName());
        destroyer.destroy(newVM);
        vms.pop_back();
        return;
    }

    Message message = Message::add(cloneVMName, ip);
    sender.send(message);
}

void AutoScaler::monitorAndScale() {
    while (true) {
        VMMonitor monitor(vms, config.monitoringInterval, config.peakCountForTrigger);
        MonitorResult result = monitor.monitorUntilEvent(config.maxCPUUsageThreshold, config.minCPUUsageThreshold);

        switch (result.action) {
            case MonitorResult::SCALE_UP:
                scaleUp();
                break;
            case MonitorResult::SCALE_DOWN:
                scaleDown(result.vm);
                break;
            default:
                LOG_ERROR("Unhandled action by VMMonitor: {}", static_cast<int>(result.action));
        }
    }
}

void AutoScaler::startVMs() {
    LOG_INFO("Starting all VMs");
    for (const auto &vm : vms) {
        if (manager.getStateOfVM(vm.getName()) == VIR_DOMAIN_RUNNING) continue;

        manager.startVM(vm.getName());
    }
}

void AutoScaler::start() {
    LOG_INFO("Starting AutoScaler...");

    listAllVMs();
    populateVMs();
    startVMs();
    printInfo();

    LOG_INFO("Watching {} VMs", vms.size());
    for (const auto &vm : vms) {
        LOG_INFO("- {}", vm.getName());
    }

    for (const auto &vm : vms) {
        const std::string ip = vm.waitAndgetIP();
        std::string vmName = vm.getName();

        if (ip.empty()) {
            LOG_ERROR("Failed to get IP for VM {} within time.", vmName);
            std::erase_if(vms, [vmName](const VM &vm) { return vm.getName() == vmName; });
            destroyer.destroy(vm);
            continue;
        }

        auto message = Message::add(vmName, ip);
        LOG_INFO("Sending message to LoadBalancer {}", message);
        sender.send(message);
    }

    try {
        monitorAndScale();
    } catch (std::runtime_error &e) {
        LOG_ERROR("Runtime Exception occured: {}", e.what());
    }
}
