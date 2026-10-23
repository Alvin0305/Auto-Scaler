#include "core/VMMonitor.hpp"
#include "logger/Logger.hpp"
#include "model/LoadState.hpp"
#include <chrono>
#include <libvirt/libvirt.h>
#include <thread>
#include <vector>

unsigned int VMMonitor::calculateCPUUsage(unsigned long long cpuTime1, unsigned long long cpuTime2, unsigned int numCPUs,
                                          double elapsedTime) const {
    if (cpuTime2 < cpuTime1 or numCPUs == 0 or elapsedTime <= 0) {
        LOG_WARN("Invalid CPU measurement. Returning 0% usage");
        return 0;
    }

    const double cpuTimeDelta = static_cast<double>(cpuTime2 - cpuTime1);
    const double cpuSeconds = static_cast<double>(cpuTimeDelta / 1'000'000'000.0);
    const double usage = (cpuSeconds / (numCPUs * elapsedTime)) * 100;

    LOG_DEBUG("\tCPU Time Delta: {}\n\tCPU Seconds: {}\n\tElapsed Seconds: {}\n\tvCPUs: {}", cpuTimeDelta, cpuSeconds,
              elapsedTime, numCPUs);
    return static_cast<int>(usage);
}

MonitorResult VMMonitor::monitorUntilEvent(unsigned int maxThreshold, unsigned int minThreshold) {
    int vmCount = vms.size();

    LOG_INFO("Monitoring {} VMs", vmCount);

    std::vector<unsigned long long> lastCpuTime(vmCount);
    std::vector<std::chrono::steady_clock::time_point> lastCalculatedAt(vmCount);
    std::vector<LoadState> previousState(vmCount, LoadState::NORMAL);
    std::vector<unsigned> loadCount(vmCount, 0);

    for (size_t i = 0; i < vmCount; i++) {
        lastCpuTime[i] = vms[i].getInfo().cpuTime;
        lastCalculatedAt[i] = std::chrono::steady_clock::now();
    }

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(interval));

        for (int i = vms.size() - 1; i >= 0; i--) {
            const auto &vm = vms[i];
            const auto currentTime = std::chrono::steady_clock::now();
            double elapsedSeconds = std::chrono::duration<double>(currentTime - lastCalculatedAt[i]).count();

            auto newCpuInfo = vm.getInfo();
            unsigned int cpuUsage = calculateCPUUsage(lastCpuTime[i], newCpuInfo.cpuTime, newCpuInfo.nrVirtCpu, elapsedSeconds);

            LoadState currentState = LoadState::NORMAL;
            if (cpuUsage < minThreshold) {
                currentState = LoadState::LOW;
            } else if (cpuUsage > maxThreshold) {
                currentState = LoadState::HIGH;
            }

            if (currentState != previousState[i]) {
                loadCount[i] = 0;
                previousState[i] = currentState;
            }

            if (currentState == LoadState::NORMAL) {
                loadCount[i] = 0;
            } else {
                loadCount[i]++;
            }

            LOG_INFO("{}: {{ CPU Usage={}%, State={}, Peak Count={}/{} }}", vm.getName(), cpuUsage, currentState, loadCount[i],
                     peakCountForTrigger);

            if (loadCount[i] >= peakCountForTrigger) {
                if (currentState == LoadState::LOW && vmCount > 1) {
                    return MonitorResult::scaledown(vm);
                }

                if (currentState == LoadState::HIGH && vmCount < 2) {
                    return MonitorResult::scaleup(vm);
                }
            }

            lastCpuTime[i] = newCpuInfo.cpuTime;
            lastCalculatedAt[i] = currentTime;
        }
    }
}
