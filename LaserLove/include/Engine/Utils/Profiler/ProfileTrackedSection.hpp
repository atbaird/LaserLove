#pragma once

#ifndef PROFILETRACKEDSECTION_HPP
#define PROFILETRACKEDSECTION_HPP
#include "Engine/Utils/Profiler/ProfileSample.hpp"

class ProfileTrackedSection
{
private:
	unsigned long int op_count;
	ProfileSample* sample;
public:
	ProfileTrackedSection();
	~ProfileTrackedSection();

	void start(const char* tag);
	void end();

	
};
#endif