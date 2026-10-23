#include "LoadBalancer.hpp"
#include "lib/httplib.hpp"
#include "logger/Logger.hpp"
#include "model/Message.hpp"
#include "model/Work.hpp"
#include <algorithm>
#include <exception>
#include <string>

using json = nlohmann::json;

void LoadBalancer::start() {
    connection.Post("/work", [this](const httplib::Request &req, httplib::Response &res) {
        LOG_INFO("Received work: {}", req.body);

        try {
            json body = json::parse(req.body);
            Work work = body.get<Work>();

            auto resultOpt = balance(work);
            if (resultOpt) {
                res.set_content(resultOpt.value()->body, "application/json");
            } else {
                json response = {{"status", "failed"}, {"result", -1}};
                res.set_content(response.dump(), "application/json");
            }
        } catch (std::exception &e) {
            LOG_ERROR("Work dividing failed: {}", e.what());
        }
    });

    connection.Post("/scale", [this](const httplib::Request &req, httplib::Response &res) {
        LOG_INFO("Received request: {}", req.body);

        try {
            json body = json::parse(req.body);
            Message message = body.get<Message>();
            onMessage(message);

            json response = {{"status", "success"}};
            res.set_content(response.dump(), "application/json");
        } catch (std::exception &e) {
            LOG_ERROR("Handling message failed: {}", e.what());
        }
    });

    connection.listen(host, port);
    LOG_INFO("Load balancer running on {}:{}", host, port);
}

void LoadBalancer::onMessage(const Message &message) {
    switch (message.action) {
        case Message::Action::VM_ADDED:
            LOG_INFO("Adding VM {} with IP {} to LoadBalancer", message.name, message.ip);
            addIP(message.ip);
            break;
        case Message::Action::VM_DESTROYED:
            LOG_INFO("Removing VM {} with IP {} to LoadBalancer", message.name, message.ip);
            removeIP(message.ip);
            break;
        case Message::Action::NONE:
            LOG_INFO("Received a NONE Message. Doing nothing");
            break;
    }
}

void LoadBalancer::addIP(const std::string &ip) {
    ips.push_back(ip);

    LOG_INFO("New Connection establishing with worker VM {}:{}", ip, workerPort);

    httplib::Client workerServer(ip, workerPort);
    workerServers.push_back(std::move(workerServer));
}

void LoadBalancer::removeIP(const std::string &ip) {
    const auto ipIt = std::find(ips.begin(), ips.end(), ip);
    if (ipIt != ips.end()) {
        ips.erase(ipIt);
    }

    const auto socketIt = std::find_if(workerServers.begin(), workerServers.end(),
                                       [ip](const httplib::Client &client) { return client.host() == ip; });
    if (socketIt != workerServers.end()) {
        workerServers.erase(socketIt);
    }
}

void LoadBalancer::showIps() const {
    LOG_INFO("IPs: ");
    for (const auto &ip : ips) {
        LOG_INFO("\t- {}", ip);
    }
}

std::optional<httplib::Result> LoadBalancer::balance(const Work &work) {
    if (ips.empty()) {
        LOG_WARN("There is no server to do load balancing");
        return std::nullopt;
    }

    lastWorkedIndex = (lastWorkedIndex + 1) % ips.size();
    LOG_INFO("Sending work {} to {}th Server: {}", work, lastWorkedIndex, ips[lastWorkedIndex]);

    json workJson = work;

    httplib::Client &workerServer = workerServers[lastWorkedIndex];
    auto response = workerServer.Post("/work", workJson.dump(), "application/json");

    if (not response) {
        auto error = response.error();
        LOG_ERROR("Request failed: {}", httplib::to_string(error));
        return std::nullopt;
    }

    LOG_INFO("Response body: {}", response->body);
    return response;
}
