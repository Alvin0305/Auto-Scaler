#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>

struct Config {
    std::string qemuURL;
    std::string diskImageFileExtension;
    std::string defaultStoragePoolName;

    unsigned int monitoringInterval;
    unsigned int minCPUUsageThreshold;
    unsigned int maxCPUUsageThreshold;

    std::string clientIP;
    unsigned int clientPort;

    std::string loadBalancerIP;
    unsigned int loadBalancerPort;

    unsigned int workerPort;

    std::string baseVMName;
    bool isBaseVMCreated;
    bool isBaseVMRunning;

    std::string server1VMName;
    std::string server1VMIP;
    bool isServer1VMCreated;
    bool isServer1VMRunning;

    std::string server2VMName;
    std::string server2VMIP;
    bool isServer2VMCreated;
    bool isServer2VMRunning;

    std::string debugEnableEnvVarname;
};

#endif
