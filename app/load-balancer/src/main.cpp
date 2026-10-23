#include "LoadBalancer.hpp"
#include "logger/Logger.hpp"
#include "model/Config.hpp"
#include "utils/ConfigParser.hpp"
#include <exception>

int main() {
    try {
        ConfigParser parser("config.json");
        Config config = parser.parse();

        LoadBalancer balancer(config.loadBalancerIP, config.loadBalancerPort, config.workerPort);
        balancer.start();
    } catch (std::exception &e) {
        LOG_ERROR("Failed to start Load Balancer: {}", e.what());
    }
}
