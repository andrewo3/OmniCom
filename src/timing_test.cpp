#include <cstdio>
#include <chrono>
#include <cstdint>

inline long long epoch_nano() {  //1*10^9
    auto currentTimePoint = std::chrono::steady_clock::now();
    auto durationSinceEpoch = currentTimePoint.time_since_epoch();
    auto nanosecondsSinceEpoch = std::chrono::duration_cast<std::chrono::nanoseconds>(durationSinceEpoch);
    return nanosecondsSinceEpoch.count();
}

int main() {
    long long last_call = epoch_nano();
    uint16_t i= 0;
    while (true) {
        long long current = epoch_nano()-last_call;
        if (current>22675.737) {
            printf("Taking too long: %lli\n",current);
        } else {
            printf("Good: %lli\n",current);
        }
        last_call = epoch_nano();
        i++;
    }
    return 0;
}