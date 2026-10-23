#include "core/AutoScaler.hpp"
#include "logger/Logger.hpp"
#include "model/Config.hpp"
#include "utils/ConfigParser.hpp"
#include <exception>

int main() {
    try {
        ConfigParser parser;
        Config config = parser.parse("config.json");

        AutoScaler autoscaler(config);
        autoscaler.start();
    } catch (std::exception &e) {
        LOG_ERROR("AutoScaler failed to start: {}", e.what());
    }
}
