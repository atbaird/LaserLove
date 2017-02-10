#include "Engine/Utils/Allocations.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Console/Console.hpp"
#include "Engine/Utils/Callstack.hpp"
#include "Engine/Utils/Thread.hpp"
//#include <string>

//template <typename T>
//T* Create()
//{
//	T* t = (T*)malloc(sizeof(T));
//	t = new(t) T(); //inplace new; uses given memory, and makes a T out of that memory; calls default constructor.
//	return t;
//}
int Allocations::gTotalBytesPresentlyAllocatedPerType[NUM_OF_ALLOCATION_TYPES];
int Allocations::gTotalBytesAllocatedPerTypeOverAllTime[NUM_OF_ALLOCATION_TYPES];
int Allocations::gNumberOfAllocationsAtStartup = 0;
int Allocations::gNumberOfAllocations = 0;
int Allocations::gTotalBytesAllocated = 0;
int Allocations::gMaxNumberOfBytesAllocatedEver = 0;
int Allocations::gNumberOfBytesAllocatedAndFreedInLastSecond = 0;
int Allocations::gAverageNumberOfBytesPerSecondDisplay = 0;
int Allocations::gFramesToSkip = 0; //1
float Allocations::gAverageNumberOfBytesPerSecond = 0.f;
float Allocations::gSecondsSinceLastUpdatedMemoryAllocations = 0.f;
bool Allocations::gStartTrackingAllocations = false;
bool Allocations::gMemoryWindowActive = false;

void Allocations::PrintOutAllocationsPerTimeForAllTime()
{
	if (MEMORY_TRACKING != 0)
	{
		DebuggerPrintf("--------------------------------------\n");
		DebuggerPrintf("Number of bytes allocated presently per type.\n");
		for (size_t i = 0; i < NUM_OF_ALLOCATION_TYPES; i++)
		{
			std::string message = "type " + std::to_string(i) + ": bytes = " + std::to_string(gTotalBytesPresentlyAllocatedPerType[i]) + "\n";
			DebuggerPrintf(message.c_str());
		}
		DebuggerPrintf("\nNumber of bytes allocated per type over all time.\n");
		for (size_t i = 0; i < NUM_OF_ALLOCATION_TYPES; i++)
		{
			std::string message = "type " + std::to_string(i) + ": bytes = " + std::to_string(gTotalBytesAllocatedPerTypeOverAllTime[i]) + "\n";
			DebuggerPrintf(message.c_str());
		}
		DebuggerPrintf("--------------------------------------\n");
	}
}
void Allocations::ThreadedSaveOutAllocationsToLogger()
{
	if (MEMORY_TRACKING != 0)
	{
		void* eh = nullptr;
		Thread* created = Thread::ThreadCreate(LoggerThreadEntry, eh);

		Thread::ThreadDetach(created);
	}
}
void Allocations::LoggerThreadEntry(void*)
{
	if (Logger::g_logger == nullptr)
	{
		return;
	}
	Logger::g_logger->LogPrintf("-------------------\nConsole log as of Logger Print call.", "console");
	for (size_t i = 0; i < Console::commandPromptLines.size(); i++)
	{
		Logger::g_logger->LogPrintf((Console::commandPromptLines.at(i).line + "\n").c_str(), "console");
	}
	Logger::g_logger->LogPrintf("\n-------------------", "console");

	Logger::g_logger->LogPrintf("\n--------------------\nMemoryLeaks", "general");
	std::vector<std::string> messages = debug_mem_t::GetPrintOutsOfAllCurrentCallstackInfo();
	for (size_t i = 0; i < messages.size(); i++)
	{
		Logger::g_logger->LogPrintf(messages.at(i).c_str(), "general");
	}
	Logger::g_logger->LogPrintf("\n--------------------", "general");
}
void Allocations::SaveOutAllocationsToLogger()
{
	if (MEMORY_TRACKING != 0)
	{
		LoggerThreadEntry(nullptr);
	}
}
#if defined(MEMORY_TRACKING) == false || MEMORY_TRACKING == 0
//No memory tracking

