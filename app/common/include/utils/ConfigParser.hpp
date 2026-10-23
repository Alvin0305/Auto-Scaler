#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include "model/Config.hpp"
#include <string>

class ConfigParser {
  public:
    ConfigParser() = default;

    Config parse(const std::string &configFilePath) const;
};

#endif
