#include "VM.hpp"
#include "logger/Logger.hpp"
#include <chrono>
#include <libvirt/libvirt.h>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

bool VM::operator==(const VM &other) const {
    return this->domainPtr == other.domainPtr;
}

void VM::start() const {
    if (getState() == VIR_DOMAIN_RUNNING) {
        throw std::runtime_error("Invalid State. Tried to run a VM which is already running.");
    }

    int ret = virDomainCreate(domainPtr.get());
    if (ret != 0) {
        throw std::runtime_error("Failed to start VM");
    }
}

void VM::shutdown() const {
    if (getState() == VIR_DOMAIN_SHUTOFF) {
        throw std::runtime_error("Invalid State. Tried to shutdown a VM which is already shutdown.");
    }

    int ret = virDomainShutdown(domainPtr.get());
    if (ret != 0) {
        throw std::runtime_error("Failed to stop VM");
    }
}

void VM::destroy() const {
    if (getState() == VIR_DOMAIN_SHUTOFF) {
        throw std::runtime_error("Invalid State. Tried to shutdown a VM which is already shutdown.");
    }

    int ret = virDomainDestroy(domainPtr.get());
    if (ret != 0) {
        throw std::runtime_error("Failed to stop VM");
    }
}

int VM::getState() const {
    int state;
    int reason;
    int ret = virDomainGetState(domainPtr.get(), &state, &reason, 0);

    if (ret != 0) {
        throw std::runtime_error("Failed to get VM state");
    }

    return state;
}

virDomainInfo VM::getInfo() const {
    virDomainInfo info;
    int ret = virDomainGetInfo(domainPtr.get(), &info);

    if (ret != 0) {
        throw std::runtime_error("Failed to get info of VM");
    }
    return info;
}

std::vector<virDomainMemoryStatStruct> VM::getMemoryStats() const {
    std::vector<virDomainMemoryStatStruct> stats(10);
    int count = virDomainMemoryStats(domainPtr.get(), stats.data(), stats.size(), 0);

    if (count < 0) {
        throw std::runtime_error("Failed to get memory statistics");
    }

    stats.resize(count);
    return stats;
}

std::string VM::waitAndgetIP() const {
    const auto startTime = std::chrono::system_clock::now();
    const auto timeout = std::chrono::seconds(30);

    while (true) {
        virDomainInterfacePtr *interfaces = nullptr;
        int count = virDomainInterfaceAddresses(domainPtr.get(), &interfaces, VIR_DOMAIN_INTERFACE_ADDRESSES_SRC_LEASE, 0);
        if (count < 0) {
            throw std::runtime_error("virDomainInterfaceAddresses failed");
        }

        for (int i = 0; i < count; i++) {
            virDomainInterfacePtr interface = interfaces[i];

            LOG_DEBUG("Interface: {}, MAC: {}", interface->name, interface->hwaddr);

            for (int j = 0; j < interface->naddrs; j++) {
                virDomainIPAddressPtr address = &interface->addrs[j];
                LOG_DEBUG("IP: {}", address->addr);

                if (address->type == VIR_IP_ADDR_TYPE_IPV4) {
                    std::string ip = address->addr;
                    return ip;
                }
            }
        }

        std::this_thread::sleep_for(std::chrono::seconds(3));

        const auto now = std::chrono::system_clock::now();
        const auto diff = now - startTime;
        if (diff > timeout) {
            LOG_ERROR("Timeout!. Failed to get IP within {} seconds.", timeout);
            break;
        }
    }

    return "";
}
