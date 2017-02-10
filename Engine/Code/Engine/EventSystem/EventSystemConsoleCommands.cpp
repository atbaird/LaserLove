#include "Engine/EventSystem/EventSystemConsoleCommands.hpp"
#include "Engine/Console/Console.hpp"
#include "Engine/EventSystem/EventSystem.hpp"


STATIC void EventSystemConsoleCommands::RegisterEventSystemConsoleCommands()
{
	static RegisterCommandHelper _CommandListAllEventsCallableFromConsole("ListAllEventsCallableFromConsole", CommandListAllEventsCallableFromConsole, HelpCommandListAllEventsCallableFromConsole);
	static RegisterCommandHelper _CommandCallConsoleCallableEvent("CallConsoleCallableEvent", CommandCallConsoleCallableEvent, HelpCommandCallConsoleCallableEvent);
}

//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------
//Commands

STATIC void EventSystemConsoleCommands::CommandListAllEventsCallableFromConsole(Command& com)
{
	g_Console->ConsolePrintf(UNIMPORTANT, "--------------------");
	if (g_EventSystem == nullptr)
	{
		g_Console->ConsolePrintf(WARNING, "Can not list any events; g_EventSystem is null.");
		return;
	}
	com;
	const std::map<EventName, std::vector<ConsoleInputVar>>* inputsAndCommands = g_EventSystem->GetVariablesExpectingForCommand();
	if (inputsAndCommands->size() == 0)
	{
		g_Console->ConsolePrintf(UNIMPORTANT, "There are no registered Events");
		return;
	}

	for (std::map<EventName, std::vector<ConsoleInputVar>>::const_iterator it = inputsAndCommands->begin(); it != inputsAndCommands->end(); it++)
	{
		g_Console->ConsolePrintf(UNIMPORTANT, "Event Name: " + it->first.GetString());
		g_Console->ConsolePrintf(UNIMPORTANT, "---");
		for (size_t i = 0; i < it->second.size(); i++)
		{
			const ConsoleInputVar& var = it->second[i];
			g_Console->ConsolePrintf(UNIMPORTANT, "  " + var.m_variableName + " : "
				+ var.m_variableType + " : " + std::to_string(var.m_numberExpectingThisVariable));
		}
	}
	g_Console->ConsolePrintf(UNIMPORTANT, "--------------------");
}

