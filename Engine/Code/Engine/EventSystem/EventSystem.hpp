#pragma once

#ifndef EVENTSYSTEM_HPP
#define EVENTSYSTEM_HPP
#include "Engine/EventSystem/RegisteredObjectFunction.hpp"
#include "Engine/EventSystem/RegisteredObjectMethod.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/StringAnimals/HashedCaseString.hpp"
#include <vector>
#include <map>

#define STATIC 
#define EventName HashedCaseString

class EventSystem;
extern EventSystem* g_EventSystem;

enum EventRegisterationErrorMessage
{
	Registration_Successful,
	Object_Or_Function_Is_Null,
	No_Name_was_Entered,
	Space_Found_In_Name, //If this error occurs then the name would not be console safe.
	Global_Event_System_Is_Null,
	NUM_OF_ERRORS
};

class EventSystem
{
private:
protected:

	//Variables
	std::map<EventName, std::vector<RegisteredObjectBase*> > m_functionRegistrations; //if strcmps are too slow, then swap out for something else.
	std::map<EventName, std::vector<RegisteredObjectBase*> > m_methodRegistrations;
	std::map<EventName, std::vector<ConsoleInputVar>> m_variablesExpectingForCommand;

#define FunctionRegistrationIterator std::map<EventName, std::vector<RegisteredObjectBase*> >::iterator
#define MethodRegistrationIterator std::map<EventName, std::vector<RegisteredObjectBase*> >::iterator

public:
	static std::string s_fileExtension;
	static std::string s_fileNameWithoutExtension;
	static std::string s_fileName;
	static std::string s_fileAbsolutePath;
	static std::string s_fileRelativePath;

	//Constructors
	EventSystem();
	virtual ~EventSystem();

	//Initialization and Deletion functions
	static void CreateGlobalEventSystem();
	static void DestroyGlobalEventSystem();


	//Fire Events EventName
	void FireEvent(const EventName& eventName, NamedProperties& params);
	void FireEvent(const EventName& eventName);


	//Event registeration with EventName
	//How to use Usage: first pick from predefined typings: bool, char, int, float, double
	//second name the variable. If you mess up the picking up the predefined types, or add in a space to the name accidently, it will mess this up.
	EventRegisterationErrorMessage RegisterEventCallbackFunction(const EventName& eventName, EventCallbackFunc* func, const std::string& usage = "", bool ifSpaceAddAnyway = false);
	template<typename T_object, typename T_methodPtr>
	EventRegisterationErrorMessage RegisterEventCallbackMethod(const EventName& eventName, T_object* object, T_methodPtr method, const std::string& usage = "", bool ifSpaceAddAnyway = false);

	//Event Unregistration with EventName
	void UnregisterEventCallbackFunction(const EventName& eventName);
	void UnregisterEventCallbackMethod(const EventName& eventName);
	void UnregisterEventCallback(const EventName& eventName);
	template<typename T_object>
	void UnregisterEventCallbackMethod(const EventName& eventName, T_object* obj);
	template<typename T_object>
	void UnregisterFromAllEvents(T_object* obj);
	void ClearAllEvents();

	void FireEventForEachFileFound(const std::string& eventToFire, const std::string& baseFolder, const std::string& filePattern,
		bool recurseSubFolders = false);
	void TryFireEventForFile(const std::string& eventToFire, std::string& fileLoc);

protected:
	void GetExpectedConsoleInputs(RegisteredObjectBase* val, const EventName& eventName);
	void RemoveExpectedConsoleInputs(RegisteredObjectBase* val, const EventName& eventName);

public:

	//Getters
	const std::map<EventName, std::vector<RegisteredObjectBase*>>* GetFunctionRegistrations();
	const std::map<EventName, std::vector<RegisteredObjectBase*>>* GetMethodRegistrations();
	const std::map<EventName, std::vector<ConsoleInputVar>>* GetVariablesExpectingForCommand();