void Allocations::MemoryAnalyticsStartup()
{
	gStartTrackingAllocations = true;
	for (size_t i = 0; i < NUM_OF_ALLOCATION_TYPES; i++)
	{
		gTotalBytesPresentlyAllocatedPerType[i] = 0;
		gTotalBytesAllocatedPerTypeOverAllTime[i] = 0;
	}
}
void Allocations::MemoryAnalyticsUpdate(const float& deltaSeconds)
{
	deltaSeconds;
}
void Allocations::MemoryAnalyticsShutdown()
{
	gStartTrackingAllocations = false;
}

void Allocations::MemoryAnalyticsRender(const float& screenWidth, const float& screenHeight, const float& fontSize)
{
}

//------------------------------------------------------------------------------------
//new
void* operator new(size_t number_of_bytes)
{
	void* ptr = (void*)std::malloc(number_of_bytes);
	return ptr;
}

void* operator new[](size_t number_of_bytes)
{
	void* ptr = (void*)std::malloc(number_of_bytes);
	return ptr;
}

void* operator new(size_t number_of_bytes, const size_t& type)
{
	type;
	void* ptr = (void*)std::malloc(number_of_bytes);
	return ptr;
}
void* operator new[](size_t number_of_bytes, const size_t& type)
{
	type;
	void* ptr = (void*)std::malloc(number_of_bytes);
	return ptr;
}
//------------------------------------------------------------------------------------
//delete
void operator delete(void* ptr)
{
	free(ptr);
}

void operator delete[](void* ptr)
{
	free(ptr);
}

#elif MEMORY_TRACKING == 1
//basic memory tracking

void Allocations::MemoryAnalyticsStartup()
{
	gNumberOfAllocationsAtStartup = gNumberOfAllocations;
	gStartTrackingAllocations = true;
	for (size_t i = 0; i < NUM_OF_ALLOCATION_TYPES; i++)
	{
		gTotalBytesPresentlyAllocatedPerType[i] = 0;
		gTotalBytesAllocatedPerTypeOverAllTime[i] = 0;
	}
}
void Allocations::MemoryAnalyticsUpdate(const float& deltaSeconds)
{
	gSecondsSinceLastUpdatedMemoryAllocations += deltaSeconds;
	if (gSecondsSinceLastUpdatedMemoryAllocations >= 1.f)
	{
		gAverageNumberOfBytesPerSecondDisplay = gNumberOfBytesAllocatedAndFreedInLastSecond;
		gAverageNumberOfBytesPerSecond = (float)gNumberOfBytesAllocatedAndFreedInLastSecond / gSecondsSinceLastUpdatedMemoryAllocations;
		DebuggerPrintf("%.2f bytes were allocated or freed in the last second.\n", gAverageNumberOfBytesPerSecond);
		gSecondsSinceLastUpdatedMemoryAllocations = 0.f;
		gNumberOfBytesAllocatedAndFreedInLastSecond = 0;
	}
}
void Allocations::MemoryAnalyticsShutdown()
{
	if (gNumberOfAllocations > gNumberOfAllocationsAtStartup)
	{
		int numOfLeaks = gNumberOfAllocations - gNumberOfAllocationsAtStartup;
		ERROR_RECOVERABLE("MEMORY LEAK: Not all memory was unallocated at the end of the program!\n Number of leaks: " + std::to_string(numOfLeaks));
	}
	else
	{
		DebuggerPrintf("There were 0 memory leaks, good job.\n");
	}
	gStartTrackingAllocations = false;
}

