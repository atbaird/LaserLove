#pragma once

#ifndef ALLOCATIONS_HPP
#define ALLOCATIONS_HPP
#include <cstdlib>
#include "Engine/Utils/Callstack.hpp"
#include "Engine/Console/RegisterCommandHelper.hpp"
#include "Engine/Utils/Logger.hpp"


//template <typename T>
//T* Create();


#define MEMORY_TRACKING 0

enum AllocationTypes
{
	GeneralAllocationType = 0,
	PhysicsAllocationType,
	NUM_OF_ALLOCATION_TYPES
};

class Allocations
{
public:
	static int gTotalBytesPresentlyAllocatedPerType[];
	static int gTotalBytesAllocatedPerTypeOverAllTime[];
	static int gNumberOfAllocationsAtStartup;
	static int gNumberOfAllocations;
	static int gTotalBytesAllocated;
	static int gMaxNumberOfBytesAllocatedEver;
	static int gNumberOfBytesAllocatedAndFreedInLastSecond;
	static int gAverageNumberOfBytesPerSecondDisplay;
	static int gFramesToSkip;
	static float gAverageNumberOfBytesPerSecond;
	static float gSecondsSinceLastUpdatedMemoryAllocations;
	static bool gStartTrackingAllocations;
	static bool gMemoryWindowActive;
	static void MemoryAnalyticsStartup();
	static void MemoryAnalyticsUpdate(const float& deltaSeconds);
	static void MemoryAnalyticsShutdown();
	static void MemoryAnalyticsRender(const float& screenWidth = 16.f, const float& screenHeight = 9.f, const float& fontSize = 0.005f);
	static void PrintOutAllocationsPerTimeForAllTime();
	static void ThreadedSaveOutAllocationsToLogger();
	static void SaveOutAllocationsToLogger();

	static void LoggerThreadEntry(void*);
};



//void* operator new(...) can only exist once in the ENTIRE project. Same goes for the other variants.

//new
void* operator new(size_t number_of_bytes);
void* operator new[](size_t number_of_bytes);
void* operator new(size_t size, const size_t& type);
void* operator new[](size_t size, const size_t& type);

//delete
void operator delete(void* ptr);
void operator delete[](void* ptr);

#endif