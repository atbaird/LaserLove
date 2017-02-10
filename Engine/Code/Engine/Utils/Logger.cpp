#include "Engine/Utils/Logger.hpp"
#include "Engine/Utils/Thread.hpp"
#include "Engine/Console/Console.hpp"
#include <ctime>


//static variables
const char* Logger::s_baseLogFileLoc = "Data/Logs/";
const char* Logger::s_logNameFileLoc = "debug";
const char* Logger::s_endingLogFileLoc = ".log";

Logger* Logger::g_logger = nullptr;

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//Constructors and Deconstructors

Logger::Logger()
	: file(nullptr),
	fileWithDate(nullptr)
{

}

Logger::~Logger()
{
	if (file != nullptr)
	{
		fclose(file);
		file = nullptr;
	}
	if (fileWithDate != nullptr)
	{
		fclose(fileWithDate);
		fileWithDate = nullptr;
	}
}


//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//Print, start and end
void Logger::LogPrintf(const char* format, const char* tag, const int& level)
{
	if (IsFiltered(tag))
	{
		return;
	}
	//Amdahl's law
	std::string findString = GetString(format);
	LoggerMessage* msg = ConstructMessage(tag, findString, level);// , options);
	HandleMessage(msg);
}


void Logger::Start()
{
	if (file != nullptr || fileWithDate != nullptr)
	{
		return;
	}
	std::string baseFileName = (std::string(s_baseLogFileLoc) + std::string(s_logNameFileLoc));
	std::string dateDoneFileName = baseFileName;
	std::string time = GetTimeAsStringForFiles();
	std::string fileEnding = std::string(s_endingLogFileLoc);
	file = fopen((baseFileName + fileEnding).c_str(), "w");
	fileWithDate = fopen((dateDoneFileName + time + fileEnding).c_str(), "w");
}

void Logger::End()
{
	if (file != nullptr)
	{
		fclose(file);
	}
	if (fileWithDate != nullptr)
	{
		fclose(fileWithDate);
	}
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//static methods
void Logger::GlobalStart()
{
	if (g_logger != nullptr)
	{
		return;
	}
	g_logger = new Logger();
	g_logger->Start();
}

void Logger::GlobalEnd()
{
	if (g_logger == nullptr)
	{
		return;
	}
	g_logger->End();
	delete g_logger;
	g_logger = nullptr;
}



//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//Console Commands
void Logger::RegisterCommands()
{
	RegisterCommandHelper _AddWhiteListValue = RegisterCommandHelper("addWhiteList", AddWhiteListValue, AddWhiteListValueHelp);
	RegisterCommandHelper _RemoveWhiteListValue = RegisterCommandHelper("removeWhiteList", AddWhiteListValue, RemoveWhiteListValueHelp);
}

void Logger::AddWhiteListValue(Command& cmd)
{
	std::string whiteListVal = cmd.get_string_by_index(0);
	if (strcmp(whiteListVal.c_str(), "") == 0)
	{
		g_Console->ConsolePrint("Could not add; no white list value set.");
		return;
	}
	else if (g_logger == nullptr)
	{
		g_Console->ConsolePrint("g_logger is null");
		return;
	}
	
	g_Console->ConsolePrint("\'" + whiteListVal + "\' was added");
	
	g_logger->whiteList.push_back(whiteListVal.c_str());
}

void Logger::RemoveWhiteListValue(Command& cmd)
{
	std::string whiteListVal = cmd.get_string_by_index(0);
	if (strcmp(whiteListVal.c_str(), "") == 0)
	{
		g_Console->ConsolePrint("Could not remove; no white list value set.");
		return;
	}
	else if (g_logger == nullptr)
	{
		g_Console->ConsolePrint("g_logger is null");
		return;
	}

	for (size_t i = 0; i < g_logger->whiteList.size(); i++)
	{
		if (strcmp(g_logger->whiteList.at(i), whiteListVal.c_str()) == 0)
		{
			g_Console->ConsolePrint("\'" + whiteListVal + "\' successfully removed");
			g_logger->whiteList.erase(g_logger->whiteList.begin() + i);
			return;
		}
	}
	g_Console->ConsolePrint("Could not remove; value is not in white list.");
}



//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//Help Console Commands
void Logger::AddWhiteListValueHelp(Command&)
{
	g_Console->ConsolePrint("AddWhiteList takes a string of chars afterwards, and then creates\na white list value for the Loggers");
}
void Logger::RemoveWhiteListValueHelp(Command&)
{
	g_Console->ConsolePrint("AddWhiteList takes a string of chars afterwards, and then removes\na white list value from the Loggers");
}


//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//Private Methods
const bool Logger::IsFiltered(const char* tag)
{
	if (g_logger == nullptr || g_logger->whiteList.size() == 0)
	{
		return false;
	}
	for (size_t i = 0; i < g_logger->whiteList.size(); i++)
	{
		if (strcmp(g_logger->whiteList.at(i), tag) == 0)
		{
			return false;
		}
	}

	return true;
}

const std::string Logger::GetString(const char* format, ...)
{
	return std::string(format) + "\0";
}

void Logger::HandleMessage(LoggerMessage* message)
{
	if (file == nullptr || fileWithDate == nullptr)
	{
		return;
	}
	std::string fullline = std::string(message->tag) + " " + std::to_string(message->level) + " " + message->time
		+ "\n" + message->format + "\n";
	//file << fullline;
	//fileWithDate << fullline;
	fwrite(fullline.c_str(), sizeof(char), fullline.length(), file);
	fwrite(fullline.c_str(), sizeof(char), fullline.length(), fileWithDate);
}

LoggerMessage* Logger::ConstructMessage(const char* tag, const std::string& findString, const int& level)
{
	LoggerMessage* mes = new LoggerMessage();
	std::string time = GetTimeAsStringForFiles();
	mes->format = findString;
	mes->tag = std::string(tag);
	mes->level = level;
	mes->time = " ";
	return mes;
}

const std::string Logger::GetTimeAsStringForFiles()
{
	//YYYYMMDD_HHMMSS
	//Time looked up via: http://www.cplusplus.com/forum/beginner/32329/

	return ConvertTimeToStringForFiles(GetCurrentTime());
}

tm* Logger::GetCurrentTime()
{
	time_t t = time(NULL);
	tm* timePtr = gmtime(&t);
	localtime_s(timePtr, &t);
	return timePtr;
}

const std::string Logger::ConvertTimeToStringForFiles(tm* timePtr)
{
	std::string zero = std::to_string(0);
	int year = timePtr->tm_year + 1900;
	int month = timePtr->tm_mon + 1;
	int day = timePtr->tm_mday;
	int hour = timePtr->tm_hour;
	int min = timePtr->tm_min;
	int sec = timePtr->tm_sec;
	std::string fullStr = std::to_string(year);
	if (month < 10)
	{
		fullStr += zero + std::to_string(month);
	}
	else
	{
		fullStr += std::to_string(month);
	}
	if (day < 10)
	{
		fullStr += zero + std::to_string(day);
	}
	else
	{
		fullStr += std::to_string(day);
	}
	fullStr += "_";
	if (hour < 10)
	{
		fullStr += zero + std::to_string(hour);
	}
	else
	{
		fullStr += std::to_string(hour);
	}
	if (min < 10)
	{
		fullStr += zero + std::to_string(min);
	}
	else
	{
		fullStr += std::to_string(min);
	}
	if (sec < 10)
	{
		fullStr += zero + std::to_string(sec);
	}
	else
	{
		fullStr += std::to_string(sec);
	}
	return fullStr;
}