	//Static Methods
	static void GlobalFireEvent(const EventName& eventName, NamedProperties& params);
	static void GlobalFireEvent(const EventName& eventName);
	static EventRegisterationErrorMessage GlobalRegisterEventCallbackFunction(const EventName& eventName, EventCallbackFunc* func, const std::string& usage = "", bool ifSpaceAddAnyway = false);
	template<typename T_object, typename T_methodPtr>
	static EventRegisterationErrorMessage GlobalRegisterEventCallbackMethod(const EventName& eventName, T_object* object, T_methodPtr method, const std::string& usage = "", bool ifSpaceAddAnyway = false);
	static void GlobalUnregisterEventCallbackFunction(const EventName& eventName);
	static void GlobalUnregisterEventCallbackMethod(const EventName& eventName);
	static void GlobalUnregisterEventCallback(const EventName& eventName);
	template<typename T_object>
	static void GlobalUnregisterEventCallbackMethod(const EventName& eventName, T_object* obj);
	template<typename T_object>
	static void GlobalUnregisterFromAllEvents(T_object* obj);
	static void GlobalClearAllEvents();
};

//-----------------------------------------------------------------------------------------------------------------------------------------------------


template<typename T_object, typename T_methodPtr>
EventRegisterationErrorMessage EventSystem::RegisterEventCallbackMethod(const EventName& eventName, T_object* object, T_methodPtr method, const std::string& usage, bool ifSpaceAddAnyway)
{
	if (object == nullptr || method == nullptr)
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
	RegisteredObjectMethod<T_object, T_methodPtr>* rom = new RegisteredObjectMethod<T_object, T_methodPtr>(object, method, usage, consoleCallable);
	MethodRegistrationIterator it = m_methodRegistrations.find(eventName);
	if (it == m_methodRegistrations.end())
	{
		std::vector<RegisteredObjectBase*> m_methods;
		m_methods.push_back(rom);
		m_methodRegistrations.insert(std::pair<EventName, std::vector<RegisteredObjectBase*>>(eventName, m_methods));
	}
	else
	{
		it->second.push_back(rom);
	}

	if (consoleCallable)
	{
		GetExpectedConsoleInputs(rom, eventName);
	}
	return Registration_Successful;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<typename T_object>
void EventSystem::UnregisterEventCallbackMethod(const EventName& eventName, T_object* obj)
{
	MethodRegistrationIterator itM = m_methodRegistrations.find(eventName);
	if (itM != m_methodRegistrations.end())
	{
		for (size_t i = 0; i < itM->second.size(); i++)
		{
			RegisteredObjectBase* objPacket = itM->second[i];
			if (objPacket->DoesMatchObject(obj))
			{
				RemoveExpectedConsoleInputs(objPacket, eventName);
				itM->second.erase(itM->second.begin() + i);
				delete objPacket;
				i--;
			}
		}
	}
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------
template<typename T_object>
void EventSystem::UnregisterFromAllEvents(T_object* obj)
{
	for (MethodRegistrationIterator itM = m_methodRegistrations.begin(); itM != m_methodRegistrations.end(); itM++)
	{
		for (size_t i = 0; i < itM->second.size(); i++)
		{
			RegisteredObjectBase* base = itM->second[i];
			if (base != nullptr && base->DoesMatchObject(obj))
			{
				RemoveExpectedConsoleInputs(base, itM->first);
				itM->second.erase(itM->second.begin() + i);
				delete base;
				i--;
			}
		}
	}
}



template<typename T_object, typename T_methodPtr>
STATIC EventRegisterationErrorMessage EventSystem::GlobalRegisterEventCallbackMethod(const EventName& eventName, T_object* object, T_methodPtr method, const std::string& usage, bool ifSpaceAddAnyway)
{
	if (g_EventSystem != nullptr)
	{
		return g_EventSystem->RegisterEventCallbackMethod<T_object, T_methodPtr>(eventName, object, method, usage, ifSpaceAddAnyway);
	}
	return Global_Event_System_Is_Null;
}

template<typename T_object>
STATIC void EventSystem::GlobalUnregisterEventCallbackMethod(const EventName& eventName, T_object* obj)
{
	if (g_EventSystem != nullptr)
	{
		g_EventSystem->UnregisterEventCallbackMethod<T_object>(eventName, obj);
	}
}

template<typename T_object>
STATIC void EventSystem::GlobalUnregisterFromAllEvents(T_object* obj)
{
	if (g_EventSystem != nullptr)
	{
		g_EventSystem->UnregisterFromAllEvents<T_object>(obj);
	}
}
#endif