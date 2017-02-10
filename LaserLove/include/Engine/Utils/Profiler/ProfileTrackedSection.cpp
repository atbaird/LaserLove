#include "Engine/Utils/Profiler/ProfileTrackedSection.hpp"


ProfileTrackedSection::ProfileTrackedSection()
	: sample(nullptr)
{

}
ProfileTrackedSection::~ProfileTrackedSection()
{
	ProfileSample::ProfileEndSample(sample);
}

void ProfileTrackedSection::start(const char* tag)
{
	sample = ProfileSample::ProfileStartSample(tag);
}
void ProfileTrackedSection::end()
{
	ProfileSample::ProfileEndSample(sample);
}