#include "Engine/Utils/AllocationsConsoleCommands.hpp"
#include "Engine/Utils/Callstack.hpp"
#include "Engine/Console/Console.hpp"

void AllocationsConsoleCommands::ToggleMemoryDebugInformation(Command&)
{
	int val = MEMORY_TRACKING;
	if (val != 0)
	{
		bool newActive = !Allocations::gMemoryWindowActive;
		Allocations::gMemoryWindowActive = newActive;

		if (newActive == true)
		{
			g_Console->ConsolePrint("Memory Window Active");
		}
		else
		{
			g_Console->ConsolePrint("Memory Window Inactive");
		}
	}
}
void AllocationsConsoleCommands::AddConsoleCommands()
{
	RegisterCommandHelper _MemoryDebug = RegisterCommandHelper("memory_debug", ToggleMemoryDebugInformation, RegisterCommandHelper::DefaultHelpMessage);
	RegisterCommandHelper _PrintCallstackLines = RegisterCommandHelper("printCallstack", PrintCallstackLines, RegisterCommandHelper::DefaultHelpMessage);
	RegisterCommandHelper _PrintMemoryAmountsAllocatedPerAllocationType = RegisterCommandHelper("printMemoryByAllocationType", PrintMemoryAmountsAllocatedPerAllocationType, RegisterCommandHelper::DefaultHelpMessage);
	RegisterCommandHelper _SaveOutAllocationsToLogger = RegisterCommandHelper("threadedSaveOutAllocationsToLogger", ConsoleThreadedSaveOutAllocationsToLogger, RegisterCommandHelper::DefaultHelpMessage);
	Logger::RegisterCommands();
}

void AllocationsConsoleCommands::PrintCallstackLines(Command&)
{
	debug_mem_t::PrintAllCallstacks(Allocations::gNumberOfAllocations - Allocations::gNumberOfAllocationsAtStartup);
}
void AllocationsConsoleCommands::PrintMemoryAmountsAllocatedPerAllocationType(Command&)
{
	Allocations::PrintOutAllocationsPerTimeForAllTime();
}

void AllocationsConsoleCommands::ConsoleThreadedSaveOutAllocationsToLogger(Command&)
{
	Allocations::ThreadedSaveOutAllocationsToLogger();
}
