#pragma once

#ifndef EVENT_HPP
#define EVENT_HPP
#include <vector>

template <typename ...Args>
class Event
{
public:
	struct Subscription;
	typedef void(*sub_callback)(Subscription* sub, Args...);
	typedef void(*function_cb)(Args...);

	struct Subscription
	{
		sub_callback utility_callback; //C style function the trigger actuall calls
		void* callback; // is callback registered by user?
		void* argument; //additional data that goes w/ subscription (optional)
		size_t priority; //Lower the priority, more important it is.

		// Left as an excercise to reader - hint:  just keep list sorted
		// int_t order_number;   // If priority is something you care about in events
		
		// Used in - place linked for these instead vectors, can save space
		// if you use a global pool for subscriptions... but not needed - vector is good starting point
	};
private:
	std::vector<Subscription> subscriptions;

	void RegisterSubscription(sub_callback util_cb, void* actual_cb, void* data, size_t priority);
	void UnregisterSubscription(void* util_cb, void* actual_cb, void* data);

public:

	template<typename T>
	static void MethodCallback(Subscription* sub, Args... args)
	{
		typedef void(T::* Callback)(Args...);
		Callback mcb = *(Callback*)(&sub->callback);
		T* obj_ptr = (T*)sub->argument;
		(obj_ptr->*mcb)(args...);
	}
	static void EventFunctionCallback(Subscription* sub, Args ...args);

	void RegisterFunction(function_cb cb, size_t priority = 1);
	void UnregisterFunction(function_cb cb);

	template <typename T>
	void RegisterMethod(T* object, void (T::*mcb)(Args...), size_t priority = 1)
	{
		void* unique_ptr = *(void**)(&mcb);
		RegisterSubscription(MethodCallback<T>, unique_ptr, object, priority);
	}

	template<typename T>
	void UnregisterMethod(T* object, void(T::*mcb)(Args...))
	{
		//Exercise for the reader
		void* unique_ptr = *(void**)(&mcb);
		UnregisterSubscription(MethodCallback<T>, unique_ptr, object);
	}

	void Trigger(Args... args);
	
};

//------------------------------------------------------------------------------------------------------------
template <typename ...Args>
void Event<Args...>::RegisterSubscription(sub_callback util_cb, void* actual_cb, void* data, size_t priority)
{
	Subscription sub;
	sub.utility_callback = util_cb;
	sub.callback = actual_cb;
	sub.argument = data;
	sub.priority = priority;
	if (subscriptions.size() == 0)
	{
		//Just insert if no subscriptions yet.
		subscriptions.push_back(sub);
		return;
	}
	else
	{
		//Find correct spot for subscription, then insert it.
		bool inserted = false;
		size_t lastPriorityLvl = 0;
		size_t currentPriorityLvl = 0;
		for (size_t i = 0; i < subscriptions.size(); i++)
		{
			currentPriorityLvl = subscriptions.at(i).priority;
			if (lastPriorityLvl != currentPriorityLvl)
			{
				if (currentPriorityLvl > priority)
				{
					subscriptions.insert(subscriptions.begin() + i, sub);
					inserted = true;
					break;
				}
			}
			lastPriorityLvl = currentPriorityLvl;
		}
		if (inserted == false)
		{
			subscriptions.push_back(sub);
		}
	}
}

//------------------------------------------------------------------------------------------------------------
template <typename ...Args>
void Event<Args...>::UnregisterSubscription(void* util_cb, void* actual_cb, void* data)
{
	//left as exercise to the reader
	for (size_t i = 0; i < subscriptions.size(); i++)
	{
		Subscription sub = Subscription.at(i);
		if (sub.argument == data && sub.callback == actual_cb && sub.utility_callback = util_cb)
		{
			subscriptions.erase(subscriptions.begin() + i);
			break;
		}
	}
}

//------------------------------------------------------------------------------------------------------------
template <typename ...Args>
void Event<Args...>::EventFunctionCallback(Subscription* sub, Args ...args)
{
	function_cb cb = (function_cb)(sub->callback);
	cb(args...);
}

//------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------
template <typename ...Args>
void Event<Args...>::RegisterFunction(function_cb cb, size_t priority)
{
	RegisterSubscription(EventFunctionCallback, cb, nullptr, priority);
}

//------------------------------------------------------------------------------------------------------------
template <typename ...Args>
void Event<Args...>::UnregisterFunction(function_cb cb)
{
	UnregisterSubscription(EventFunctionCallback, cb, nullptr);
}

//------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------
template<typename ...Args>
void Event<Args...>::Trigger(Args... args)
{
	for (auto sub : subscriptions)
	{
		(sub).utility_callback(&sub, args...);
	}
}

#endif