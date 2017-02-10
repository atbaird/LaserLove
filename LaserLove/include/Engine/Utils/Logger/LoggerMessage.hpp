#pragma once

#ifndef LOGGERMESSAGE_HPP
#define LOGGERMESSAGE_HPP
#include "Engine/Utils/Logger/LoggerWarningLevel.hpp"
#include <string>

class LoggerMessage
{
private:
public:
	int level;
	std::string tag;
	std::string format; // message to be printed out.
	std::string time;

	LoggerMessage();
	~LoggerMessage();
};
#endif