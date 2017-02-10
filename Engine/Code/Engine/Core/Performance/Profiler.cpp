#include "Engine/Core/Performance/Profiler.hpp"
#include "Engine/Console/Console.hpp"
#include "Engine/Utils/Profiler/ProfilerNode.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/Performance/Performance.hpp"
#include "Engine/Core/FileUtils.hpp"
#include "Engine/Core/StringUtils.hpp"

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
//Static variables
bool Profiler::ProfilingDesiredEnabled = true;
bool Profiler::ProfilingEnabled = false;
ProfilerNode* Profiler::s_CurrentSample = nullptr;
ProfilerNode* Profiler::s_PrevFrame = nullptr;
ProfilerNode* Profiler::s_CurrentFrame = nullptr;
std::vector<int>* Profiler::s_activeIDs = nullptr;
size_t Profiler::s_previousFrameCount = 0;
size_t Profiler::s_currentFrameCount = 0;
size_t Profiler::s_framesProfiled = 0;
size_t Profiler::s_firstToRender = 0;
size_t Profiler::s_numPushesOverMaxTracked = 0;
double Profiler::s_longestTimeTaken = 0.0;
double Profiler::s_lastFrameTiming = 0.0;
std::vector<const char*>* Profiler::s_tagsOfLastFrame = nullptr;
std::vector<uint64_t>* Profiler::s_timesOfLastFrame = nullptr;
ObjectPool<ProfilerNode> Profiler::objectPoolNodes;


//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
//Console command methods
void Profiler::ToggleProfiling(Command& com)
{
	com;
	ToggleDesiredProfilingEnabled();
	if (g_Console != nullptr)
	{
		if (ProfilingDesiredEnabled)
		{
			g_Console->ConsolePrint("Desired Profiling enabled.");
		}
		else
		{
			g_Console->ConsolePrint("Desired Profiling disabled.");
		}
	}
}

