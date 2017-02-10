#pragma once

#ifndef EVENTSYSTEMCONSOLECOMMANDS_HPP
#define EVENTSYSTEMCONSOLECOMMANDS_HPP
#include "Engine/Console/RegisterCommandHelper.hpp"

#define STATIC

struct EventSystemConsoleCommands
{
public:
	static void RegisterEventSystemConsoleCommands();

	//Commands
	static void CommandListAllEventsCallableFromConsole(Command& com);
	static void CommandCallConsoleCallableEvent(Command& com);

	//Help Commands
	static void HelpCommandListAllEventsCallableFromConsole(Command& com);
	static void HelpCommandCallConsoleCallableEvent(Command& com);
};
#endif