//-----------------------------------------------------------------------------------------------
// Time.cpp
//	A simple high-precision time utility function for Windows
//	based on code by Squirrel Eiserloh

//-----------------------------------------------------------------------------------------------
#include "Engine/Core/Time.hpp"
#include "Engine/Core/Performance/Performance.hpp"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


//-----------------------------------------------------------------------------------------------
const double InitializeTime(uint64_t& out_initialTime)
{
	//Initializes time
	LARGE_INTEGER countsPerSecond;
	QueryPerformanceFrequency(&countsPerSecond);
	out_initialTime = Performance::GetCurrentPerformanceCount();
	return(1.0 / static_cast<double>(countsPerSecond.QuadPart));
}


//-----------------------------------------------------------------------------------------------
const double GetCurrentTimeSeconds()
{
	//returns the current time in seconds.
	static uint64_t initialTime;
	static double secondsPerCount = InitializeTime(initialTime);
	uint64_t currentCount = Performance::GetCurrentPerformanceCount();
	LONGLONG elapsedCountsSinceInitialTime = currentCount - initialTime;

	double currentSeconds = static_cast<double>(elapsedCountsSinceInitialTime)* secondsPerCount;
	return currentSeconds;
}


