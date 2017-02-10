#pragma once
#include "Engine/Console/Console.hpp"
#include "Engine/Console/Command.hpp"

void Help(Command& input);
void Clear(Command& input);
void Quit(Command& input);

void HelpHelpMessage(Command& input);
void ClearHelpMessage(Command& input);
void QuitHelpMessage(Command& input);