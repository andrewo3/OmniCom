#ifndef UTIL_H
#define UTIL_H

#include <chrono>

// get time in milliseconds since epoch
inline long long epoch() {
    auto currentTimePoint = std::chrono::system_clock::now();
    auto durationSinceEpoch = currentTimePoint.time_since_epoch();
    auto millisecondsSinceEpoch = std::chrono::duration_cast<std::chrono::milliseconds>(durationSinceEpoch);
    return millisecondsSinceEpoch.count();
}


#endif