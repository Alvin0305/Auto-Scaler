#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "core/WorkSender.hpp"
#include "lib/httplib.hpp"
#include "model/Config.hpp"
#include "model/LoadState.hpp"
#include "tools/WorkGenerator.hpp"
#include <atomic>
#include <mutex>
#include <thread>

class Client {
  private:
    std::atomic<bool> running{false};
    std::mutex mtx;

    httplib::Server server;

    LoadState state{LoadState::LOW};
    Config config;

    WorkGenerator workGenerator;
    WorkSender sender;

    std::thread workSenderThread;

    void workSender();
    void messageReceiver();

    void cli();

    void increaseLoad();
    void decreaseLoad();

  public:
    Client(const Config &config)
        : workGenerator(config.work, config.workValueRangeInLowLoad, config.workValueRangeInHighLoad),
          sender(config.loadBalancerIP, config.loadBalancerPort){};

    void start();
};

#endif
