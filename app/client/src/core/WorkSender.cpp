#include "core/WorkSender.hpp"
#include "lib/httplib.hpp"
#include "logger/Logger.hpp"
#include <exception>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void WorkSender::send(const Work &work) {
    const json workJson = work;
    const std::string body = workJson.dump();

    try {
        auto response = connection.Post("/work", body, "application/json");
        if (not response) {
            auto error = response.error();
            LOG_ERROR("Request failed with error: {}", httplib::to_string(error));
            return;
        }

        LOG_INFO("Response for work {} is {}", work, response->body);
    } catch (const std::exception &e) {
        LOG_ERROR("Failed to send work: {}", e.what());
    }
}
