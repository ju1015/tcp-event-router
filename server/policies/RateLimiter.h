#ifndef RATE_LIMITER_H
#define RATE_LIMITER_H

#include "../core/Client.h"

class RateLimiter {
public:
    static bool allow(Client& client);
};

#endif