void Allocations::MemoryAnalyticsRender(const float& screenWidth, const float& screenHeight, const float& fontSize)
{
	if (gMemoryWindowActive == true && g_Console != nullptr && g_Console->GetFont() != nullptr)
	{
		Font* font = g_Console->GetFont();
		font->RenderText(Vector3(0.f, screenHeight - (screenHeight * 0.056f), 0.f), "Number of Memory Allocations: " + std::to_string(gNumberOfAllocations), fontSize);
		font->RenderText(Vector3(0.f, screenHeight - (screenHeight * 0.111f), 0.f), "Total bytes Allocated: " + std::to_string(gTotalBytesAllocated), fontSize);
		font->RenderText(Vector3(0.f, screenHeight - (screenHeight * 0.167f), 0.f), "Highwater of bytes allocated: " + std::to_string(gMaxNumberOfBytesAllocatedEver), fontSize);
		font->RenderText(Vector3(0.f, screenHeight - (screenHeight * 0.222f), 0.f), "Average number of bytes allocated or freed in the last second: " + std::to_string(gAverageNumberOfBytesPerSecondDisplay), fontSize);
		font->RenderText(Vector3(0.f, screenHeight - (screenHeight * 0.278f), 0.f), "Average bytes freed or allocated per second: " + std::to_string(gAverageNumberOfBytesPerSecond), fontSize);
		font->RenderText(Vector3(0.f, screenHeight - (screenHeight * 0.333f), 0.f), "Seconds since last updated Memory Allocations: " + std::to_string(gSecondsSinceLastUpdatedMemoryAllocations), fontSize);
	}
}


