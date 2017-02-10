#pragma once

#ifndef CRITICALSECTION_HPP
#define CRITICALSECTION_HPP
#include <mutex>

/*
CRITICAL_SECTION
Constructor:
	InitializeCriticalSectionAndSpinCount(&cs, 8);
	EnterCriticalSection(&cs);
	LeaveCriticalSection(&cs);
	TryEnterCriticalSection(&cs);

std::mutex
	lock
	unlock
	try_lock

*/


class CriticalSection
{
	std::mutex mutex;
public:
	CriticalSection(const CriticalSection& copy);

	void lock();
	void unlock();
	const bool try_lock();
};
#endif