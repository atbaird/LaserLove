#pragma once
#include "Engine/Console/Command.hpp"
#include "Engine/Console/RegisterCommandHelper.hpp"

#ifndef FBX_REGISTER_COMMANDS_CONSOLE_HPP
#define  FBX_REGISTER_COMMANDS_CONSOLE_HPP

struct fbxCommandHolderStruct
{
	static RegisterCommandHelper _fbxList;
};

void RegisterAllImportantFBXCommandsToConsole();

//Commands
void FbxList(Command& input);

#endif // !FBX_REGISTER_COMMANDS_CONSOLE_HPP
