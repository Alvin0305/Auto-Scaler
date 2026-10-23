#ifndef AUTO_SCALER_HPP
#define AUTO_SCALER_HPP

#include "VM.hpp"
#include "VMManager.hpp"
#include "connection/MessageSender.hpp"
#include "model/Config.hpp"
#include "scaler/VMCloner.hpp"
#include "scaler/VMDestroyer.hpp"
#include "utils/IDGenerator.hpp"
#include <vector>

class AutoScaler {
  private:
    std::vector<VM> vms;

    Config config;

    VMManager manager;
    VMCloner cloner;
    VMDestroyer destroyer;
    IDGenerator idGenerator;
    MessageSender sender;

    void listAllVMs() const;
    void printInfo() const;

    void populateVMs();
    void startVMs();

    void scaleUp();
    void scaleDown(const VM &vm);
    void monitorAndScale();

  public:
    AutoScaler(const Config &config)
        : config(config), manager(config.qemuURL), cloner(manager.getConnectionPtr(), config),
          destroyer(manager.getConnectionPtr()), idGenerator(), sender(config.loadBalancerIP, config.loadBalancerPort) {};
    void start();
};

#endif
