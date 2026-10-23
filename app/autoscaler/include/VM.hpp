#ifndef VM_HPP
#define VM_HPP

#include <libvirt/libvirt.h>
#include <memory>
#include <string>
#include <vector>

class VMManager;
class VMCloner;
class VMDestroyer;

class VMPtrDeleter {
  public:
    void operator()(const virDomainPtr ptr) const noexcept {
        if (ptr != nullptr) {
            virDomainFree(ptr);
        }
    }
};

using VMHandle = std::unique_ptr<_virDomain, VMPtrDeleter>;

class VM {
  private:
    std::string name;
    VMHandle domainPtr;

  public:
    explicit VM(virDomainPtr domain, const std::string &name) : domainPtr(domain), name(name) {};
    ~VM() = default;

    VM(const VM &other) = delete;
    VM &operator=(const VM &other) = delete;
    VM(VM &&other) noexcept = default;
    VM &operator=(VM &&other) noexcept = default;

    bool operator==(const VM &other) const;

    void start() const;
    void shutdown() const;
    void destroy() const;

    int getState() const;
    virDomainInfo getInfo() const;
    std::vector<virDomainMemoryStatStruct> getMemoryStats() const;

    std::string waitAndgetIP() const;

    friend class VMManager;
    friend class VMCloner;
    friend class VMDestroyer;

    std::string getName() const {
        return name;
    }
};

#endif
