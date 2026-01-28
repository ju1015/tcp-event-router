#ifndef EVENT_H
#define EVENT_H

#include <string>
#include<ctime>
enum class EventType {
	UNKNOWN,
	CHAT,
	LOGIN,
	LOGOUT,
	ALERT,      // something is wrong
	METRIC,     // numeric/system data
	SECURITY,   // auth, intrusion, violations
	INFO
};

struct Event {
	EventType type;
	std::string payload;
	std::string source;     // client IP or name
	time_t timestamp;
	Event(EventType t = EventType::UNKNOWN,
			const std::string& p = "")
		: type(t), payload(p),timestamp(time(nullptr)) {}
};

#endif
