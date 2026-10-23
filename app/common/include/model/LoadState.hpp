#ifndef LOAD_STATE_HPP
#define LOAD_STATE_HPP

#include <ostream>

enum class LoadState { LOW, HIGH, NORMAL };

inline std::ostream &operator<<(std::ostream &out, const LoadState &state) {
    switch (state) {
        case LoadState::LOW:
            out << "LOW";
            break;
        case LoadState::HIGH:
            out << "HIGH";
            break;
        case LoadState::NORMAL:
            out << "NORMAL";
            break;
        default:
            out << "Unknown";
            break;
    }

    return out;
}

#endif
