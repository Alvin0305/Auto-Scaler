#include "tools/WorkGenerator.hpp"
#include "model/LoadState.hpp"
#include "model/Work.hpp"
#include "utils/RandomGenerator.hpp"

Work WorkGenerator::generate(const LoadState &state) const {
    unsigned int num;
    switch (state) {
        case LoadState::LOW:
            num = RandomGenerator::generate(workValueRangeInLowState.first, workValueRangeInLowState.second);
            return Work{work, num};
        case LoadState::HIGH:
            num = RandomGenerator::generate(workValueRangeInHighState.first, workValueRangeInHighState.second);
            return Work{work, num};
        default:
            return Work{0};
    }
}
