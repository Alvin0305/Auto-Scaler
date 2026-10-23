#include "core/Client.hpp"
#include "logger/Logger.hpp"
#include "model/Config.hpp"
#include "utils/ConfigParser.hpp"
#include <exception>

int main() {
    try {
        ConfigParser parser;
        Config config = parser.parse("config.json");
        LOG_DEBUG("Config:\n{}", config);

        Client client(config);
        client.start();
    } catch (std::exception &e) {
        LOG_ERROR("Failed to start Client: {}", e.what());
    }
    return 0;
}
