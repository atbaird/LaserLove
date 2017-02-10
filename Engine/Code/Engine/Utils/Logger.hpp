#pragma once

#ifndef LOGGER_HPP
#define LOGGER_HPP
#include <string>
#include "Engine/Utils/ThreadSafe/threadsafe_queue.hpp"
#include "Engine/Utils/Logger/LoggerMessage.hpp"
#include "Engine/Utils/Logger/LoggerWarningLevel.hpp"
#include "Engine/Console/RegisterCommandHelper.hpp"
#include <stdio.h>


class Logger
{
private:

	//static variables
	static const char* s_baseLogFileLoc;
	static const char* s_logNameFileLoc;
	static const char* s_endingLogFileLoc;

	FILE* file;
	FILE* fileWithDate;
	std::vector<const char*> whiteList;
	//static std::ofstream file;
	//static std::ofstream fileWithDate;
	size_t m_onlyHoldOntoNLogs  = 0;


public:
	static Logger* g_logger;

	//Constructors and Deconstructors
	Logger();
	virtual ~Logger();

	//Print, Start and End
	void LogPrintf(const char* format, const char* tag = nullptr, const int& level = 0);
	void Start();
	void End();

	//static methods
	static void GlobalStart();
	static void GlobalEnd();

	//console commands
	static void RegisterCommands();
	static void AddWhiteListValue(Command&);
	static void RemoveWhiteListValue(Command&);

	//helper console commands
	static void AddWhiteListValueHelp(Command&);
	static void RemoveWhiteListValueHelp(Command&);

private:
	//Private Methods
	static const bool IsFiltered(const char* tag);
	static const std::string GetString(const char* format, ...);
	void HandleMessage(LoggerMessage* message);
	LoggerMessage* ConstructMessage(const char* tag, const std::string& findString, const int& level);
	const std::string GetTimeAsStringForFiles();
	tm* GetCurrentTime();
	const std::string ConvertTimeToStringForFiles(tm* time);
};
#endif