STATIC void EventSystemConsoleCommands::CommandCallConsoleCallableEvent(Command& com)
{
	if (g_EventSystem == nullptr)
	{
		g_Console->ConsolePrintf(WARNING, "Can not call any events; g_EventSystem is null.");
		return;
	}
	std::string eventName = com.get_string_by_index(0);
	if (eventName.length() == 0)
	{
		g_Console->ConsolePrintf(WARNING, "Did not run event: no event defined");
		return;
	}
	const std::map<EventName, std::vector<ConsoleInputVar>>* inputsAndCommands = g_EventSystem->GetVariablesExpectingForCommand();
	std::map<EventName, std::vector<ConsoleInputVar>>::const_iterator it = inputsAndCommands->find(eventName);
	if (it == inputsAndCommands->end())
	{
		g_Console->ConsolePrintf(WARNING, "Did not run event: did not exist as a callable from console");
		g_Console->ConsolePrintf(WARNING, "event.");
		return;
	}
	NamedProperties params;

	std::string lastVarType = "";
	std::string lastVarName = "";
	for (size_t i = 1; i < com.get_number_of_variables(); i++)
	{
		std::string currentVar = com.get_string_by_index(i);
		if (currentVar.length() <= 0)
		{
			g_Console->ConsolePrintf(WARNING, "Did not run event: not enough data (need: valueType,");
			g_Console->ConsolePrintf(WARNING, "valueName, valueToSet)!");
			return;
		}

		if (i % 3 == 1)
		{
			std::string lowerCaseWord = MakeLower(currentVar);
			if (RegisteredObjectBase::VerifyVariableTypeStrIsOfValidType(lowerCaseWord))
			{
				lastVarType = lowerCaseWord;
			}
			else
			{
				g_Console->ConsolePrintf(WARNING, "Did not run event: invalid variable type or missing variable");
				g_Console->ConsolePrintf(WARNING, "type detected! (need: valueType, valueName, valueToSet)");
				return;
			}
		}
		else if (i % 3 == 2)
		{
			lastVarName = currentVar;
		}
		else
		{
			if (strcmp(lastVarType.c_str(), RegisteredObjectBase::s_BoolString) == 0)
			{
				bool val = false;
				std::string toLower = MakeLower(currentVar);
				if (strcmp(toLower.c_str(), "t") == 0
					|| strcmp(toLower.c_str(), "true") == 0)
				{
					val = true;
				}

				params.Set(lastVarName, val);
			}
			else if (strcmp(lastVarType.c_str(), RegisteredObjectBase::s_CharString) == 0)
			{
				char val = currentVar[0];
				params.Set(lastVarName, val);
			}
			else if (strcmp(lastVarType.c_str(), RegisteredObjectBase::s_IntString) == 0)
			{
				int val = atoi(currentVar.c_str());
				params.Set(lastVarName, val);
			}
			else if (strcmp(lastVarType.c_str(), RegisteredObjectBase::s_FloatString) == 0)
			{
				float val = (float)atof(currentVar.c_str());
				params.Set(lastVarName, val);
			}
			else if (strcmp(lastVarType.c_str(), RegisteredObjectBase::s_DoubleString) == 0)
			{
				double val = atof(currentVar.c_str());
				params.Set(lastVarName, val);
			}
			else if (strcmp(lastVarType.c_str(), RegisteredObjectBase::s_StringString) == 0)
			{
				std::string str = currentVar;
				params.Set(lastVarName, str);
			}
			lastVarType = "";
			lastVarName = "";
		}
	}

	g_Console->ConsolePrintf(UNIMPORTANT, "Firing event: " + eventName);
	g_EventSystem->FireEvent(eventName, params);



}


//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------
//Help Commands

STATIC void EventSystemConsoleCommands::HelpCommandListAllEventsCallableFromConsole(Command& com)
{
	com;
	g_Console->ConsolePrintf(UNIMPORTANT, "ListAllEventsCallableFromConsole will list all of the ");
	g_Console->ConsolePrintf(UNIMPORTANT, "events callable from the console.");
	/*
			g_Console->ConsolePrintf(UNIMPORTANT, "  " + var.m_variableName + " : "
				+ var.m_variableType + " : " + std::to_string(var.m_numberExpectingThisVariable));
	*/
	g_Console->ConsolePrintf(UNIMPORTANT, "It will first list the Event name, then separates out ");
	g_Console->ConsolePrintf(UNIMPORTANT, "the variables it takes via --- lines.");
	g_Console->ConsolePrintf(UNIMPORTANT, "order of the information is:");
	g_Console->ConsolePrintf(UNIMPORTANT, "VariableName : VariableType : RefernceCount");
	g_Console->ConsolePrintf(UNIMPORTANT, "For details about how to call one of these events type:");
	g_Console->ConsolePrintf(UNIMPORTANT, "help CallConsoleCallableEvent");
}


STATIC void EventSystemConsoleCommands::HelpCommandCallConsoleCallableEvent(Command& com)
{
	com;
	g_Console->ConsolePrintf(UNIMPORTANT, "CallConsoleCallableEvent Takes in an EventName followed");
	g_Console->ConsolePrintf(UNIMPORTANT, "by multiple ValueType, ValueName, and ValueToSetAs sets");
	g_Console->ConsolePrintf(UNIMPORTANT, "in that order.");
	g_Console->ConsolePrintf(UNIMPORTANT, "Note: Event Names are case sensitive, so lower and upper ");
	g_Console->ConsolePrintf(UNIMPORTANT, "case matters. Valid data types: bool, char, int, float,");
	g_Console->ConsolePrintf(UNIMPORTANT, "double, string. strings presently only do one word settings only.");
	g_Console->ConsolePrintf(UNIMPORTANT, "So make the best of it.");
	g_Console->ConsolePrintf(UNIMPORTANT, "Note as well that an EventName has to have no spaces in it to be");
	g_Console->ConsolePrintf(UNIMPORTANT, "possible to call from here.");
}