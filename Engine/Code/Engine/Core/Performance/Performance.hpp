#pragma once

#ifndef PERFORMANCE_HPP
#define PERFORMACE_HPP
#include <cstdint>

class Performance
{
private:
public:
	static uint64_t GetCurrentPerformanceCount();
	static double PerformanceCountToSeconds(uint64_t& performanceCount);
	static double GetCurrentPerformanceCountSeconds();
};

#endif