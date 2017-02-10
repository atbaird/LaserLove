#include "Engine/Utils/Thread.hpp"
#include <chrono>


Thread* Thread::ThreadCreate(void (*functionPtr)(void*), void* data)
{
	Thread* thr = new Thread();
	thr->m_thread = std::thread(functionPtr, data);
	return thr;
}
void Thread::ThreadDetach(Thread* thr)
{
	if (thr != nullptr)
	{
		thr->m_thread.detach();
		delete thr;
	}
}

const bool Thread::ThreadJoin(Thread* thr)
{
	if (thr == nullptr)
	{
		return false;
	}
	thr->m_thread.join();
	delete thr;
	return true;
}

void Thread::ThreadSleep(const float& seconds)
{
	std::chrono::milliseconds cMS = (std::chrono::milliseconds)(int)(1000.f * seconds);//std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::duration<float>(ms));
	std::this_thread::sleep_for(cMS);
}

void Thread::ThreadYield()
{
	std::this_thread::yield();
}

unsigned int Thread::GetComputerCoreCount()
{
	return std::thread::hardware_concurrency();
}