//------------------------------------------------------------------------------------
//new
void* operator new(size_t number_of_bytes)
{
	size_t* ptr = (size_t*)std::malloc(number_of_bytes + (sizeof(size_t) * 2));
	DebuggerPrintf("Alloc %p of %u bytes. This is allocation number %i.\n", ptr, number_of_bytes, Allocations::gNumberOfAllocations);
	++Allocations::gNumberOfAllocations;
	Allocations::gTotalBytesAllocated += number_of_bytes;
	Allocations::gNumberOfBytesAllocatedAndFreedInLastSecond += number_of_bytes;
	if (Allocations::gTotalBytesAllocated > Allocations::gMaxNumberOfBytesAllocatedEver)
	{
		Allocations::gMaxNumberOfBytesAllocatedEver = Allocations::gTotalBytesAllocated;
	}
	Allocations::gTotalBytesPresentlyAllocatedPerType[GeneralAllocationType] = Allocations::gTotalBytesPresentlyAllocatedPerType[GeneralAllocationType] + number_of_bytes;
	Allocations::gTotalBytesAllocatedPerTypeOverAllTime[GeneralAllocationType] = Allocations::gTotalBytesAllocatedPerTypeOverAllTime[GeneralAllocationType] + number_of_bytes;
	*ptr = number_of_bytes;
	ptr++;
	*ptr = 0;
	ptr++;
	return ptr;
}
void* operator new[](size_t number_of_bytes)
{
	size_t* ptr = (size_t*)std::malloc(number_of_bytes + (sizeof(size_t) * 2));
	DebuggerPrintf("Alloc %p of %u bytes. This is allocation number %i.\n", ptr, number_of_bytes, Allocations::gNumberOfAllocations);
	++Allocations::gNumberOfAllocations;
	Allocations::gTotalBytesAllocated += number_of_bytes;
	Allocations::gNumberOfBytesAllocatedAndFreedInLastSecond += number_of_bytes;
	if (Allocations::gTotalBytesAllocated > Allocations::gMaxNumberOfBytesAllocatedEver)
	{
		Allocations::gMaxNumberOfBytesAllocatedEver = Allocations::gTotalBytesAllocated;
	}
	Allocations::gTotalBytesPresentlyAllocatedPerType[GeneralAllocationType] = Allocations::gTotalBytesPresentlyAllocatedPerType[GeneralAllocationType] + number_of_bytes;
	Allocations::gTotalBytesAllocatedPerTypeOverAllTime[GeneralAllocationType] = Allocations::gTotalBytesAllocatedPerTypeOverAllTime[GeneralAllocationType] + number_of_bytes;
	*ptr = number_of_bytes;
	ptr++;
	*ptr = 0;
	ptr++;
	return ptr;
}
void* operator new(size_t number_of_bytes, const size_t& type)
{
	size_t realType = type;
	if (type >= NUM_OF_ALLOCATION_TYPES)
	{
		realType = GeneralAllocationType;
	}
	size_t* ptr = (size_t*)std::malloc(number_of_bytes + (sizeof(size_t) * 2));
	DebuggerPrintf("Alloc %p of %u bytes. This is allocation number %i.\n", ptr, number_of_bytes, Allocations::gNumberOfAllocations);
	++Allocations::gNumberOfAllocations;
	Allocations::gTotalBytesAllocated += number_of_bytes;
	Allocations::gNumberOfBytesAllocatedAndFreedInLastSecond += number_of_bytes;
	if (Allocations::gTotalBytesAllocated > Allocations::gMaxNumberOfBytesAllocatedEver)
	{
		Allocations::gMaxNumberOfBytesAllocatedEver = Allocations::gTotalBytesAllocated;
	}
	Allocations::gTotalBytesPresentlyAllocatedPerType[realType] = Allocations::gTotalBytesPresentlyAllocatedPerType[realType] + number_of_bytes;
	Allocations::gTotalBytesAllocatedPerTypeOverAllTime[realType] = Allocations::gTotalBytesAllocatedPerTypeOverAllTime[realType] + number_of_bytes;
	*ptr = number_of_bytes;
	ptr++;
	*ptr = realType;
	ptr++;
	return ptr;
}
void* operator new[](size_t number_of_bytes, const size_t& type)
{
	size_t realType = type;
	if (type >= NUM_OF_ALLOCATION_TYPES)
	{
		realType = GeneralAllocationType;
	}
	size_t* ptr = (size_t*)std::malloc(number_of_bytes + (sizeof(size_t) * 2));
	DebuggerPrintf("Alloc %p of %u bytes. This is allocation number %i.\n", ptr, number_of_bytes, Allocations::gNumberOfAllocations);
	++Allocations::gNumberOfAllocations;
	Allocations::gTotalBytesAllocated += number_of_bytes;
	Allocations::gNumberOfBytesAllocatedAndFreedInLastSecond += number_of_bytes;
	if (Allocations::gTotalBytesAllocated > Allocations::gMaxNumberOfBytesAllocatedEver)
	{
		Allocations::gMaxNumberOfBytesAllocatedEver = Allocations::gTotalBytesAllocated;
	}
	Allocations::gTotalBytesPresentlyAllocatedPerType[realType] = Allocations::gTotalBytesPresentlyAllocatedPerType[realType] + number_of_bytes;
	Allocations::gTotalBytesAllocatedPerTypeOverAllTime[realType] = Allocations::gTotalBytesAllocatedPerTypeOverAllTime[realType] + number_of_bytes;
	*ptr = number_of_bytes;
	ptr++;
	*ptr = realType;
	ptr++;
	return ptr;
}
//------------------------------------------------------------------------------------
//delete
void operator delete(void* ptr)
{
	size_t* ptr_size = (size_t*)ptr;
	--ptr_size;
	size_t allocType = *ptr_size;
	--ptr_size;
	size_t number_of_bytes = *ptr_size;

	--Allocations::gNumberOfAllocations;
	Allocations::gTotalBytesAllocated -= number_of_bytes;
	Allocations::gNumberOfBytesAllocatedAndFreedInLastSecond += number_of_bytes;
	Allocations::gTotalBytesPresentlyAllocatedPerType[allocType] = Allocations::gTotalBytesPresentlyAllocatedPerType[allocType] - number_of_bytes;
	free(ptr_size);
}
void operator delete[](void* ptr)
{
	size_t* ptr_size = (size_t*)ptr;
	--ptr_size;
	size_t allocType = *ptr_size;
	--ptr_size;
	size_t number_of_bytes = *ptr_size;

	--Allocations::gNumberOfAllocations;
	Allocations::gTotalBytesAllocated -= number_of_bytes;
	Allocations::gNumberOfBytesAllocatedAndFreedInLastSecond += number_of_bytes;
	Allocations::gTotalBytesPresentlyAllocatedPerType[allocType] = Allocations::gTotalBytesPresentlyAllocatedPerType[allocType] - number_of_bytes;
	free(ptr_size);
}

#else
//Robust memory tracking


