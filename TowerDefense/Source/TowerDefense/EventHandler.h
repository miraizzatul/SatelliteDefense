#pragma once

#include <vector>
#include <functional>

//Use variadic template now in case we need to use functions with multiple paramaters in the future
//without variadic template, we have to write a new class for every combinations which is not ideal
template <typename... Args>
class EventHandler
{
public:
	using EventCallback = std::function<void(Args...)>;

	void AddListener(EventCallback listener)
	{
		listeners.push_back(std::move(listener));
	}
	
	void Broadcast(Args... args) const
	{
		for (const auto& listener : listeners)
		{
			listener(args...);
		}
	}

private:
	std::vector<EventCallback> listeners;
};
