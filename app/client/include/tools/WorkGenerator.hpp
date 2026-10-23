#ifndef WORK_GENERATOR_HPP
#define WORK_GENERATOR_HPP

#include "model/Config.hpp"
#include "model/LoadState.hpp"
#include "model/Work.hpp"
#include <string>

class WorkGenerator {
  private:
    const std::string work;
    const Range workValueRangeInLowState;
    const Range workValueRangeInHighState;

  public:
    explicit WorkGenerator(const std::string &work, const Range &workValueRangeInLowState, const Range &workValueRangeInHighState)
        : work(work), workValueRangeInLowState(workValueRangeInLowState), workValueRangeInHighState(workValueRangeInHighState) {};

    WorkGenerator(const WorkGenerator &other) = delete;
    WorkGenerator &operator=(const WorkGenerator &other) = delete;

    Work generate(const LoadState &state) const;
};

#endif
