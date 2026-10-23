#ifndef LOAD_BALANCER_HPP
#define LOAD_BALANCER_HPP

#include "lib/httplib.hpp"
#include "model/Message.hpp"
#include "model/Work.hpp"
#include <string>
#include <vector>

class LoadBalancer {
  private:
    int lastWorkedIndex{0};

    httplib::Server connection;

    std::string host;
    unsigned int port;

    unsigned int workerPort;

    std::vector<std::string> ips;
    std::vector<httplib::Client> workerServers;

    std::optional<httplib::Result> balance(const Work &work);

    void addIP(const std::string &ip);
    void removeIP(const std::string &ip);

    void onMessage(const Message &message);

  public:
    LoadBalancer(const std::string &host, unsigned int port, unsigned int workedPort)
        : host(host), port(port), workerPort(workedPort) {};

    void start();
    void stop();
};

#endif