void Allocations::MemoryAnalyticsStartup()
{
	Callstack::CallstackSystemInit();
	gNumberOfAllocationsAtStartup = gNumberOfAllocations;
	gStartTrackingAllocations = true;
	for (size_t i = 0; i < NUM_OF_ALLOCATION_TYPES; i++)
	{
		gTotalBytesPresentlyAllocatedPerType[i] = 0;
		gTotalBytesAllocatedPerTypeOverAllTime[i] = 0;
	}
}
void Allocations::MemoryAnalyticsUpdate(const float& deltaSeconds)
{
	gSecondsSinceLastUpdatedMemoryAllocations += deltaSeconds;
	if (gSecondsSinceLastUpdatedMemoryAllocations >= 1.f)
	{
		gAverageNumberOfBytesPerSecondDisplay = gNumberOfBytesAllocatedAndFreedInLastSecond;
		gAverageNumberOfBytesPerSecond = (float)gNumberOfBytesAllocatedAndFreedInLastSecond / gSecondsSinceLastUpdatedMemoryAllocations;
		DebuggerPrintf("%.2f bytes were allocated or freed in the last second.\n", gAverageNumberOfBytesPerSecond);
		gSecondsSinceLastUpdatedMemoryAllocations = 0.f;
		gNumberOfBytesAllocatedAndFreedInLastSecond = 0;
	}
}
void Allocations::MemoryAnalyticsShutdown()
{
	//For all leaks
	// print call stack.

	if ((debug_mem_t::gAllocatedListCount) != 0)
	{
		int realNumOfAllocations = Allocations::gNumberOfAllocations;
		int numOfLeaks = realNumOfAllocations - Allocations::gNumberOfAllocationsAtStartup;
		ERROR_RECOVERABLE("MEMORY LEAKS DETECTED: Not all memory was unallocated at the end of the program!\n Number of leaks: " + std::to_string(numOfLeaks));
		debug_mem_t::PrintAllCallstacks(numOfLeaks);
	}
	gStartTrackingAllocations = false;
	//Allocations::SaveOutAllocationsToLogger();
	Callstack::CallstackSystemDeinit();
	debug_mem_t::ClearList();
}

void Allocations::MemoryAnalyticsRender(const float& screenWidth, const float& screenHeight, const float& fontSize)
{
	if (gMemoryWindowActive == true && g_Console != nullptr && g_Console->GetFont() != nullptr)
	{
		Font* font = g_Console->GetFont();
		font->RenderText(Vector3(0.f, screenHeight - (screenHeight * 0.056f), 0.f), "Number of Memory Allocations: " + std::to_string(gNumberOfAllocations), fontSize);
		font->RenderText(Vector3(0.f, screenHeight - (screenHeight * 0.111f), 0.f), "Total bytes Allocated: " + std::to_string(gTotalBytesAllocated), fontSize);
		font->RenderText(Vector3(0.f, screenHeight - (screenHeight * 0.167f), 0.f), "Highwater of bytes allocated: " + std::to_string(gMaxNumberOfBytesAllocatedEver), fontSize);
		font->RenderText(Vector3(0.f, screenHeight - (screenHeight * 0.222f), 0.f), "Average number of bytes allocated or freed in the last second: " + std::to_string(gAverageNumberOfBytesPerSecondDisplay), fontSize);
		font->RenderText(Vector3(0.f, screenHeight - (screenHeight * 0.278f), 0.f), "Average bytes freed or allocated per second: " + std::to_string(gAverageNumberOfBytesPerSecond), fontSize);
		font->RenderText(Vector3(0.f, screenHeight - (screenHeight * 0.333f), 0.f), "Seconds since last updated Memory Allocations: " + std::to_string(gSecondsSinceLastUpdatedMemoryAllocations), fontSize);
	}
}

