#ifndef VM_CLONER_HPP
#define VM_CLONER_HPP

#include "VM.hpp"
#include "model/Config.hpp"
#include "utils/IDGenerator.hpp"
#include <libvirt/libvirt.h>
#include <string>

class VMCloner {
  private:
    const virConnectPtr connection;
    const IDGenerator idGenerator;

    const Config &config;

    std::string getSourceVMDiskPath() const;

    std::string buildNewDiskPath(const std::string &sourceVMDiskPath, const std::string &newVMName) const;
    std::string buildStorageVolXML(const std::string &diskName, unsigned long long capacity) const;

    std::string generateNewVMXml() const;
    void cloneDisk(const std::string &sourceVMDiskPath, const std::string &cloneVMDiskPath) const;

  public:
    explicit VMCloner(const virConnectPtr connection, const Config &config)
        : connection(connection), idGenerator(), config(config) {};

    VM clone(const std::string &sourceVMName, const std::string &newName) const;
};

#endif
