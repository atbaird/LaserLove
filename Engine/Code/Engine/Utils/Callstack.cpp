#include "Engine/Utils/Callstack.hpp"
#include "Engine/Console/Console.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/StringUtils.hpp"
#include <string>
#define WIN32_LEAN_AND_MEAN
#define _WINSOCKAPI_
#include <Windows.h>
#include <DbgHelp.h>


#define MAX_SYMBOL_NAME_LENGTH 128
#define MAX_FILENAME_LENGTH 1024
#define MAX_DEPTH 128
typedef BOOL(__stdcall *sym_initialize_t)(IN HANDLE hProcess, IN PSTR UserSearchPath, IN BOOL fInvadeProcess);
typedef BOOL(__stdcall *sym_cleanup_t)(IN HANDLE hProcess);
typedef BOOL(__stdcall *sym_from_addr_t)(IN HANDLE hProcess, IN DWORD64 Address, OUT PDWORD64 Displacement, OUT PSYMBOL_INFO Symbol);
typedef BOOL(__stdcall *sym_get_line_t)(IN HANDLE hProcess, IN DWORD64 dwAddr, OUT PDWORD pdwDisplacement, OUT PIMAGEHLP_LINE64 Symbol);

static HMODULE gDebugHelp;
static HANDLE gProcess;
static SYMBOL_INFO  *gSymbol;
static int gCallstackCount = 0;

// only called from single thread - so can use a shared buffer
static char gFileName[MAX_FILENAME_LENGTH];
static callstack_line_t gCallstackBuffer[MAX_DEPTH];

static sym_initialize_t LSymInitialize;
static sym_cleanup_t LSymCleanup;
static sym_from_addr_t LSymFromAddr;
static sym_get_line_t LSymGetLineFromAddr64;

Callstack* Callstack::gCallstack = nullptr;

debug_mem_t* debug_mem_t::gAllocatedList = nullptr;
size_t debug_mem_t::gAllocatedListCount = 0;
size_t debug_mem_t::gAllocatedListAvailableID = 0;


callstack_line_t::callstack_line_t()
	: line(0),
	offset(0)
{
	for (size_t i = 0; i < 128; i++)
	{
		filename[i] = '\0';
		function_name[i] = '\0';
	}
}

const bool Callstack::CallstackSystemInit()
{

	gDebugHelp = LoadLibraryA("dbghelp.dll");
	if (gDebugHelp == nullptr)
	{
		ERROR_AND_DIE("dbghelp.dll is NULL!");
	}
	LSymInitialize = (sym_initialize_t)GetProcAddress(gDebugHelp, "SymInitialize");
	LSymCleanup = (sym_cleanup_t)GetProcAddress(gDebugHelp, "SymCleanup");
	LSymFromAddr = (sym_from_addr_t)GetProcAddress(gDebugHelp, "SymFromAddr");
	LSymGetLineFromAddr64 = (sym_get_line_t)GetProcAddress(gDebugHelp, "SymGetLineFromAddr64");

	gProcess = GetCurrentProcess();
	LSymInitialize(gProcess, NULL, TRUE);

	gSymbol = (SYMBOL_INFO *)malloc(sizeof(SYMBOL_INFO) + (MAX_FILENAME_LENGTH * sizeof(char)));
	gSymbol->MaxNameLen = MAX_FILENAME_LENGTH;
	gSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);

	for (size_t i = 0; i < MAX_DEPTH; i++)
	{
		gCallstackBuffer[i] = callstack_line_t();
	}

	return true;
}

void Callstack::CallstackSystemDeinit()
{
	LSymCleanup(gProcess);

	FreeLibrary(gDebugHelp);
	gDebugHelp = NULL;

	if (gCallstackCount != 0) {
		gCallstackCount = 0;
	}
}

