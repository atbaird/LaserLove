#include "Engine/Core/Performance/Performance.hpp"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>



uint64_t Performance::GetCurrentPerformanceCount()
{
	LARGE_INTEGER out_initialTime;
	QueryPerformanceCounter(&out_initialTime);
	return out_initialTime.QuadPart;
}

double Performance::PerformanceCountToSeconds(uint64_t& performanceCount)
{
	LARGE_INTEGER countsPerSecond;
	QueryPerformanceFrequency(&countsPerSecond);
	double secondsPerCount = static_cast<double>(1.f / countsPerSecond.QuadPart);
	return (double)(static_cast<double>(performanceCount) * secondsPerCount);
}

double Performance::GetCurrentPerformanceCountSeconds()
{
	uint64_t performanceCount = GetCurrentPerformanceCount();
	return PerformanceCountToSeconds(performanceCount);
}
