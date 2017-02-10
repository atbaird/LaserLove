#include "Engine/Console/ConsoleCommands.hpp"


void Help(Command& input)
{
	input;
	g_Console->Help(input);
}

void Clear(Command& input)
{
	input;
	g_Console->Clear();
}

void Quit(Command& input)
{
	input;
	g_Console->quit();
}

void HelpHelpMessage(Command& input)
{
	input;
	g_Console->ConsolePrintf(UNIMPORTANT, "help provides some information on various commands");
	g_Console->ConsolePrintf(UNIMPORTANT, "provided in the console.");
	g_Console->ConsolePrintf(UNIMPORTANT, "follow the format of \'help <string>\' to successfully");
	g_Console->ConsolePrintf(UNIMPORTANT, "call it.");
}
void ClearHelpMessage(Command& input)
{
	input;
	g_Console->ConsolePrintf(UNIMPORTANT, "clear clears the Console of it's history.");
	g_Console->ConsolePrintf(UNIMPORTANT, "only need to type in \'clear\' to successfully call it.");
}
void QuitHelpMessage(Command& input)
{
	input;
	g_Console->ConsolePrintf(UNIMPORTANT, "quit causes the game to quit.");
	g_Console->ConsolePrintf(UNIMPORTANT, "only need to type in \'quit\' to successfully call it.");
}