//------------------------------------------------------------------------------------
//new
void* operator new(size_t number_of_bytes)
{
	int index = -1;
	size_t* ptr = (size_t*)std::malloc(number_of_bytes + (sizeof(size_t) * 2) + sizeof(int));
	//DebuggerPrintf("Alloc %p of %u bytes. This is allocation number %i.\n", ptr, number_of_bytes, Allocations::gNumberOfAllocations);
	++Allocations::gNumberOfAllocations;
	Allocations::gTotalBytesAllocated += number_of_bytes;
	Allocations::gNumberOfBytesAllocatedAndFreedInLastSecond += number_of_bytes;
	if (Allocations::gTotalBytesAllocated > Allocations::gMaxNumberOfBytesAllocatedEver)
	{
		Allocations::gMaxNumberOfBytesAllocatedEver = Allocations::gTotalBytesAllocated;
	}

	if (Allocations::gStartTrackingAllocations)
	{
		Callstack* cs = Callstack::CreateCallstack(Allocations::gFramesToSkip, number_of_bytes);
		index = debug_mem_t::AddCallstackToBack(cs);
	}
	Allocations::gTotalBytesPresentlyAllocatedPerType[GeneralAllocationType] = Allocations::gTotalBytesPresentlyAllocatedPerType[GeneralAllocationType] + number_of_bytes;
	Allocations::gTotalBytesAllocatedPerTypeOverAllTime[GeneralAllocationType] = Allocations::gTotalBytesAllocatedPerTypeOverAllTime[GeneralAllocationType] + number_of_bytes;

	*ptr = number_of_bytes;
	ptr++;
	*ptr = GeneralAllocationType;
	ptr++;
	int* ptrI = (int*)ptr;
	*ptrI = index;
	ptrI++;
	return ptrI;
}
void* operator new[](size_t number_of_bytes)
{
	int index = -1;
	size_t* ptr = (size_t*)std::malloc(number_of_bytes + (sizeof(size_t) * 2) + sizeof(int));
	//DebuggerPrintf("Alloc %p of %u bytes. This is allocation number %i.\n", ptr, number_of_bytes, Allocations::gNumberOfAllocations);
	++Allocations::gNumberOfAllocations;
	Allocations::gTotalBytesAllocated += number_of_bytes;
	Allocations::gNumberOfBytesAllocatedAndFreedInLastSecond += number_of_bytes;
	if (Allocations::gTotalBytesAllocated > Allocations::gMaxNumberOfBytesAllocatedEver)
	{
		Allocations::gMaxNumberOfBytesAllocatedEver = Allocations::gTotalBytesAllocated;
	}

	if (Allocations::gStartTrackingAllocations)
	{
		Callstack* cs = Callstack::CreateCallstack(Allocations::gFramesToSkip, number_of_bytes);
		index = debug_mem_t::AddCallstackToBack(cs);
	}
	Allocations::gTotalBytesPresentlyAllocatedPerType[GeneralAllocationType] = Allocations::gTotalBytesPresentlyAllocatedPerType[GeneralAllocationType] + number_of_bytes;
	Allocations::gTotalBytesAllocatedPerTypeOverAllTime[GeneralAllocationType] = Allocations::gTotalBytesAllocatedPerTypeOverAllTime[GeneralAllocationType] + number_of_bytes;

	*ptr = number_of_bytes;
	ptr++;
	*ptr = GeneralAllocationType;
	ptr++;
	int* ptrI = (int*)ptr;
	*ptrI = index;
	ptrI++;
	return ptrI;
}

