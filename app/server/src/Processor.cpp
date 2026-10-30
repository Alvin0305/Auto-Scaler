#include "Processor.hpp"

int Processor::process(int num) const {
    if (num == 0) return 0;
    if (num == 1) return 1;
    return process(num - 1) + process(num - 2);
}