Callstack* Callstack::CreateCallstack(const size_t& skip_frames, const size_t& bytes_worth) //callstack_t* CallstackFetch( uint_t skip_frames );
{
	void *stack[MAX_DEPTH];
	uint32_t frames = CaptureStackBackTrace(1 + skip_frames, MAX_DEPTH, stack, NULL);

	gCallstackCount++;

	size_t size = sizeof(Callstack) + sizeof(size_t) + (sizeof(void*) * frames);
	void *buf_data = malloc(size);


	time_t  timev;
	time(&timev);
	tm* timePtr = gmtime(&timev);
	localtime_s(timePtr, &timev);
	Callstack *cs = (Callstack*)buf_data;
	cs->frames = (void**)(cs+1);
	cs->frame_count = frames;
	cs->bytes_worth = bytes_worth;
	cs->timeCreated = timePtr;
	memcpy(cs->frames, stack, sizeof(void*) * frames); //memcpy overwrites frame_count. :|

	return cs;
}
void Callstack::FreeCallstack(Callstack* cs)
{
	gCallstackCount--;
	//free(cs->frames);
	free(cs);
}
void Callstack::PrintCallStack(Callstack* cs, const size_t& currentIdx, const size_t& numOfLeaks, const bool& fireLeaksAsWarnings) //callstack_line_t* CallstackGetLines(callstack_t *cs);
{
	if (cs == nullptr)
	{
		return;
	}
	{
		IMAGEHLP_LINE64 LineInfo;
		DWORD LineDisplacement = 0; // Displacement from the beginning of the line 
		LineInfo.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

		uint32_t count = cs->frame_count;
		for (uint32_t i = 0; i < count; ++i) {
			callstack_line_t *line = &(gCallstackBuffer[i]);
			DWORD64 ptr = (DWORD64)(cs->frames[i]);
			LSymFromAddr(gProcess, ptr, 0, gSymbol);

			StringCopy((line->function_name), 128, gSymbol->Name);

			BOOL bRet = LSymGetLineFromAddr64(
				GetCurrentProcess(), // Process handle of the current process 
				ptr, // Address 
				&LineDisplacement, // Displacement will be stored here by the function 
				&LineInfo);         // File name / line information will be stored here 

			if (bRet) {
				line->line = LineInfo.LineNumber;

				std::string fileNameStr = std::string(LineInfo.FileName);
				char const *filename = fileNameStr.substr(fileNameStr.find_last_of("\\src")).c_str();//StringFindLast(LineInfo.FileName, "\\src");
				if (filename == NULL) {
					filename = LineInfo.FileName;
				}
				else {
					filename += 5; // skip to the important bit - so that it can be double clicked in Output
				}


				StringCopy(line->filename, 128, filename);
				line->offset = LineDisplacement;
			}
			else {
				line->line = 0;
				line->offset = 0;
				StringCopy(line->filename, 128, "N/A");
			}
			//std::string message = "file: " + std::string(LineInfo.FileName) + " ,\nfunction_name: " + std::string(line->function_name) + " ,\nline: " + std::to_string(line->line);
			if (fireLeaksAsWarnings)
			{
				RecoverableWarningNoPopUp(LineInfo.FileName, line->function_name, line->line, "Callstack " + std::to_string(currentIdx) + " of " + std::to_string(numOfLeaks) + ". Callstack line " + std::to_string(i + 1) + " of " + std::to_string(count)
					+ "\n" + "file: " + std::string(LineInfo.FileName) + " ,\nfunction_name: " + std::string(line->function_name) + " ,\nline: " + std::to_string(line->line)
					+ "\nBytes lost: " + std::to_string(cs->bytes_worth));
			}
		}
	}
}


