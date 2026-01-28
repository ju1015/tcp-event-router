#ifndef EVENT_PARSER_H
#define EVENT_PARSER_H

#include <string>
#include "Event.h"

class EventParser {
public:
    static Event parse(const std::string& message);
};

#endif
