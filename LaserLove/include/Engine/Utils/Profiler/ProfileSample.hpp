#pragma once

#ifndef PROFILESAMPLE_HPP
#define PROFILESAMPLE_HPP
#include <vector>

class ProfileSample
{
private:
	//globals
	static ProfileSample* gCurrent;

	//variables
	ProfileSample* parent;
	std::vector<ProfileSample*> children;
	ProfileSample* prev;
	ProfileSample* next;
	const char* tag;
	int start_op;
	int op_duration;
public:
	static ProfileSample* gFrameSample;
	//Constructors
	ProfileSample();
	~ProfileSample();

	//getters
	const std::vector<ProfileSample*> GetChildren() const;

	//static methods
	static ProfileSample* ProfileStartSample(const char* tag);
	static void ProfileEndSample(ProfileSample* sample);
	static void ProfileReportHierarchy();
	static void ProfileStartFrame();
	static void ProfileEndFrame();

private:
	//private static methods
	static void Append(ProfileSample* parent, ProfileSample* sample);
};
#endif