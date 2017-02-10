#pragma once

#include "Engine/Console/RegisterCommandHelper.hpp"
#include "Engine/Utils/ObjectPool.hpp"
#if !defined(Profiling_Config)
#define Profiling_Config 1
#endif

#if !defined(PROFILING_MAX_TRACKED_FRAMES)
#define PROFILING_MAX_TRACKED_FRAMES 0
#else
#define PROFILING_MAX_TRACKED_FRAMES
#endif


#ifndef PROFILER_HPP
#define PROFILER_HPP

class ProfilerNode;

class Profiler
{
private:
	static bool ProfilingDesiredEnabled;
	static bool ProfilingEnabled;
	static ProfilerNode* s_CurrentSample;
	static ProfilerNode* s_PrevFrame;
	static ProfilerNode* s_CurrentFrame;
	static size_t s_previousFrameCount;
	static size_t s_currentFrameCount;
	static size_t s_framesProfiled;
	static size_t s_firstToRender;
	static size_t s_numPushesOverMaxTracked;
	static double s_longestTimeTaken;
	static double s_lastFrameTiming;
	static std::vector<const char*>* s_tagsOfLastFrame;
	static std::vector<uint64_t>* s_timesOfLastFrame;
	static ObjectPool<ProfilerNode> objectPoolNodes;
	static std::vector<int>* s_activeIDs;
public:

	//Console command methods
	static void ToggleProfiling(Command& com);
	static void ConsoleProfilingPrintLastFrame(Command& com);

	//Console help command methods
	static void HelpToggleProfiling(Command& com);
	static void HelpConsoleProfilingPrintLastFrame(Command& com);

	//Converters
	static double ConvertUint64_tToDoubleSeconds(uint64_t time);

	//Startup/Update/Shutdown
	static void ProfilerSystemStartup(size_t numToAllocate = 500);
	static void ProfilerSystemsShutdown();
	static void ProfileFrameMark();
	static void FrameEnd();
	static void FrameStart();
	static void Delete(ProfilerNode*);
	static void Pop();
	static void Push(const char* tag);
	static ProfilerNode* ProfilerGetLastFrame();
	static const bool ProfilerLogSection(int id); //DO NOT use ID = 0, returns whether it started a node, returns false if stopped a node.
	static const bool DetermineIfIDWithinUsedIDs(int id);
	
	static void GetDataOutOfProfilerNodeTree(ProfilerNode* current, std::vector<const char*>& tags, std::vector<uint64_t>& timeDiff);
	static void GetSortedLastFrameTimes(std::vector<const char*>* sortedTags, std::vector<uint64_t>* sortedTimeDiffs);

	static void RegisterProfilerConsoleCommands();

	//Enable/DisableProfiling
	static void ToggleDesiredProfilingEnabled();
	static void SetDesiredProfilingEnabled(bool enabled);
	static bool GetProfilingEnabled();
	static bool GetDesiredProfilingEnabled();
};

//Macros
#define PROFILE_LOG_SECTION(id) Profiler::ProfilerLogSection ( id );

#endif
