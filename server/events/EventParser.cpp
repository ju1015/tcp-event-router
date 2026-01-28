#include "EventParser.h"

Event EventParser::parse(const std::string& message) {

    // Find separator
    size_t pos = message.find(':');
    if (pos == std::string::npos) {
        return Event(EventType::CHAT, message);
    }

    std::string typeStr = message.substr(0, pos);
    std::string payload = message.substr(pos + 1);

    if (typeStr == "CHAT") {
        return Event(EventType::CHAT, payload);
    }
    if (typeStr == "LOGIN") {
        return Event(EventType::LOGIN, payload);
    }
    if (typeStr == "LOGOUT") {
        return Event(EventType::LOGOUT, payload);
    }

    return Event(EventType::UNKNOWN, payload);
}