debug_mem_t::debug_mem_t()
	: prev(nullptr),
	next(nullptr),
	size_callstack(0),
	id(0),
	cs(nullptr)
{
}
const int debug_mem_t::AddCallstackToBack(Callstack* cs)
{
	debug_mem_t* current = (debug_mem_t*)malloc(sizeof(debug_mem_t));
	current->cs = cs;
	current->size_callstack = sizeof(cs);
	gAllocatedListAvailableID++;
	current->id = gAllocatedListAvailableID;
	if (gAllocatedList == nullptr)
	{
		gAllocatedList = current;
		gAllocatedList->prev = gAllocatedList;
		gAllocatedList->next = gAllocatedList;
	}
	else
	{
		debug_mem_t* next = gAllocatedList;
		debug_mem_t* prev = next->prev;
		gAllocatedList->prev = nullptr;
		prev->next = nullptr;
		next->prev = nullptr;
		prev->next = nullptr;
		current->next = next;
		current->prev = prev;
		next->prev = current;
		prev->next = current;
	}
	gAllocatedListCount++;
	return current->id;
}
const bool debug_mem_t::RemoveCallstackByID(const size_t& csID)
{
	if (gAllocatedList == nullptr)
	{
		return false;
	}
	if (gAllocatedList->id == csID)
	{
		debug_mem_t* prev = gAllocatedList->prev;
		debug_mem_t* next = gAllocatedList->next;
		if (prev == gAllocatedList)
		{
			prev = nullptr;
		}
		if (next == gAllocatedList)
		{
			next = nullptr;
		}
		Callstack::FreeCallstack(gAllocatedList->cs);
		free(gAllocatedList);
		gAllocatedList = nullptr;
		if (next != nullptr)
		{
			gAllocatedList = next;
			next->prev = nullptr;
			prev->next = nullptr;
			next->prev = prev;
			prev->next = next;
		}
		gAllocatedListCount--;

		return true;
	}
	debug_mem_t* cur = gAllocatedList->next;

	while (cur != gAllocatedList)
	{
		debug_mem_t* next = cur->next;
		if (cur->id == csID)
		{
			debug_mem_t* prev = cur->prev;
			Callstack::FreeCallstack(cur->cs);
			free(cur);
			prev->next = nullptr;
			next->prev = nullptr;
			next->prev = prev;
			prev->next = next;

			gAllocatedListCount--;
			return true;
		}
		cur = nullptr;
		cur = next;
	}
	return false;
}
void debug_mem_t::ClearList()
{
	//ClearList(gAllocatedList, true);
	if (gAllocatedList == nullptr)
	{
		return;
	}
	debug_mem_t* cur = gAllocatedList->next;
	while (cur != gAllocatedList)
	{
		debug_mem_t* prev = cur->prev;
		debug_mem_t* next = cur->next;
		Callstack::FreeCallstack(cur->cs);
		free(cur);
		prev->next = nullptr;
		next->prev = nullptr;
		next->prev = prev;
		prev->next = next;
		cur = nullptr;
		cur = next;
	}
	
	Callstack::FreeCallstack(gAllocatedList->cs);
	free(gAllocatedList);
	gAllocatedList = nullptr;
	
	gAllocatedListCount = 0;
}
void debug_mem_t::ClearList(debug_mem_t* listStart, const bool& clearCS)
{

	debug_mem_t* cur = listStart->next;
	while (cur != listStart)
	{
		debug_mem_t* prev = cur->prev;
		debug_mem_t* next = cur->next;
		if (clearCS)
		{
			Callstack::FreeCallstack(cur->cs);
		}
		free(cur);
		prev->next = nullptr;
		next->prev = nullptr;
		next->prev = prev;
		prev->next = next;
		cur = nullptr;
		cur = next;
	}
	if (clearCS)
	{
		Callstack::FreeCallstack(listStart->cs);
	}
	free(listStart);
	listStart = nullptr;

	gAllocatedListCount = 0;
}
void debug_mem_t::PrintAllCallstacks(const size_t& numOfLeaks, const bool& fireLeaksAsWarnings)
{
	size_t size = gAllocatedListCount;
	if (gAllocatedList != nullptr)
	{
		bool passedOnce = false;
		debug_mem_t* cur = gAllocatedList;
		size_t idx = 1;
		for (size_t i = 0; i < size; i++)
		{
			Callstack::PrintCallStack(cur->cs, idx, numOfLeaks, fireLeaksAsWarnings);

			debug_mem_t* next = cur->next;
			cur = nullptr;
			cur = next;

			passedOnce = true;
			idx++;
		}
		//while ((passedOnce == false) || (list != cur))
		//{
		//	Callstack::PrintCallStack(cur->cs, idx, numOfLeaks, fireLeaksAsWarnings);
		//
		//	debug_mem_t* next = cur->next;
		//	cur = nullptr;
		//	cur = next;
		//
		//	passedOnce = true;
		//	idx++;
		//}
	}
}
debug_mem_t* debug_mem_t::CopyWholeList()
{
	if (gAllocatedListCount == 0 || gAllocatedList == nullptr)
	{
		return nullptr;
	}
	const size_t sizeOfNode = sizeof(debug_mem_t);
	debug_mem_t* copyListCurrent = (debug_mem_t*)malloc(sizeOfNode * gAllocatedListCount);
	debug_mem_t* copyGlobal = copyListCurrent;

	debug_mem_t* cur = gAllocatedList;
	for (size_t i = 0; i < gAllocatedListCount; i++)
	{
		/*
		void *stack[MAX_DEPTH];
	uint32_t frames = CaptureStackBackTrace(1 + skip_frames, MAX_DEPTH, stack, NULL);

	gCallstackCount++;

	size_t size = sizeof(Callstack) + sizeof(size_t) + (sizeof(void*) * frames);
	void *buf_data = malloc(size);

	Callstack *cs = (Callstack*)buf_data;
	cs->frames = (void**)(cs+1);
	cs->frame_count = frames;
	cs->bytes_worth = bytes_worth;
	memcpy(cs->frames, stack, sizeof(void*) * frames); //memcpy overwrites frame_count. :|

	return cs;
		*/
		/*

		debug_mem_t* current = (debug_mem_t*)malloc(sizeof(debug_mem_t));
		current->cs = cs;
		current->size_callstack = sizeof(cs);
		*/

		//sizeof(Callstack) + sizeof(size_t) + (sizeof(void*) * frames);
		debug_mem_t* nextSlot = copyListCurrent;
		debug_mem_t* prevSlot = copyListCurrent;
		nextSlot++;
		prevSlot--;
		copyListCurrent->size_callstack = cur->size_callstack;
		copyListCurrent->cs = cur->cs;
		if (i != 0)
		{
			copyListCurrent->prev = prevSlot;
		}
		else
		{
			copyListCurrent->prev = (copyListCurrent + ((gAllocatedListCount - 1)));
		}

		if (i != gAllocatedListCount - 1)
		{
			copyListCurrent->next = nextSlot;
		}
		else
		{
			copyListCurrent->next = copyGlobal;
		}
		copyListCurrent++;
		debug_mem_t* temp = cur->next;
		cur = nullptr;
		cur = temp;
	}

	//gAllocatedListCount

	return copyGlobal;
}

