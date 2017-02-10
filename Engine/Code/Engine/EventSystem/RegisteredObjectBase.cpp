#include "Engine/EventSystem/RegisteredObjectBase.hpp"
#include "Engine/Core/StringUtils.hpp"
#include <sstream>

STATIC const char* RegisteredObjectBase::s_BoolString = "bool";
STATIC const char* RegisteredObjectBase::s_CharString = "char";
STATIC const char* RegisteredObjectBase::s_IntString = "int";
STATIC const char* RegisteredObjectBase::s_FloatString = "float";
STATIC const char* RegisteredObjectBase::s_DoubleString = "double";
STATIC const char* RegisteredObjectBase::s_StringString = "string";

RegisteredObjectBase::RegisteredObjectBase(const std::string& usage, bool consoleCallable)
	: m_usage(usage),
	m_consoleCallable(consoleCallable)
{
	ParseForIfReallyCanDoConsoleCommand();
}
RegisteredObjectBase::~RegisteredObjectBase() 
{
}

bool RegisteredObjectBase::VerifyVariableTypeStrIsOfValidType(const std::string& lowerCaseWord)
{
	if (strcmp(lowerCaseWord.c_str(), s_BoolString) == 0
		|| strcmp(lowerCaseWord.c_str(), s_CharString) == 0
		|| strcmp(lowerCaseWord.c_str(), s_IntString) == 0
		|| strcmp(lowerCaseWord.c_str(), s_FloatString) == 0
		|| strcmp(lowerCaseWord.c_str(), s_DoubleString) == 0
		|| strcmp(lowerCaseWord.c_str(), s_StringString) == 0)
	{
		return true;
	}
	return false;
}

void RegisteredObjectBase::ParseForIfReallyCanDoConsoleCommand()
{
	if (m_consoleCallable == false)
	{
		return;
	}

	std::vector<std::string> parsedName = ParseString(m_callbackName, ' ');
	if (parsedName.size() > 1)
	{
		//Can not run! name is not Console safe!
		m_consoleCallable = false;
		return;
	}

	std::vector<std::string> parsedWords = ParseString(m_usage, ' ');

	if (parsedWords.size() % 2 == 1)
	{
		//Not enough information to work with.
		m_consoleCallable = false;
		return;
	}

	ConsoleInputVar var;
	for (size_t i = 0; i < parsedWords.size(); i++)
	{
		std::string word = parsedWords[i];
		bool idxEven = i % 2 == 0;
		if (idxEven)
		{
			var = ConsoleInputVar();
			std::string lowerCaseWord = MakeLower(word);
			if (VerifyVariableTypeStrIsOfValidType(lowerCaseWord))
			{
				var.m_variableType = lowerCaseWord;
			}
			else
			{
				//INVALID INPUT!
				m_consoleVariables.clear();
				m_consoleCallable = false;
				break;
			}
		}
		else
		{
			var.m_variableName = word;
			var.m_numberExpectingThisVariable = 1;
			m_consoleVariables.push_back(var);
		}
	}
}