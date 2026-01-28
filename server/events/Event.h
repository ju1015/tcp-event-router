#ifndef EVENT_H
#define EVENT_H

#include <string>

enum class EventType {
    UNKNOWN,
    CHAT,
    LOGIN,
    LOGOUT
};

struct Event {
    EventType type;
    std::string payload;

    Event(EventType t = EventType::UNKNOWN,
          const std::string& p = "")
        : type(t), payload(p) {}
};

#endif
