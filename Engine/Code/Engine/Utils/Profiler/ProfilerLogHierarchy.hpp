#pragma once

#ifndef PROFILERLOGHIERARCHY_HPP
#define PROFILERLOGHIERARCHY_HPP
#include "Engine/Utils/Profiler/ProfileSample.hpp"

class ProfilerLogHierarchy
{
private:
public:
	ProfilerLogHierarchy();
	~ProfilerLogHierarchy();

	void LogSample(ProfileSample* s, int depth);
};
#endif