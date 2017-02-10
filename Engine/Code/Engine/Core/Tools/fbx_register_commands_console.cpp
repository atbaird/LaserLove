#include "Engine/Core/Tools/fbx_register_commands_console.hpp"
#include "Engine/Console/Console.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/Tools/fbx.hpp"
#include <string>

void FbxList(Command& input)
{
	const std::string filename = input.get_string_by_index(1);
	FbxListScene(filename.c_str());
}

RegisterCommandHelper fbxCommandHolderStruct::_fbxList("fbxlist", FbxList);

void RegisterAllImportantFBXCommandsToConsole()
{
	if (g_Console != nullptr)
	{
		//static RegisterCommandHelper _helpHelper("help", Help);
		//static RegisterCommandHelper _fbxList("fbxlist", FbxList);
	}
	else
	{
		DebuggerPrintf("Console is Null!");
	}
}