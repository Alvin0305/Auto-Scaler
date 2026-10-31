#include "core/Client.hpp"
#include "lib/httplib.hpp"
#include "logger/Logger.hpp"
#include "model/LoadState.hpp"
#include "model/Work.hpp"
#include "tools/WorkGenerator.hpp"
#include <chrono>
#include <iostream>
#include <mutex>
#include <ostream>
#include <string>
#include <thread>

void Client::decreaseLoad() {
    LOG_INFO("State changed to LOW");
    state = LoadState::LOW;
}

void Client::increaseLoad() {
    LOG_INFO("State changed to HIGH");
    state = LoadState::HIGH;
}

void Client::workSender() {
    while (running) {
        Work work = workGenerator.generate(state);

        switch (state) {
            case LoadState::LOW:
                std::this_thread::sleep_for(std::chrono::milliseconds(config.workIntervalInLowLoad));
                break;
            case LoadState::HIGH:
                std::this_thread::sleep_for(std::chrono::milliseconds(config.workIntervalInHighLoad));
                break;
            default:
                LOG_WARN("Unhandled Load State");
                break;
        }

        sender.send(work);
    }
}

void Client::cli() {
    while (running) {
        std::cout << "Enter any of the following options" << std::endl;
        std::cout << "1. increase (i)" << std::endl;
        std::cout << "2. decrease (d)" << std::endl;
        std::cout << "3. exit (e)" << std::endl;

        std::string input;
        std::cin >> input;

        if (input == "increase" or input == "i") {
            increaseLoad();
        } else if (input == "decrease" or input == "d") {
            decreaseLoad();
        } else if (input == "exit" or input == "e") {
            std::cout << "Exiting Autoscaler client" << std::endl;
            {
                std::lock_guard<std::mutex> guard(mtx);
                server.stop();
            }
            running = false;
        } else {
            std::cout << "Invalid command" << std::endl;
        }
    }
}

void Client::start() {
    running = true;

    workSenderThread = std::thread(&Client::workSender, this);

    cli();

    workSenderThread.join();
}
