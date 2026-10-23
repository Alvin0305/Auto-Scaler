#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <ostream>
#include <string>
#include <utility>

using Range = std::pair<unsigned int, unsigned int>;

struct Config {
    std::string qemuURL;
    std::string diskImageFileExtension;
    std::string defaultStoragePoolName;
    std::string defaultStoragePoolPath;

    std::string clientIP;
    unsigned int clientPort;

    std::string loadBalancerIP;
    unsigned int loadBalancerPort;

    unsigned int workerPort;

    std::string baseVMName;
    std::string server1VMName;
    std::string server2VMName;

    unsigned int monitoringInterval;
    unsigned int minCPUUsageThreshold;
    unsigned int maxCPUUsageThreshold;
    unsigned int peakCountForTrigger;

    unsigned int workIntervalInLowLoad;
    unsigned int workIntervalInHighLoad;

    Range workValueRangeInLowLoad;
    Range workValueRangeInHighLoad;

    std::string work;

    inline friend std::ostream &operator<<(std::ostream &out, const Config &config) {
        out << "\tQEMU URL=" << config.qemuURL << std::endl;
        out << "\tDisk Image File Extension=" << config.diskImageFileExtension << std::endl;
        out << "\tDefault Storage Pool Path=" << config.defaultStoragePoolPath << std::endl;
        out << "\tDefault Storage Pool Name=" << config.defaultStoragePoolName << std::endl;

        out << "\tClient IP=" << config.clientIP << std::endl;
        out << "\tClient Port=" << config.clientPort << std::endl;
        out << "\tLoad Balancer IP=" << config.loadBalancerIP << std::endl;
        out << "\tLoad Balancer Port=" << config.loadBalancerPort << std::endl;
        out << "\tWorked Port=" << config.workerPort << std::endl;

        out << "\tBase VM Name=" << config.baseVMName << std::endl;
        out << "\tServer1 VM Name=" << config.server1VMName << std::endl;
        out << "\tServer2 VM Name=" << config.server2VMName << std::endl;

        out << "\tMonitoring Interval=" << config.monitoringInterval << std::endl;
        out << "\tMin CPU Usage Threshold=" << config.minCPUUsageThreshold << std::endl;
        out << "\tMax CPU Usage Threshold=" << config.maxCPUUsageThreshold << std::endl;
        out << "\tPeak Count For Trigger=" << config.peakCountForTrigger << std::endl;

        out << "\tWork=" << config.work << std::endl;
        out << "\tWork Interval In Low Load=" << config.workIntervalInLowLoad << std::endl;
        out << "\tWork Interval In High Load=" << config.workIntervalInHighLoad << std::endl;
        out << "\tWork Value Range In Low Load=[" << config.workValueRangeInLowLoad.first << ", "
            << config.workValueRangeInLowLoad.second << "]" << std::endl;
        out << "\tWork Value Range In High Load=[" << config.workValueRangeInHighLoad.first << ", "
            << config.workValueRangeInHighLoad.second << "]" << std::endl;

        return out;
    }
};

#endif