void* operator new(size_t number_of_bytes, const size_t& type)
{
	size_t realType = type;
	if (type >= NUM_OF_ALLOCATION_TYPES)
	{
		realType = GeneralAllocationType;
	}
	int index = -1;
	size_t* ptr = (size_t*)std::malloc(number_of_bytes + (sizeof(size_t) * 2) + sizeof(int));
	//DebuggerPrintf("Alloc %p of %u bytes. This is allocation number %i.\n", ptr, number_of_bytes, Allocations::gNumberOfAllocations);
	++Allocations::gNumberOfAllocations;
	Allocations::gTotalBytesAllocated += number_of_bytes;
	Allocations::gNumberOfBytesAllocatedAndFreedInLastSecond += number_of_bytes;
	if (Allocations::gTotalBytesAllocated > Allocations::gMaxNumberOfBytesAllocatedEver)
	{
		Allocations::gMaxNumberOfBytesAllocatedEver = Allocations::gTotalBytesAllocated;
	}

	if (Allocations::gStartTrackingAllocations)
	{
		Callstack* cs = Callstack::CreateCallstack(Allocations::gFramesToSkip, number_of_bytes);
		index = debug_mem_t::AddCallstackToBack(cs);
	}
	Allocations::gTotalBytesPresentlyAllocatedPerType[realType] = Allocations::gTotalBytesPresentlyAllocatedPerType[realType] + number_of_bytes;
	Allocations::gTotalBytesAllocatedPerTypeOverAllTime[realType] = Allocations::gTotalBytesAllocatedPerTypeOverAllTime[realType] + number_of_bytes;

	*ptr = number_of_bytes;
	ptr++;
	*ptr = realType;
	ptr++;
	int* ptrI = (int*)ptr;
	*ptrI = index;
	ptrI++;
	return ptrI;
}
void* operator new[](size_t number_of_bytes, const size_t& type)
{
	size_t realType = type;
	if (type >= NUM_OF_ALLOCATION_TYPES)
	{
		realType = GeneralAllocationType;
	}
	int index = -1;
	size_t* ptr = (size_t*)std::malloc(number_of_bytes + (sizeof(size_t) * 2) + sizeof(int));
	//DebuggerPrintf("Alloc %p of %u bytes. This is allocation number %i.\n", ptr, number_of_bytes, Allocations::gNumberOfAllocations);
	++Allocations::gNumberOfAllocations;
	Allocations::gTotalBytesAllocated += number_of_bytes;
	Allocations::gNumberOfBytesAllocatedAndFreedInLastSecond += number_of_bytes;
	if (Allocations::gTotalBytesAllocated > Allocations::gMaxNumberOfBytesAllocatedEver)
	{
		Allocations::gMaxNumberOfBytesAllocatedEver = Allocations::gTotalBytesAllocated;
	}

	if (Allocations::gStartTrackingAllocations)
	{
		Callstack* cs = Callstack::CreateCallstack(Allocations::gFramesToSkip, number_of_bytes);
		index = debug_mem_t::AddCallstackToBack(cs);
	}
	Allocations::gTotalBytesPresentlyAllocatedPerType[realType] = Allocations::gTotalBytesPresentlyAllocatedPerType[realType] + number_of_bytes;
	Allocations::gTotalBytesAllocatedPerTypeOverAllTime[realType] = Allocations::gTotalBytesAllocatedPerTypeOverAllTime[realType] + number_of_bytes;

	*ptr = number_of_bytes;
	ptr++;
	*ptr = realType;
	ptr++;
	int* ptrI = (int*)ptr;
	*ptrI = index;
	ptrI++;
	return ptrI;
}
//------------------------------------------------------------------------------------
//delete
void operator delete(void* ptr)
{
	if (ptr == nullptr)
	{
		return;
	}
	int* ptr_index = (int*) ptr;
	--ptr_index;
	int index = *ptr_index;
	size_t* ptr_size = (size_t*)ptr_index;
	--ptr_size;
	size_t type = *ptr_size;
	--ptr_size;
	size_t number_of_bytes = *ptr_size;
	--Allocations::gNumberOfAllocations;
	Allocations::gTotalBytesAllocated -= number_of_bytes;
	Allocations::gNumberOfBytesAllocatedAndFreedInLastSecond += number_of_bytes;
	Allocations::gTotalBytesPresentlyAllocatedPerType[type] = Allocations::gTotalBytesPresentlyAllocatedPerType[type] - number_of_bytes;
	if (Allocations::gStartTrackingAllocations && index != -1)
	{
		debug_mem_t::RemoveCallstackByID(index);
	}
	free(ptr_size);
}
void operator delete[](void* ptr)
{
	int* ptr_index = (int*)ptr;
	--ptr_index;
	int index = *ptr_index;
	size_t* ptr_size = (size_t*)ptr_index;
	--ptr_size;
	size_t type = *ptr_size;
	--ptr_size;
	size_t number_of_bytes = *ptr_size;
	--Allocations::gNumberOfAllocations;
	Allocations::gTotalBytesAllocated -= number_of_bytes;
	Allocations::gNumberOfBytesAllocatedAndFreedInLastSecond += number_of_bytes;
	Allocations::gTotalBytesPresentlyAllocatedPerType[type] = Allocations::gTotalBytesPresentlyAllocatedPerType[type] - number_of_bytes;
	if (Allocations::gStartTrackingAllocations && index != -1)
	{
		debug_mem_t::RemoveCallstackByID(index);
	}
	free(ptr_size);
}

#endif 

//void* operator new(size_t size, void* ptr)
//{
//	return ptr;
//}