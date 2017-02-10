#pragma once

#ifndef CALLSTACK_HPP
#define CALLSTACK_HPP
#include <stdlib.h>
#include <stdint.h>
#include <cstdlib>
#include <vector>
#include "Engine/Utils/CBuffer.hpp"
#include <ctime>

//Callstack = (Callstack*)(buffer + 0);
//offset += sizeof(Callstack);

//sizeof(Callstack) + size(void*) * framecount;

struct callstack_line_t
{
	callstack_line_t();
	char filename[128];
	char function_name[128];
	uint32_t line;
	uint32_t offset;
};

struct Callstack
{
	static Callstack* gCallstack;
	void** frames;
	size_t frame_count;
	size_t bytes_worth;
	tm* timeCreated;

	
	static const bool CallstackSystemInit();
	static void CallstackSystemDeinit();
	static Callstack* CreateCallstack(const size_t& skip_frames, const size_t& bytes_worth); //callstack_t* CallstackFetch( uint_t skip_frames );
	static void PrintCallStack(Callstack* cs, const size_t& currentIdx = 0, const size_t& numOfLeaks = 0, const bool& fireLeaksAsWarnings = true); //callstack_line_t* CallstackGetLines(callstack_t *cs);
	static void FreeCallstack(Callstack* cs);
	static const std::string ConvertTimeToStringForFiles(tm* timePtr);
};


struct debug_mem_t
{
public:
	static debug_mem_t* gAllocatedList;
	static size_t gAllocatedListCount;
	static size_t gAllocatedListAvailableID;

	debug_mem_t();

	static const int AddCallstackToBack(Callstack* cs);
	static const bool RemoveCallstackByID(const size_t& csID);
	static void ClearList();
	static void ClearList(debug_mem_t* listStart, const bool& clearCS = false);
	static void PrintAllCallstacks(const size_t& numOfLeaks, const bool& fireLeaksAsWarnings = true);
	static debug_mem_t* CopyWholeList();
	static std::vector<std::string> GetPrintOutsOfAllCurrentCallstackInfo();

	size_t size_callstack;
	Callstack* cs;
	size_t id;
	debug_mem_t *prev, *next;
};

//static void AllocateCallstack(); //Fetch Callstack
//Windows::CaptureStackBacktrace(stzip frames, int frameCount, void** array of addresses to fill, out hash);

#endif