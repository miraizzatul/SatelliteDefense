#pragma once

#include <vector>
#include <functional>

class EventHandler
{
public:
	using EventCallback = std::function<void()>;

	void Bind(EventCallback callback);

	void Broadcast();

private:
	std::vector<EventCallback> listeners;
};
