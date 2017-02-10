#include "Engine/Console/AConsoleBuildMeFirsts.hpp"


std::map<std::string, console_command_cb>* ConsoleGlobals::m_commands = nullptr;
std::map<std::string, console_command_cb>* ConsoleGlobals::m_helpMessages = nullptr;