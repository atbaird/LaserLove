#pragma once

#ifndef PROFILEPRINTSCOPED_HPP
#define PROFILEPRINTSCOPED_HPP
#include "Engine/Utils/Profiler/ProfileSection.hpp"


class ProfilePrintScoped
{
private:
	const char* _id;
	ProfileSection _section;
public:
	//Constructors
	ProfilePrintScoped(const char* id);
	~ProfilePrintScoped();
};

#define PROFILE_PRINT_SCOPE(tag) ProfilePrintScoped _S_##__LINE__##(tag);

#endif