void Profiler::ConsoleProfilingPrintLastFrame(Command& com)
{
	com;
	if (s_tagsOfLastFrame == nullptr || s_timesOfLastFrame == nullptr)
	{
		return;
	}
	bool saveToFile = false;
	std::string saveTo = com.get_string_by_index(0);
	saveTo = StartFromBackAndChopOffAfterEncounterChar(saveTo, '/');
	saveTo = StartFromBackAndChopOffAfterEncounterChar(saveTo, '\\');
	if (saveTo.size() > 0)
	{
		saveTo = "Data/Profiling/" + saveTo;
		saveToFile = true;
		if (saveTo.size() >= 4)
		{
			std::string sub = saveTo.substr(saveTo.size() - 4, 4);
			if (SimpleStrCmpLower(sub, ".txt") != true)
			{
				saveTo += ".txt";
			}
		}
		else
		{
			saveTo += ".txt";
		}
	}


	//std::vector<const char*> sortedTags;
	//std::vector<uint64_t> sortedTimeDiffs;
	//Profiler::GetSortedLastFrameTimes(sortedTags, sortedTimeDiffs);

	std::string wholeFile = "";
	double currentFrameTime = ConvertUint64_tToDoubleSeconds((s_PrevFrame->GetStop() - s_PrevFrame->GetStart()));
	if (saveToFile)
	{
		wholeFile += "This frame\'s total seconds taken: " + std::to_string(currentFrameTime);
		wholeFile += "\nLongest frame total seconds taken: " + std::to_string(s_longestTimeTaken);
		wholeFile += "\nTotal number of frames tested: " + std::to_string(s_framesProfiled) + "\n-----\n";
	}
	if (g_Console != nullptr)
	{
		g_Console->ConsolePrint("---");
		g_Console->ConsolePrint("This frame\'s total seconds taken: " + std::to_string(currentFrameTime));
		g_Console->ConsolePrint("Longest frame total seconds taken: " + std::to_string(s_longestTimeTaken));
		g_Console->ConsolePrint("Total number of frames tested: " + std::to_string(s_framesProfiled));
		g_Console->ConsolePrint("-----");
	}

	for (size_t i = 0; i < s_timesOfLastFrame->size(); i++)
	{

		double seconds = ConvertUint64_tToDoubleSeconds(s_timesOfLastFrame->at(i));
		//std::string message = std::string(sortedTags.at(i)) + " : " + std::to_string(seconds) + " seconds";
		std::string message = std::string(s_tagsOfLastFrame->at(i)) + " : " + std::to_string(seconds) + " seconds, " + std::to_string(seconds / currentFrameTime) + "% of Frame Time";

		if (g_Console != nullptr)
		{
			g_Console->ConsolePrint(message);
		}
		if (saveToFile)
		{
			wholeFile += message + "\n";
		}
	}
	if (g_Console != nullptr)
	{
		g_Console->ConsolePrint("-----");
		g_Console->ConsolePrint("---");
	}
	if (saveToFile)
	{
		wholeFile += "-----";
		FileUtils::SaveBufferToTextFile(saveTo, wholeFile);
	}
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
//Console help command methods
void Profiler::HelpToggleProfiling(Command& com)
{
	com;
	if (g_Console != nullptr)
	{
		g_Console->ConsolePrint("toggleprofiler toggles profiling on and off.");
	}
}

void Profiler::HelpConsoleProfilingPrintLastFrame(Command& com)
{
	com;
	if (g_Console != nullptr)
	{
		g_Console->ConsolePrint("ProfilerPrintLastFrame will print out the last frame\'s data based on how much time each one took.");
	}
}


//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
//Converters
double Profiler::ConvertUint64_tToDoubleSeconds(uint64_t time)
{
	return (double(time) / 1000000);
}

#if defined(Profiling_Config) && (Profiling_Config == 1)
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
//Startup/Update/Shutdown
void Profiler::ProfilerSystemStartup(size_t numToAllocate)
{
	s_framesProfiled = 0;
	s_longestTimeTaken = 0.0;
	s_lastFrameTiming = 0.0;
	s_tagsOfLastFrame = new std::vector<const char*>();
	s_timesOfLastFrame = new std::vector<uint64_t>();
	objectPoolNodes.init(numToAllocate);
	ProfilingEnabled = true;
}

void Profiler::ProfilerSystemsShutdown()
{
	if (s_PrevFrame != nullptr)
	{
		Delete(s_PrevFrame);
		s_PrevFrame = nullptr;
	}
	if (s_CurrentFrame != nullptr)
	{
		Delete(s_CurrentFrame);
		s_CurrentFrame = nullptr;
	}
	if (s_CurrentSample != nullptr)
	{
		s_CurrentSample = nullptr;
	}
	if (s_tagsOfLastFrame != nullptr)
	{
		delete s_tagsOfLastFrame;
		s_tagsOfLastFrame = nullptr;
	}
	if (s_timesOfLastFrame != nullptr)
	{
		delete s_timesOfLastFrame;
		s_timesOfLastFrame = nullptr;
	}
	if (s_activeIDs != nullptr)
	{
		delete s_activeIDs;
		s_activeIDs = nullptr;
	}
	objectPoolNodes.exit();
	ProfilingEnabled = false;
}

void Profiler::ProfileFrameMark()
{
	FrameEnd();
	FrameStart();
}


void Profiler::FrameEnd()
{
	if (ProfilingEnabled == false)
	{
		return;
	}
	ASSERT_OR_DIE(s_CurrentFrame == s_CurrentSample, "CurrentSample does not equal CurrentFrame!");
	Pop();
	if (s_CurrentFrame != nullptr)
	{
		s_framesProfiled++;
		s_lastFrameTiming = ConvertUint64_tToDoubleSeconds(s_CurrentFrame->GetStop() - s_CurrentFrame->GetStart());
		if (s_lastFrameTiming > s_longestTimeTaken)
		{
			s_longestTimeTaken = s_lastFrameTiming;
		}
	}
	Delete(s_PrevFrame);
	s_PrevFrame = nullptr;
	s_PrevFrame = s_CurrentFrame;
	GetSortedLastFrameTimes(s_tagsOfLastFrame, s_timesOfLastFrame);
	s_previousFrameCount = s_currentFrameCount;
	s_CurrentFrame = nullptr;
	s_CurrentSample = nullptr;
	s_currentFrameCount = 0;
	ProfilingEnabled = ProfilingDesiredEnabled;
}

void Profiler::FrameStart()
{
	if (ProfilingEnabled == false || ProfilingDesiredEnabled == false)
	{
		return;
	}
	Push("frame");
	s_CurrentFrame = nullptr;
	s_CurrentFrame = s_CurrentSample;
}

void Profiler::Delete(ProfilerNode* node)
{
	if (node == nullptr)
	{
		return;
	}
	for(int i = node->GetChildCount() -1; i >= 0; i--)
	{
		ProfilerNode* child = node->GetEditableChildByIdx(i);
		ProfilerNode::UnhookProfilerNodeFromNeighborsAndParent(child);
		Delete(child);
	}
	if (node->GetDeleteTag() == true)
	{
		delete node->GetTag();
	}
	objectPoolNodes.Dealloc(node);

}

void Profiler::Pop()
{
	if (s_CurrentSample == nullptr)
	{
		return;
	}
	int max = PROFILING_MAX_TRACKED_FRAMES;
	if (max > 0 && s_numPushesOverMaxTracked > 0)
	{
		s_numPushesOverMaxTracked--;
	}
	uint64_t stop = Performance::GetCurrentPerformanceCount();
	s_CurrentSample->SetStop(stop);
	ProfilerNode* parent = s_CurrentSample->GetEditableParent();
	if (parent != nullptr)
	{
		s_CurrentSample = nullptr;
		s_CurrentSample = parent;
	}
}

void Profiler::Push(const char* tag)
{
	if (ProfilingEnabled == false)
	{
		return;
	}
	int max = PROFILING_MAX_TRACKED_FRAMES;
	if (max > 0 && s_currentFrameCount >= PROFILING_MAX_TRACKED_FRAMES)
	{
		s_numPushesOverMaxTracked++;
		return;
	}

	uint64_t start = Performance::GetCurrentPerformanceCount();
	ProfilerNode* newNode = objectPoolNodes.Alloc(tag);
	s_currentFrameCount++;
	newNode->SetStart(start);
	if (s_CurrentSample != nullptr)
	{
		s_CurrentSample->AddChild(newNode);
		newNode->SetParent(s_CurrentSample);
	}
	s_CurrentSample = nullptr;
	s_CurrentSample = newNode;
}

ProfilerNode* Profiler::ProfilerGetLastFrame()
{
	return s_PrevFrame;
}

const bool Profiler::ProfilerLogSection(int id)
{
	//s_activeIDs
	bool isWithinIDs = DetermineIfIDWithinUsedIDs(id);
	if (isWithinIDs == true)
	{
		ProfilerNode* node = s_CurrentSample;
		int sid = s_CurrentSample->GetID();
		GUARANTEE_OR_DIE(sid == id, "CURRENT SAMPLE NODE ID DOES NOT EQUAL ID WANTED TO SHUT DOWN!");
		Pop();
		if (s_activeIDs != nullptr)
		{
			for (size_t i = 0; i < s_activeIDs->size(); i++)
			{
				if (id == s_activeIDs->at(i))
				{
					s_activeIDs->erase(s_activeIDs->begin() + i);
					break;
				}
			}
		}
		uint64_t time = node->GetStop() - node->GetStart();
		float timeSeconds = (float)Performance::PerformanceCountToSeconds(time);
		DebuggerPrintf("Node %i took %.8f seconds\n", sid, timeSeconds);
		return false;

	}
	else
	{
		char* star = new char[11];
		star[10] = '\0';
		itoa(id, star, 10);
		Push(star);
		s_CurrentSample->SetID(id);
		s_CurrentSample->SetDeleteTag(true);

		if (s_activeIDs == nullptr)
		{
			s_activeIDs = new std::vector<int>();
		}
		s_activeIDs->push_back(id);
	}
	return true;
}

const bool Profiler::DetermineIfIDWithinUsedIDs(int id)
{
	if (s_activeIDs == nullptr)
	{
		return false;
	}
	bool isWithin = false;

	for (size_t i = 0; i < s_activeIDs->size(); i++)
	{
		if (s_activeIDs->at(i) == id)
		{
			isWithin = true;
			break;
		}
	}

	return isWithin;
}

void Profiler::GetDataOutOfProfilerNodeTree(ProfilerNode* current, std::vector<const char*>& tags, std::vector<uint64_t>& timeDiff)
{
	if (current == nullptr)
	{
		return;
	}
	tags.push_back(current->GetTag());
	timeDiff.push_back(current->GetStop() - current->GetStart());
	for (int i = 0; i < current->GetChildCount(); i++)
	{
		ProfilerNode* node = current->GetEditableChildByIdx(i);
		GetDataOutOfProfilerNodeTree(node, tags, timeDiff);
	}
}
void Profiler::GetSortedLastFrameTimes(std::vector<const char*>* sortedTags, std::vector<uint64_t>* sortedTimeDiffs)
{
	if (sortedTags == nullptr || sortedTimeDiffs == nullptr)
	{
		return;
	}
	sortedTags->clear();
	sortedTimeDiffs->clear();
	std::vector<const char*> variables;
	std::vector<uint64_t> timeDiff;
	std::vector<ProfilerNode*> alreadyadded;
	GetDataOutOfProfilerNodeTree(s_PrevFrame, variables, timeDiff);

	size_t orgSize = timeDiff.size();
	for (size_t i = 0; i < orgSize; i++)
	{
		const char* tagOfBiggest;
		uint64_t biggest = 0;
		size_t idx = 0;
		for (size_t j = 0; j < timeDiff.size(); j++)
		{
			if (timeDiff.at(j) > biggest)
			{
				tagOfBiggest = variables.at(j);
				biggest = timeDiff.at(j);
				idx = j;
			}
		}
		timeDiff.erase(timeDiff.begin() + idx);
		variables.erase(variables.begin() + idx);
		sortedTags->push_back(tagOfBiggest);
		sortedTimeDiffs->push_back(biggest);
	}

}

void Profiler::RegisterProfilerConsoleCommands()
{
	const RegisterCommandHelper s_toggleProfiler("toggleprofiler", ToggleProfiling, HelpToggleProfiling);
	const RegisterCommandHelper s_profilingPrintLastFrame("ProfilingPrintLastFrame", ConsoleProfilingPrintLastFrame, HelpConsoleProfilingPrintLastFrame);
}


//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
//Enable/DisableProfiling
void Profiler::ToggleDesiredProfilingEnabled()
{
	ProfilingDesiredEnabled = !ProfilingDesiredEnabled;
}

void Profiler::SetDesiredProfilingEnabled(bool enabled)
{
	ProfilingDesiredEnabled = enabled;
}

bool Profiler::GetProfilingEnabled()
{
	return ProfilingEnabled;
}

bool Profiler::GetDesiredProfilingEnabled()
{
	return ProfilingDesiredEnabled;
}

#else
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
//Startup/Update/Shutdown
void Profiler::ProfilerSystemStartup(size_t)
{

}

void Profiler::ProfilerSystemsShutdown()
{

}

void Profiler::ProfileFrameMark()
{

}

void Profiler::FrameEnd()
{

}

void Profiler::FrameStart()
{

}

void Profiler::Delete(ProfilerNode*)
{

}

void Profiler::Pop()
{

}

void Profiler::Push(const char*)
{

}


ProfilerNode* Profiler::ProfilerGetLastFrame()
{
	return nullptr;
}

const bool Profiler::ProfilerLogSection(int)
{
	return false;
}

const bool Profiler::DetermineIfIDWithinUsedIDs(int id)
{
	return false;
}

void Profiler::GetDataOutOfProfilerNodeTree(ProfilerNode* current, std::vector<std::string>& tags, std::vector<uint64_t>& timeDiff)
{

}

void Profiler::RegisterProfilerConsoleCommands()
{
	const RegisterCommandHelper s_toggleProfiler("toggleprofiler", ToggleProfiling, HelpToggleProfiling);
}


//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
//Enable/DisableProfiling
void Profiler::ToggleDesiredProfilingEnabled()
{
	ProfilingDesiredEnabled = false;
}

void Profiler::SetDesiredProfilingEnabled(bool enabled)
{
	enabled;
	ProfilingDesiredEnabled = false;
}

bool Profiler::GetProfilingEnabled()
{
	return false;
}


bool Profiler::GetDesiredProfilingEnabled()
{
	return ProfilingDesiredEnabled;
}
#endif