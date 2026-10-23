#ifndef FORMATTER_HPP
#define FORMATTER_HPP

#include "model/Config.hpp"
#include "model/LoadState.hpp"
#include "model/Message.hpp"
#include "model/Work.hpp"
#include "utils/utils.hpp"
#include <format>
#include <libvirt/libvirt.h>
#include <sstream>
#include <string>

template <> struct std::formatter<virDomainInfo> : std::formatter<std::string> {
    auto format(const virDomainInfo &vm, auto &ctx) const {
        std::ostringstream oss;
        oss << vm;

        return std::formatter<std::string>::format(oss.str(), ctx);
    }
};

template <> struct std::formatter<Work> : std::formatter<std::string> {
    auto format(const Work &work, auto &ctx) const {
        std::ostringstream oss;
        oss << work;

        return std::formatter<std::string>::format(oss.str(), ctx);
    }
};

template <> struct std::formatter<Message> : std::formatter<std::string> {
    auto format(const Message &message, auto &ctx) const {
        std::ostringstream oss;
        oss << message;

        return std::formatter<std::string>::format(oss.str(), ctx);
    }
};

template <> struct std::formatter<Config> : std::formatter<std::string> {
    auto format(const Config &config, auto &ctx) const {
        std::ostringstream oss;
        oss << config;

        return std::formatter<std::string>::format(oss.str(), ctx);
    }
};

template <> struct std::formatter<LoadState> : std::formatter<std::string> {
    auto format(const LoadState &state, auto &ctx) const {
        std::ostringstream oss;
        oss << state;

        return std::formatter<std::string>::format(oss.str(), ctx);
    }
};

#endif
