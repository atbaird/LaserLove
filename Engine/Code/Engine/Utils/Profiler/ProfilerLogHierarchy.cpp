#include "Engine/Utils/Profiler/ProfilerLogHierarchy.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"


ProfilerLogHierarchy::ProfilerLogHierarchy()
{
	LogSample(ProfileSample::gFrameSample, 0);
}
ProfilerLogHierarchy::~ProfilerLogHierarchy()
{
}

void ProfilerLogHierarchy::LogSample(ProfileSample* s, int depth)
{
	if (s == nullptr)
	{
		return;
	}
	//DebuggerPrintf("%*s % took %.6fs", depth, "-", s->tag, (s->op_duration));
	
	std::vector<ProfileSample*> children = s->GetChildren();
	for (size_t i = 0; i < children.size(); i++)
	{
		LogSample(children.at(i), depth + 1);
	}

}