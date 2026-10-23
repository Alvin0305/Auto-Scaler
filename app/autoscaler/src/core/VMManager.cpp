#include "core/VMManager.hpp"
#include "VM.hpp"
#include "exception/VMNotFoundException.hpp"
#include "logger/Logger.hpp"
#include <libvirt/libvirt.h>
#include <stdexcept>
#include <string>
#include <vector>

VMManager::VMManager(const std::string &qemuURL) {
    if (not connect(qemuURL)) {
        throw std::runtime_error("Connection failed");
    }
}

VMManager::~VMManager() {
    disconnect();
}

VMManager::VMManager(VMManager &&other) noexcept : connection(other.connection) {
    other.connection = nullptr;
}

VMManager &VMManager::operator=(VMManager &&other) noexcept {
    if (this != &other) {
        if (connection != nullptr) {
            virConnectClose(connection);
        }

        connection = other.connection;
        other.connection = nullptr;
    }

    return *this;
}

bool VMManager::connect(const std::string &qemuURL) {
    connection = virConnectOpen(qemuURL.c_str());
    LOG_INFO("Connected to: {}", virConnectGetURI(connection));
    return connection != nullptr;
}

void VMManager::disconnect() {
    if (connection != nullptr) {
        virConnectClose(connection);
    }
}

std::vector<std::string> VMManager::getAllVMNames() const {
    virDomainPtr *domains = nullptr;
    int numDomains = virConnectListAllDomains(connection, &domains, 0);

    if (numDomains == 0) {
        LOG_WARN("No VMs");
    }

    std::vector<std::string> domainNames(numDomains);
    for (int i = 0; i < numDomains; i++) {
        domainNames[i] = virDomainGetName(domains[i]);
    }

    free(domains);
    return domainNames;
}

std::optional<VM> VMManager::getVMByName(const std::string &name) const {
    virDomainPtr domainPtr = virDomainLookupByName(connection, name.c_str());

    if (domainPtr == nullptr) {
        return std::nullopt;
    }
    return VM(domainPtr, name);
}

void VMManager::startVM(const std::string &name) const {
    LOG_INFO("Starting {}...", name);
    auto vmOpt = getVMByName(name);
    if (not vmOpt) {
        throw VMNotFoundException(name);
    }

    vmOpt->start();
}

void VMManager::shutdownVM(const std::string &name) const {
    LOG_INFO("Shutting down {}", name);
    auto vmOpt = getVMByName(name);
    if (not vmOpt) {
        throw VMNotFoundException(name);
    }

    vmOpt->shutdown();
}

int VMManager::getStateOfVM(const std::string &name) const {
    auto vmOpt = getVMByName(name);
    if (not vmOpt) {
        throw VMNotFoundException(name);
    }

    return vmOpt->getState();
}

virDomainInfo VMManager::getInfoOfVM(const std::string &name) const {
    auto vmOpt = getVMByName(name);
    if (not vmOpt) {
        throw VMNotFoundException(name);
    }

    return vmOpt->getInfo();
}

std::vector<virDomainMemoryStatStruct> VMManager::getMemInfoOfVM(const std::string &name) const {
    auto vmOpt = getVMByName(name);
    if (not vmOpt) {
        throw VMNotFoundException(name);
    }

    return vmOpt->getMemoryStats();
}

const virConnectPtr VMManager::getConnectionPtr() const {
    return connection;
}
