#pragma once

#ifndef ALLOCATIONSCONSOLECOMMANDS_HPP
#define ALLOCATIONSCONSOLECOMMANDS_HPP
#include "Engine/Utils/Allocations.hpp"
#include "Engine/Console/RegisterCommandHelper.hpp"

struct AllocationsConsoleCommands
{

	static void AddConsoleCommands();
	static void PrintCallstackLines(Command&);
	static void ToggleMemoryDebugInformation(Command&);
	static void PrintMemoryAmountsAllocatedPerAllocationType(Command&);


	static void ConsoleThreadedSaveOutAllocationsToLogger(Command&);
};

#endif