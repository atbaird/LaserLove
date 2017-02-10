#pragma once

#ifndef CONSOLEINPUTVAR_HPP
#define CONSOLEINPUTVAR_HPP
#include <string>

struct ConsoleInputVar
{
public:
	//Variables
	std::string m_variableName = "";
	std::string m_variableType = "";
	int m_numberExpectingThisVariable = 0;
public:
	//Constructors
	ConsoleInputVar();
};
#endif