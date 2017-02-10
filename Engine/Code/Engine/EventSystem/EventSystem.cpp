#include "Engine/EventSystem/EventSystem.hpp"
#include "Engine/Core/FileUtils.hpp"
#include "Engine/Core/StringUtils.hpp"

EventSystem* g_EventSystem = nullptr;
STATIC std::string EventSystem::s_fileExtension = "FileExension";
STATIC std::string EventSystem::s_fileNameWithoutExtension = "FileNameWithoutExtension";
STATIC std::string EventSystem::s_fileName = "FileName";
STATIC std::string EventSystem::s_fileAbsolutePath = "FileAbsolutePath";
STATIC std::string EventSystem::s_fileRelativePath = "FileRelativePath";

//-----------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------
//Constructors
EventSystem::EventSystem() 
{
}

EventSystem::~EventSystem() 
{
	ClearAllEvents(); 
}


//-----------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------
//Initialization and Deletion functions
STATIC void EventSystem::CreateGlobalEventSystem()
{
	if (g_EventSystem == nullptr)
	{
		g_EventSystem = new EventSystem();
	}
}

STATIC void EventSystem::DestroyGlobalEventSystem()
{
	if (g_EventSystem != nullptr)
	{
		delete g_EventSystem;
		g_EventSystem = nullptr;
	}
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------
//FireEvent with EventName
void EventSystem::FireEvent(const EventName& eventName, NamedProperties& params)
{
	FunctionRegistrationIterator it1 = m_functionRegistrations.find(eventName);
	if (it1 != m_functionRegistrations.end())
	{
		std::vector<RegisteredObjectBase*>& callbacks = it1->second;
		for (size_t i = 0; i < callbacks.size(); i++)
		{
			callbacks[i]->Execute(params);
		}
	}

	MethodRegistrationIterator it2 = m_methodRegistrations.find(eventName);
	if (it2 != m_methodRegistrations.end())
	{
		std::vector<RegisteredObjectBase*>& callbacks = it2->second;
		for (size_t i = 0; i < callbacks.size(); i++)
		{
			callbacks[i]->Execute(params);
		}
	}

}

void EventSystem::FireEvent(const EventName& eventName)
{
	NamedProperties params;
	FireEvent(eventName, params);
}


//-----------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------
//RegisterEventCallbackFunction
EventRegisterationErrorMessage EventSystem::RegisterEventCallbackFunction(const EventName& eventName, EventCallbackFunc* func, const std::string& usage, bool ifSpaceAddAnyway)
{
	if (func == nullptr)
	{
		return Object_Or_Function_Is_Null;
	}

	bool addReference = true;
	std::vector<EventName> parsedEventName = ParseString(eventName, ' ');
	if (parsedEventName.size() == 0)
	{
		return No_Name_was_Entered;
	}
	else if (parsedEventName.size() > 1)
	{
		if (ifSpaceAddAnyway == false)
		{
			return Space_Found_In_Name;
		}
		addReference = false;
	}

	bool consoleCallable = addReference;
	RegisteredObjectFunction* objFunc = new RegisteredObjectFunction(func, usage, consoleCallable);
	FunctionRegistrationIterator it1 = m_functionRegistrations.find(eventName);
	if (it1 == m_functionRegistrations.end())
	{
		std::vector<RegisteredObjectBase*> m_functions;
		m_functions.push_back(objFunc);
		m_functionRegistrations.insert(std::pair<EventName, std::vector<RegisteredObjectBase*>>(eventName, m_functions));
	}
	else
	{
		it1->second.push_back(objFunc);
	}

	if (consoleCallable)
	{
		GetExpectedConsoleInputs(objFunc, eventName);
	}
	return Registration_Successful;
}


//-----------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------
//Event Unregistration with EventName
void EventSystem::UnregisterEventCallbackFunction(const EventName& eventName)
{
	FunctionRegistrationIterator it1 = m_functionRegistrations.find(eventName);
	if (it1 == m_functionRegistrations.end())
	{
		return;
	}
	std::vector<RegisteredObjectBase*>& bases = it1->second;
	for (size_t i = 0; i < bases.size(); i++)
	{
		if (bases[i] != nullptr)
		{
			RemoveExpectedConsoleInputs(bases[i], eventName);
			delete bases[i];
			bases[i] = nullptr;
		}
	}
	m_functionRegistrations.erase(it1);
}

void EventSystem::UnregisterEventCallbackMethod(const EventName& eventName)
{
	MethodRegistrationIterator it = m_methodRegistrations.find(eventName);
	if (it == m_methodRegistrations.end())
	{
		return;
	}
	std::vector<RegisteredObjectBase*>& bases = it->second;
	for (size_t i = 0; i < bases.size(); i++)
	{
		if (bases[i] != nullptr)
		{
			RemoveExpectedConsoleInputs(bases[i], eventName);
			delete bases[i];
			bases[i] = nullptr;
		}
	}
	m_methodRegistrations.erase(it);
}


void EventSystem::UnregisterEventCallback(const EventName& eventName)
{
	UnregisterEventCallbackFunction(eventName);
	UnregisterEventCallbackMethod(eventName);
}

void EventSystem::ClearAllEvents()
{
	for (FunctionRegistrationIterator it = m_functionRegistrations.begin(); it != m_functionRegistrations.end(); it++)
	{
		for (size_t i = 0; i < it->second.size(); i++)
		{
			RegisteredObjectBase* obj = it->second[i];
			if (obj != nullptr)
			{
				delete obj;
			}
			it->second.erase(it->second.begin() + i);
			i--;
		}
	}

	m_functionRegistrations.clear();

	for (MethodRegistrationIterator it = m_methodRegistrations.begin(); it != m_methodRegistrations.end(); it++)
	{
		for (size_t i = 0; i < it->second.size(); i++)
		{
			RegisteredObjectBase* obj = it->second[i];
			if (obj != nullptr)
			{
				delete obj;
			}
			it->second.erase(it->second.begin() + i);
			i--;
		}
	}
	m_methodRegistrations.clear();
	m_variablesExpectingForCommand.clear();
}


void EventSystem::FireEventForEachFileFound(const std::string& eventToFire, const std::string& baseFolder, const std::string& filePattern,
	bool recurseSubFolders)
{
	std::vector<std::string> files = FileUtils::EnumerateFilesInDirectory(baseFolder, filePattern, recurseSubFolders);

	for (size_t i = 0; i < files.size(); i++)
	{
		TryFireEventForFile(eventToFire, files[i]);
	}
}

void EventSystem::TryFireEventForFile(const std::string& eventToFire, std::string& fileLoc)
{
	std::string fileCopy = fileLoc;
	NamedProperties params;
	std::string basePath = FileUtils::GetProjectFilePath();

	int lastPeriodIdx = GetNthEncounterIdxOfCharInStrReverse(fileLoc, '.', 1);
	int lastForwardSlashIdx = GetNthEncounterIdxOfCharInStrReverse(fileLoc, '/', 1);

	if (lastForwardSlashIdx == -1)
	{
		lastForwardSlashIdx = 0;
	}
	size_t charCntFromSlash = fileLoc.size() - (lastForwardSlashIdx + 1);
	if (lastPeriodIdx != -1)
	{
		size_t charCntFromPeriod = fileLoc.size() - lastPeriodIdx;
		size_t charCntFromSlashWithoutExtension = charCntFromSlash - charCntFromPeriod;
		params.Set(EventSystem::s_fileExtension, fileLoc.substr(lastPeriodIdx, charCntFromPeriod));
		params.Set(EventSystem::s_fileNameWithoutExtension, fileLoc.substr(lastForwardSlashIdx + 1, charCntFromSlashWithoutExtension));
	}
	params.Set(EventSystem::s_fileName, fileLoc.substr(lastForwardSlashIdx + 1, charCntFromSlash));

	if (fileLoc.size() >= basePath.size() && strcmp(basePath.c_str(), fileLoc.substr(0, basePath.size()).c_str()) == 0)
	{
		params.Set(EventSystem::s_fileAbsolutePath, fileCopy);
		params.Set(EventSystem::s_fileRelativePath, fileLoc.substr(basePath.size(), fileLoc.size() - basePath.size()));
	}
	else
	{
		params.Set(EventSystem::s_fileAbsolutePath, basePath + fileLoc);
		params.Set(EventSystem::s_fileRelativePath, fileCopy);
	}


	FireEvent(eventToFire, params);
}


void EventSystem::GetExpectedConsoleInputs(RegisteredObjectBase* val, const EventName& eventName)
{
	if (val == nullptr)
	{
		return;
	}
	
	std::map<EventName, std::vector<ConsoleInputVar>>::iterator it = m_variablesExpectingForCommand.find(eventName);
	if (it != m_variablesExpectingForCommand.end())
	{
		for (size_t varIdx = 0; varIdx < val->m_consoleVariables.size(); varIdx++)
		{
			ConsoleInputVar& varData = val->m_consoleVariables[varIdx];
			bool found = false;
			for (size_t i = 0; i < it->second.size(); i++)
			{
				ConsoleInputVar& compiledData = it->second[i];
				if (strcmp(compiledData.m_variableName.c_str(), varData.m_variableName.c_str()) == 0)
				{
					varData.m_numberExpectingThisVariable++;
					found = true;
					break;
				}
			}
			if (found)
			{
				it->second.push_back(val->m_consoleVariables[varIdx]);
			}
		}
	}
	else
	{
		m_variablesExpectingForCommand.insert(std::pair<EventName, std::vector<ConsoleInputVar>>(eventName, val->m_consoleVariables));
	}
}

void EventSystem::RemoveExpectedConsoleInputs(RegisteredObjectBase* val, const EventName& eventName)
{
	if (val == nullptr)
	{
		return;
	}

	std::map<EventName, std::vector<ConsoleInputVar>>::iterator it = m_variablesExpectingForCommand.find(eventName);
	if (it == m_variablesExpectingForCommand.end())
	{
		return;
	}
	for (size_t varIdx = 0; varIdx < val->m_consoleVariables.size(); varIdx++)
	{
		ConsoleInputVar& varData = val->m_consoleVariables[varIdx];
		for (size_t i = 0; i < it->second.size(); i++)
		{
			ConsoleInputVar& compiledData = it->second[i];
			if (strcmp(compiledData.m_variableName.c_str(), varData.m_variableName.c_str()) == 0)
			{
				compiledData.m_numberExpectingThisVariable--;
				if (compiledData.m_numberExpectingThisVariable == 0)
				{
					it->second.erase(it->second.begin() + i);
				}
				break;
			}
		}
	}
}


//-----------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------
//Getters
const std::map<EventName, std::vector<RegisteredObjectBase*>>* EventSystem::GetFunctionRegistrations()
{
	return &m_functionRegistrations;
}

const std::map<EventName, std::vector<RegisteredObjectBase*>>* EventSystem::GetMethodRegistrations()
{
	return &m_methodRegistrations;
}

const std::map<EventName, std::vector<ConsoleInputVar>>* EventSystem::GetVariablesExpectingForCommand()
{
	return &m_variablesExpectingForCommand;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------------------------------------------------
//Static Functions


STATIC void EventSystem::GlobalFireEvent(const EventName& eventName, NamedProperties& params)
{
	if (g_EventSystem != nullptr)
	{
		g_EventSystem->FireEvent(eventName, params);
	}
}
STATIC void EventSystem::GlobalFireEvent(const EventName& eventName)
{
	if (g_EventSystem != nullptr)
	{
		g_EventSystem->FireEvent(eventName);
	}
}

STATIC EventRegisterationErrorMessage EventSystem::GlobalRegisterEventCallbackFunction(const EventName& eventName, EventCallbackFunc* func, const std::string& usage, bool ifSpaceAddAnyway)
{
	if (g_EventSystem != nullptr)
	{
		return g_EventSystem->RegisterEventCallbackFunction(eventName, func, usage, ifSpaceAddAnyway);
	}
	return Global_Event_System_Is_Null;
}

STATIC void EventSystem::GlobalUnregisterEventCallbackFunction(const EventName& eventName)
{
	if (g_EventSystem != nullptr)
	{
		g_EventSystem->UnregisterEventCallbackFunction(eventName);
	}
}
STATIC void EventSystem::GlobalUnregisterEventCallbackMethod(const EventName& eventName)
{
	if (g_EventSystem != nullptr)
	{
		g_EventSystem->UnregisterEventCallbackMethod(eventName);
	}
}

STATIC void EventSystem::GlobalUnregisterEventCallback(const EventName& eventName)
{
	if (g_EventSystem != nullptr)
	{
		g_EventSystem->UnregisterEventCallback(eventName);
	}
}

STATIC void EventSystem::GlobalClearAllEvents()
{
	if (g_EventSystem != nullptr)
	{
		g_EventSystem->ClearAllEvents();
	}
}