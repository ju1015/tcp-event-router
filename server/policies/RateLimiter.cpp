#include "RateLimiter.h"
#include <chrono>

bool RateLimiter::allow(Client& client) {
    using namespace std::chrono;

    auto now = steady_clock::now();
    auto& rate = client.rate;

    // Refill tokens every second
    auto elapsed = duration_cast<seconds>(now - rate.lastRefill).count();
    if (elapsed >= 1) {
        rate.tokens = 10; // reset per second
        rate.lastRefill = now;
    }

    if (rate.tokens <= 0) {
        return false;
    }

    rate.tokens--;
    return true;
}
