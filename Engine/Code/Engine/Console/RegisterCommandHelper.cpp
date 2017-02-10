#include "Engine/Console/RegisterCommandHelper.hpp"
#include "Engine/Console/Console.hpp"

void RegisterCommandHelper::DefaultHelpMessage(Command&)
{
}

RegisterCommandHelper::RegisterCommandHelper(const std::string& name, console_command_cb cb, console_command_cb helpMessage)
{
	//if (g_Console != nullptr)
	{
		Console::RegisterCommand(name, cb, helpMessage);
	}
}