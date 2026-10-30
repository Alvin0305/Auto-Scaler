#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "core/WorkSender.hpp"
#include "lib/httplib.hpp"
#include "model/Config.hpp"
#include "model/LoadState.hpp"
#include "tools/WorkGenerator.hpp"

#include <atomic>
#include <thread>

class Client {
  private:
    std::atomic<bool> running{false};
    std::atomic<LoadState> state{LoadState::LOW};

    httplib::Server server;

    Config config;

    WorkGenerator workGenerator;
    WorkSender sender;

    std::thread workSenderThread;

    void workSender();

    void increaseLoad();
    void decreaseLoad();

    void startWebServer();

  public:
    Client(const Config &config)
        : config(config),
          workGenerator(config.work,
                        config.workValueRangeInLowLoad,
                        config.workValueRangeInHighLoad),
          sender(config.loadBalancerIP,
                 config.loadBalancerPort) {};

    void start();
};

#endif
