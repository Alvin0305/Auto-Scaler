#include "core/Client.hpp"

#include "logger/Logger.hpp"
#include "model/Work.hpp"

#include <chrono>
#include <iostream>
#include <string>
#include <thread>

void Client::decreaseLoad() {
    LOG_INFO("State changed to LOW");
    state.store(LoadState::LOW);
}

void Client::increaseLoad() {
    LOG_INFO("State changed to HIGH");
    state.store(LoadState::HIGH);
}

void Client::workSender() {

    while (running) {

        // Read the current workload state
        LoadState currentState = state.load();

        Work work = workGenerator.generate(currentState);

        switch (currentState) {

            case LoadState::LOW:
                std::this_thread::sleep_for(
                    std::chrono::milliseconds(
                        config.workIntervalInLowLoad
                    )
                );
                break;

            case LoadState::HIGH:
                std::this_thread::sleep_for(
                    std::chrono::milliseconds(
                        config.workIntervalInHighLoad
                    )
                );
                break;

            default:
                LOG_WARN("Unhandled Load State");
                break;
        }

        if (running) {
            sender.send(work);
        }
    }
}

void Client::startWebServer() {

    // Get current client status
    server.Get("/api/status",
        [this](const httplib::Request &req,
               httplib::Response &res) {

            LoadState currentState = state.load();

            std::string load =
                currentState == LoadState::HIGH ? "HIGH" : "LOW";

            std::string response =
                "{\"running\":true,\"load\":\"" + load + "\"}";

            res.set_content(response, "application/json");
        }
    );

    // Increase workload
    server.Post("/api/load/increase",
        [this](const httplib::Request &req,
               httplib::Response &res) {

            increaseLoad();

            res.set_content(
                "{\"success\":true,\"load\":\"HIGH\",\"message\":\"Load increased\"}",
                "application/json"
            );
        }
    );

    // Decrease workload
    server.Post("/api/load/decrease",
        [this](const httplib::Request &req,
               httplib::Response &res) {

            decreaseLoad();

            res.set_content(
                "{\"success\":true,\"load\":\"LOW\",\"message\":\"Load decreased\"}",
                "application/json"
            );
        }
    );

    LOG_INFO("Client API running on port 8000");

    server.listen("127.0.0.1", 8000);
}

void Client::start() {

    running = true;

    // Start workload generation in a separate thread
    workSenderThread = std::thread(
        &Client::workSender, this
    );

    // Start HTTP server
    startWebServer();

    // Stop workload generation when server exits
    running = false;

    if (workSenderThread.joinable()) {
        workSenderThread.join();
    }
}
