#ifndef VM_NOT_FOUND_EXCEPTION_HPP
#define VM_NOT_FOUND_EXCEPTION_HPP

#include <stdexcept>
#include <string>

class VMNotFoundException : std::runtime_error {
  private:
    std::string name;

  public:
    explicit VMNotFoundException(const std::string &name)
        : name(name), std::runtime_error(std::string("VM not found: ") + name) {};
};

#endif
