#pragma once

#ifndef ACONSOLEBUILDMEFIRSTS_HPP
#define ACONSOLEBUILDMEFIRSTS_HPP
#include <map>

class Command;
//-----------------------------------------------------
typedef void(*console_command_cb)(Command&);

class ConsoleGlobals
{

public:
	static std::map<std::string, console_command_cb>* m_commands;
	static std::map<std::string, console_command_cb>* m_helpMessages;
};
#endif