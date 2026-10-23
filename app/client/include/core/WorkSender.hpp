#ifndef WORK_SENDER_HPP
#define WORK_SENDER_HPP

#include "lib/httplib.hpp"
#include "model/Work.hpp"
#include <string>

class WorkSender {
  private:
    httplib::Client connection;

  public:
    WorkSender(const std::string &loadBalancerIP, unsigned int loadBalancerPort)
        : connection(loadBalancerIP, loadBalancerPort) {};

    void send(const Work &work);
};

#endif
