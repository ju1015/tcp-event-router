#include "EventRouter.h"
#include <iostream>
#include "../adapters/FileAdapter.h"
#include "../policies/RateLimiter.h"

void EventRouter::route(Client& client, const Event& event) {

	// 🔐 Handle authentication event
	if (event.type == EventType::AUTH) {
		if (event.payload == "my_gateway_key_123") {
			client.authenticate();
			client.sendText("AUTH_OK");
		} else {
			client.sendText("AUTH_FAIL");
			client.closeConnection();
		}
		return;
	}

	// 🔒 Block unauthenticated clients
	if (!client.isAuthenticated()) {
		client.sendText("ERROR: not authenticated");
		return;
	}
	// 🔒 Rate limiting check
	if (!RateLimiter::allow(client)) {
		client.sendText("ERROR: rate limit exceeded");
		return;
	}

	static FileAdapter fileAdapter("logs");
	bool accepted = true;

	switch (event.type) {

		case EventType::ALERT:
			std::cout << "[ALERT] " << event.payload << std::endl;			
			fileAdapter.write(event); 
			break;

		case EventType::METRIC:
			std::cout << "[METRIC] " << event.payload << std::endl;
			fileAdapter.write(event);
			break;

		case EventType::SECURITY:
			std::cout << "[SECURITY] " << event.payload << std::endl;
			fileAdapter.write(event);
			break;

		case EventType::INFO:
			std::cout << "[INFO] " << event.payload << std::endl;
			fileAdapter.write(event);
			break;

		default:
			accepted = false;
			break;
	}

	// ACK-style response
	if (accepted) {
		client.sendText("ACK");
	} else {
		client.sendText("ERROR: invalid event");
	}
}
