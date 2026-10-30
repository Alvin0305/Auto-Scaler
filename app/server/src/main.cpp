#include "Processor.hpp"
#include "httplib.hpp"
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>

using json = nlohmann::json;

int main() {
    Processor processor;
    httplib::Server server;

    server.Get("/", [](const httplib::Request &req, httplib::Response &res) {
        std::cout << "Received GET Request" << std::endl;
        res.set_content("Hello from server!", "text/plain");
    });

    server.Post("/work", [processor](const httplib::Request &req, httplib::Response &res) {
        std::cout << "Received POST Request" << std::endl;
        std::cout << req.body << std::endl;

        try {
            json data = json::parse(req.body);
            int value = data["value"];

            std::cout << "Value: " << value << std::endl;
            int result = processor.process(value);

            json response = {{"status", "success"}, {"result", result}};
            res.set_content(response.dump(), "application/json");
        } catch (const json::parse_error &e) {
            res.status = 400;
            json response = {{"status", "error"}, {"message", "Invalid JSON"}};

            res.set_content(response.dump(), "application/json");
        }
    });

    std::cout << "Server listening on port 8080\n";
    server.listen("0.0.0.0", 8080);

    return 0;
}
