#pragma once

#ifndef THREAD_HPP
#define THREAD_HPP
#include <thread>

class Thread
{
private:
	std::thread m_thread;
public:
	static Thread* ThreadCreate(void(*functionPtr)(void*), void* data = nullptr);
	static void ThreadDetach(Thread* thr); //destruction
	static const bool ThreadJoin(Thread* thr); //destruction.
	static void ThreadSleep(const float& seconds);
	static void ThreadYield();
	static unsigned int GetComputerCoreCount();
};
#endif