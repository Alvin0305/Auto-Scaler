#ifndef CONFIG_PARSER_HPP
#define CONFIG_PARSER_HPP

#include "model/Config.hpp"
#include <string>

class ConfigParser {
  private:
    const std::string configPath;

  public:
    ConfigParser(const std::string &configPath) : configPath(configPath) {};

    Config parse() const;
};

#endif
