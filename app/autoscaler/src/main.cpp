#include "core/AutoScaler.hpp"
#include "logger/Logger.hpp"
#include "model/Config.hpp"
#include "utils/ConfigParser.hpp"
#include <exception>

int main() {
    try {
        ConfigParser parser("config.json");
        Config config = parser.parse();

        AutoScaler autoscaler(config);
        autoscaler.start();
    } catch (std::exception &e) {
        LOG_ERROR("AutoScaler failed to start: {}", e.what());
    }
}
