#pragma once
#include "Engine/Console/Command.hpp"
#include <string>

#ifndef REGISTERCOMMANDHELPER_HPP
#define REGISTERCOMMANDHELPER_HPP
/*
/////////////////////////////////////////////////////////////////////////
//MACROS
class RegisterCommandHelper
{
public:
	RegisterCommandHelper(char const *name, console_command_cb cb)
	{
		Console::RegisterCommand(name, cb);
	}
};
static RegisterCommandHelper _helpHelper("help", ShowHelp);

#define CONSOLE_COMMAND( name ) void ConsoleCommand_ ## name ## ( Command &args); \
	static RegisterCommandHelper RegistrationHelper_ ## name ## ( #name, ConsoleCommand_ ## name ## ); \
	void ConsoleCommand_ ## name ##(command &args)

CONSOLE_COMMAND(Help)
{
	//implement help
}
CONSOLE_COMMAND(Clear)
{
	//args
	//implement clear
}

// Becomes
void ConsoleCommand_Clear(Command &);
static RegisterCommandHelper RegistrationHelper_Clear("Clear", ConsoleCommand_Clear);
void ConsoleCommand_Clear(Command &args)
{

}*/

//-----------------------------------------------------
typedef void(*console_command_cb)(Command&);


class RegisterCommandHelper
{
public:
	static void DefaultHelpMessage(Command&);
	RegisterCommandHelper(const std::string& name = "", console_command_cb cb = nullptr, console_command_cb helpMessage = DefaultHelpMessage);
};

#define CONSOLE_COMMAND( name ) void ConsoleCommand_ ## name ## ( Command &args); \
	static RegisterCommandHelper RegistrationHelper_ ## name ## ( #name, ConsoleCommand_ ## name ## ); \
	void ConsoleCommand_ ## name ##(command &args)


#endif