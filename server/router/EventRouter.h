#ifndef EVENT_ROUTER_H
#define EVENT_ROUTER_H

#include "../events/Event.h"
#include "../core/Client.h"
#include "../adapters/FileAdapter.h"

class EventRouter {
private:
	FileAdapter fileAdapter;
public:
    void route(Client& client, const Event& event);
};

#endif
