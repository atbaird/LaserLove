#pragma once
#include "Engine/Renderer/Rgba.hpp"
#include <string>
#include <vector>

#ifndef COMMAND_HPP
#define COMMAND_HPP


class Command
{
public:
	Command();
	Command(const std::string& full_command_str); // split name and args into two buffers.

	const int get_number_of_variables() const { variables.size(); };
	const std::string get_command_name() const;
	const bool get_next_string(const std::string* out, const std::string& def) const;
	const std::string get_string_by_index(const int& index) const;
	const size_t get_number_of_variables();
	const std::string rebuild_string_from_variables(size_t startIdx, size_t endIdx);
	static const std::string make_string_lower_case(const std::string& input);

private:
	std::string commandName;
	std::vector<std::string> variables;
// 	{
// 		char const* arg;
// 		if (get_next_string(&arg, nullptr))
// 		{
// 			if (ParseColor(out, arg))
// 			{
// 				return true;
// 			}
// 		}
// 		*out = def_value;
// 		return false;
// 	}
};

#endif

/*
//recommendation
class Command
{
	public:
	Command(char const *full_command_str); // split name and args into two buffers.

	char const *get_command_name() const;
	bool get_next_string(char const** out, char const* default);
	bool get_next_color(rgba * out, Rgba def_value)
	{
		char const* arg;
		if(get_next_string(&arg, nullptr))
		{
			if(ParseColor(out, arg))
			{
			return true;
			}
		}
		*out = def_value;
		return false;
	}
}
*/