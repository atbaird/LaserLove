#pragma once

#ifndef REGISTEREDOBJECTBASE_HPP
#define REGISTEREDOBJECTBASE_HPP
#include "Engine/EventSystem/NamedProperties.hpp"
#include "Engine/EventSystem/ConsoleInputVar.hpp"
#include <vector>

#define STATIC


struct RegisteredObjectBase
{
public:
	//bool, char, int, float, double
	static const char* s_BoolString;
	static const char* s_CharString;
	static const char* s_IntString;
	static const char* s_FloatString;
	static const char* s_DoubleString;
	static const char* s_StringString;
public:

	//Variables
	std::vector<ConsoleInputVar> m_consoleVariables;
	std::string m_callbackName = "";
	std::string m_usage = "";
	bool m_consoleCallable = false;

public:
	//Constructors
	RegisteredObjectBase( const std::string& usage = "", bool consoleCallable = false);
	virtual ~RegisteredObjectBase();

	//Getters
	const std::string GetCallbackName() const { return m_callbackName; };
	const std::string GetUsage() const { return m_usage; };
	const bool GetConsoleCallable() const { return m_consoleCallable; };

	//Overrideable Methods
	virtual void Execute(NamedProperties& params) = 0;
	virtual bool DoesMatchObject(void*) const { return false; };
	virtual void* GetObjectAddress() const { return nullptr; };

	static bool VerifyVariableTypeStrIsOfValidType(const std::string& val);

protected:
	virtual void ParseForIfReallyCanDoConsoleCommand();
};

#endif