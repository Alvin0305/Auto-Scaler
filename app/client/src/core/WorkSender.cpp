#include "core/WorkSender.hpp"
#include "lib/httplib.hpp"
#include "logger/Logger.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

void WorkSender::send(const Work &work) {
    const json workJson = work;
    const std::string body = workJson.dump();

    auto response = connection.Post("/work", body, "application/json");
    if (not response) {
        auto error = response.error();
        LOG_ERROR("Request failed with error: {}", httplib::to_string(error));
        return;
    }

    LOG_INFO("Response for work {} is {}", work, response->body);
}
