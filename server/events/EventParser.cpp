#include "EventParser.h"

Event EventParser::parse(const std::string& message) {


    if (message.rfind("AUTH ", 0) == 0) {
        std::string key = message.substr(5);
        return Event(EventType::AUTH, key);
    }

    size_t pos = message.find(':');

    // No TYPE → treat as INFO
    if (pos == std::string::npos) {
        return Event(EventType::INFO, message);
    }

    std::string typeStr = message.substr(0, pos);
    std::string payload = message.substr(pos + 1);

    if (typeStr == "ALERT") {
        return Event(EventType::ALERT, payload);
    }
    if (typeStr == "METRIC") {
        return Event(EventType::METRIC, payload);
    }
    if (typeStr == "SECURITY") {
        return Event(EventType::SECURITY, payload);
    }
    if (typeStr == "INFO") {
        return Event(EventType::INFO, payload);
    }

    // Unknown TYPE → downgrade to INFO
    return Event(EventType::INFO, message);
}
