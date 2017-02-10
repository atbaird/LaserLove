#pragma once

#ifndef THREADSAFE_QUEUE_HPP
#define THREADSAFE_QUEUE_HPP
#include <queue>
#include <mutex>
#include "Engine/Utils/AllocationsSpecial/untrackedallocations.hpp"


template <typename type>
class threadsafe_queue : protected std::queue<type>
{
	std::mutex mtx;
	//bool cs;
public:
	void enque(const type& val) //push_back
	{
		mtx.lock();
		//Lock(cs);
		push(val);
		mtx.unlock();
		//Unlock(cs);
	}

	const bool deque(type* out)
	{
		bool result = false;
		mtx.lock();
		//Lock(cs);
		if (size() > 0)
		{
			*out = front();
			pop();
			//queue::pop_front();
			result = true;
		}
		mtx.unlock();
		//Unlock(cs);
		return result;
	}

	const size_t size() const
	{
		return queue::size();
		//size_t count = 0;
		//type* cur = front;
		//while (cur != back)
		//{
		//	count++;
		//	cur++;
		//}
		//return count;
	}

	const bool testIsContained(type t) const
	{
		type* cur = front;
		for (size_t i = 0; i < size(); i++)
		{
			type test = *cur;
			if (t == test)
			{
				return true;
			}
			cur++;
		}
		return false;
	}

	const int GetIndexOfContained(type t) const
	{
		type* cur = front;
		for (size_t i = 0; i < size(); i++)
		{
			type test = *cur;
			if (t == test)
			{
				return i;
			}
			cur++;
		}
		return -1;
	}
};
#endif