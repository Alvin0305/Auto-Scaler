#include "connection/MessageSender.hpp"
#include "lib/httplib.hpp"
#include "logger/Logger.hpp"
#include "utils/utils.hpp"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

std::ostream &operator<<(std::ostream &out, const Message::Action &action) {
    out << messageActionToString(action);
    return out;
}

void MessageSender::send(const Message &message) {
    const json messageJson = message;
    const std::string body = messageJson.dump();

    auto response = client.Post("/scale", body, "application/json");
    if (not response) {
        auto error = response.error();
        LOG_ERROR("Request Failed with error: {}", httplib::to_string(error));
        return;
    }

    LOG_INFO("Response for message {} is {}", message, response->body);
}
