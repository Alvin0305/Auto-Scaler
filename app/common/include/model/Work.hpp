#ifndef WORK_HPP
#define WORK_HPP

#include <nlohmann/json.hpp>
#include <ostream>
#include <string>

using json = nlohmann::json;

struct Work {
    std::string work;
    unsigned int value;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Work, work, value)

inline std::ostream &operator<<(std::ostream &out, const Work &work) {
    out << "{ value: " << work.value << " }";
    return out;
}

#endif

/*
{
	"work": "fibonacci",
	"value": 10
}
/*
