#include "utils/ConfigParser.hpp"
#include "model/Config.hpp"
#include <fstream>
#include <nlohmann/json.hpp>
#include <string>
#include <sys/socket.h>

using json = nlohmann::json;

Config ConfigParser::parse(const std::string &configFilePath) const {
    std::ifstream configFS(configFilePath);
    if (not configFS.is_open()) {
        throw std::runtime_error(std::string("Failed to open ") + configFilePath);
    }

    json j = json::parse(configFS);

    Config config;
    config.qemuURL = j.at("qemuURL").get<std::string>();
    config.defaultStoragePoolName = j.at("defaultStoragePoolName").get<std::string>();
    config.defaultStoragePoolPath = j.at("defaultStoragePoolPath").get<std::string>();
    config.diskImageFileExtension = j.at("diskImageFileExtension").get<std::string>();

    json loadBalancerJSON = j.at("loadBalancer");
    config.loadBalancerIP = loadBalancerJSON.at("ip").get<std::string>();
    config.loadBalancerPort = loadBalancerJSON.at("port").get<unsigned int>();

    json VMsJSON = j.at("VMs");
    config.baseVMName = VMsJSON.at("baseVMName").get<std::string>();
    config.server1VMName = VMsJSON.at("server1VMName").get<std::string>();
    config.server2VMName = VMsJSON.at("server2VMName").get<std::string>();

    config.workerPort = j.at("workerPort").get<unsigned int>();
    config.monitoringInterval = j.at("monitoringInterval").get<unsigned int>();
    config.minCPUUsageThreshold = j.at("minCPUUsageThreshold").get<unsigned int>();
    config.maxCPUUsageThreshold = j.at("maxCPUUsageThreshold").get<unsigned int>();
    config.peakCountForTrigger = j.at("peakCountForTrigger").get<unsigned int>();

    json workJSON = j.at("work");
    config.work = workJSON.at("work").get<std::string>();
    config.workIntervalInLowLoad = workJSON.at("intervalInLowLoad").get<unsigned int>();
    config.workIntervalInHighLoad = workJSON.at("intervalInHighLoad").get<unsigned int>();
    config.workValueRangeInLowLoad = {workJSON.at("valueStartInLowLoad").get<unsigned int>(),
                                      workJSON.at("valueEndInLowLoad").get<unsigned int>()};
    config.workValueRangeInHighLoad = {workJSON.at("valueStartInHighLoad").get<unsigned int>(),
                                       workJSON.at("valueEndInHighLoad").get<unsigned int>()};

    return config;
}
