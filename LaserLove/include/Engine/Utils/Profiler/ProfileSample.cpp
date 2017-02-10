#include "Engine/Utils/Profiler/ProfileSample.hpp"

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//Globals
ProfileSample* ProfileSample::gCurrent = nullptr;
ProfileSample* ProfileSample::gFrameSample = nullptr;

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//Constructors
ProfileSample::ProfileSample()
	: parent(nullptr),
	next(nullptr),
	prev(nullptr)
{

}
ProfileSample::~ProfileSample()
{

}


//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//getters
const std::vector<ProfileSample*> ProfileSample::GetChildren() const
{
	return children;
}

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//Static methods
ProfileSample* ProfileSample::ProfileStartSample(const char* tag)
{
	ProfileSample* sample = nullptr;// AllocSample();
	return sample;
	Append(gCurrent, sample);
	sample->tag = tag;
	sample->parent = gCurrent;
	gCurrent = nullptr;
	gCurrent = sample;
	sample->start_op = 0;// GetOpCount();
	return sample;
}
void ProfileSample::ProfileEndSample(ProfileSample* sample)
{
	if (sample == nullptr)
	{
		return;
	}
	sample->op_duration = 0;// GetOpCount() - sample->start_op;
	gCurrent = nullptr;
	gCurrent = sample->parent;
	//DeallocSample(sample);
}
void ProfileSample::ProfileReportHierarchy()
{

}
void ProfileSample::ProfileStartFrame()
{

}
void ProfileSample::ProfileEndFrame()
{

}

//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------
//Private static methods
void ProfileSample::Append(ProfileSample* parent, ProfileSample* sample)
{
	if (sample == nullptr
		|| parent == nullptr)
	{
		return;
	}
	parent->children.push_back(sample);
}