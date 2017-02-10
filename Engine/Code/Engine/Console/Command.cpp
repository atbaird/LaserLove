#include "Engine/Console/Command.hpp"
#include <sstream>
#include <algorithm>

Command::Command()
{
}
Command::Command(const std::string& full_command_str)
{
	bool hitFirstSpace = false;
	std::string varHolder = "";
	for (size_t i = 0; i < full_command_str.length(); i++)
	{
		unsigned char curChar = full_command_str.at(i);
		if (hitFirstSpace == false)
		{
			if (curChar == ' ' && varHolder.length() > 0)
			{
				commandName = varHolder;
				varHolder = "";
				hitFirstSpace = true;
				continue;
			}
			else if(curChar != ' ')
			{
				varHolder += curChar;
			}
		}
		else
		{
			if (curChar == ' ' && varHolder.length() > 0)
			{
				variables.push_back(varHolder);
				varHolder = "";
				continue;
			}
			else if(curChar != ' ')
			{
				varHolder += curChar;
			}
		}
	}
	if (varHolder.length() > 0)
	{
		if (commandName.length() == 0)
		{
			commandName = varHolder;
		}
		else
		{
			variables.push_back(varHolder);
		}
	}
	commandName = Command::make_string_lower_case(commandName);
}

const std::string Command::get_command_name() const
{
	return commandName;
}
const bool Command::get_next_string(const std::string* out, const std::string& def) const
{
	if (variables.size() <= 0)
	{
		out = &def;
		return false;
	}
	for (size_t i = 0; i < variables.size(); i++)
	{
		if (def == variables[i])
		{
			if (i >= variables.size() - 1)
			{
				out = &def;
				return false;
			}
			out = &variables[i];
			break;
		}
	}
	return true;
}

const std::string Command::get_string_by_index(const int& index) const
{
	if (index < 0 || index >= (int)variables.size())
	{
		return "";
	}
	std::string var0 = variables.at(index);
	return var0;
}

const size_t Command::get_number_of_variables()
{
	return variables.size();
}

const std::string Command::rebuild_string_from_variables(size_t startIdx, size_t endIdx)
{
	if (startIdx > endIdx)
	{
		return "";
	}
	if (endIdx >= variables.size())
	{
		endIdx = variables.size() - 1;
	}
	std::string str = "";
	for (size_t idx = startIdx; idx <= endIdx; idx++)
	{
		if (idx != startIdx)
		{
			str += " ";
		}
		str += variables.at(idx);
	}
	return str;
}


const std::string Command::make_string_lower_case(const std::string& input)
{
	std::string toReturn = input;
	std::transform(toReturn.begin(), toReturn.end(), toReturn.begin(), tolower);
	return toReturn;
}