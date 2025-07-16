#include "EventHandler.h"

void EventHandler::Bind(EventCallback callback)
{
	listeners.push_back(callback);
}

void EventHandler::Broadcast()
{
	for (auto& cb : listeners) cb();
}