std::vector<std::string> debug_mem_t::GetPrintOutsOfAllCurrentCallstackInfo()
{
	std::vector<std::string> messages;
	if (gAllocatedList == nullptr)
	{
		return messages;
	}
	debug_mem_t* cur = gAllocatedList;
	int currentCallStackCount = gAllocatedListCount;
	for (int callstackIdx = 0; callstackIdx < currentCallStackCount; callstackIdx++)
	{
		IMAGEHLP_LINE64 LineInfo;
		DWORD LineDisplacement = 0; // Displacement from the beginning of the line 
		LineInfo.SizeOfStruct = sizeof(IMAGEHLP_LINE64);

		uint32_t count = cur->cs->frame_count;
		for (uint32_t frameIdx = 0; frameIdx < count; ++frameIdx) {
			callstack_line_t *line = &(gCallstackBuffer[frameIdx]);
			DWORD64 ptr = (DWORD64)(cur->cs->frames[frameIdx]);
			LSymFromAddr(gProcess, ptr, 0, gSymbol);

			StringCopy((line->function_name), 128, gSymbol->Name);

			BOOL bRet = LSymGetLineFromAddr64(
				GetCurrentProcess(), // Process handle of the current process 
				ptr, // Address 
				&LineDisplacement, // Displacement will be stored here by the function 
				&LineInfo);         // File name / line information will be stored here 

			if (bRet) {
				line->line = LineInfo.LineNumber;

				std::string fileNameStr = std::string(LineInfo.FileName);
				char const *filename = fileNameStr.substr(fileNameStr.find_last_of("\\src")).c_str();//StringFindLast(LineInfo.FileName, "\\src");
				if (filename == NULL) {
					filename = LineInfo.FileName;
				}
				else {
					filename += 5; // skip to the important bit - so that it can be double clicked in Output
				}


				StringCopy(line->filename, 128, filename);
				line->offset = LineDisplacement;
			}
			else {
				line->line = 0;
				line->offset = 0;
				StringCopy(line->filename, 128, "N/A");
			}



			std::string message = Callstack::ConvertTimeToStringForFiles(cur->cs->timeCreated)
				+ std::string(LineInfo.FileName) + " " +  std::string(line->function_name)
				+ " " + std::to_string(line->line) + "\nCallstack " + std::to_string(frameIdx) +  "of" 
				+ std::to_string(currentCallStackCount) + ". Callstack line " + std::to_string(frameIdx + 1)
				+ " of " + std::to_string(count) + "\n" + "file: " + std::string(LineInfo.FileName) + " ,\nfunction_name: " 
				+ std::string(line->function_name) + " ,\nline: " + std::to_string(line->line) +
				"\nBytes lost: " + std::to_string(cur->cs->bytes_worth) + "\n" + '\0';
			std::string copy;
			copy.resize(message.length() + 1);
			copy = message;
			messages.push_back(copy);
		}
	}
	return messages;
}
const std::string Callstack::ConvertTimeToStringForFiles(tm* timePtr)
{
	std::string zero = std::to_string(0);
	int year = timePtr->tm_year + 1900;
	int month = timePtr->tm_mon + 1;
	int day = timePtr->tm_mday;
	int hour = timePtr->tm_hour;
	int min = timePtr->tm_min;
	int sec = timePtr->tm_sec;
	std::string fullStr = std::to_string(year);
	if (month < 10)
	{
		fullStr += zero + std::to_string(month);
	}
	else
	{
		fullStr += std::to_string(month);
	}
	if (day < 10)
	{
		fullStr += zero + std::to_string(day);
	}
	else
	{
		fullStr += std::to_string(day);
	}
	fullStr += "_";
	if (hour < 10)
	{
		fullStr += zero + std::to_string(hour);
	}
	else
	{
		fullStr += std::to_string(hour);
	}
	if (min < 10)
	{
		fullStr += zero + std::to_string(min);
	}
	else
	{
		fullStr += std::to_string(min);
	}
	if (sec < 10)
	{
		fullStr += zero + std::to_string(sec);
	}
	else
	{
		fullStr += std::to_string(sec);
	}
	return fullStr;
}
