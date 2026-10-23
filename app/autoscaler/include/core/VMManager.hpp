#ifndef VM_MANAGER_HPP
#define VM_MANAGER_HPP

#include "VM.hpp"
#include <libvirt/libvirt.h>
#include <optional>
#include <string>
#include <vector>

class VMManager {
  private:
    virConnectPtr connection;

  public:
    explicit VMManager(const std::string &qemuURL);
    ~VMManager();

    VMManager(const VMManager &other) = delete;
    VMManager &operator=(const VMManager &other) = delete;
    VMManager(VMManager &&other) noexcept;
    VMManager &operator=(VMManager &&other) noexcept;

    bool connect(const std::string &qemuURL);
    void disconnect();

    std::vector<std::string> getAllVMNames() const;
    std::optional<VM> getVMByName(const std::string &name) const;

    void startVM(const std::string &name) const;
    void shutdownVM(const std::string &name) const;

    int getStateOfVM(const std::string &name) const;
    virDomainInfo getInfoOfVM(const std::string &name) const;
    std::vector<virDomainMemoryStatStruct> getMemInfoOfVM(const std::string &name) const;

    const virConnectPtr getConnectionPtr() const;
};

#endif
