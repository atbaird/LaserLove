#include "Engine/Utils/Profiler/ProfilePrintScoped.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"


//Constructors
ProfilePrintScoped::ProfilePrintScoped(const char* id)
{
	_id = id;
	_section.start();
}
ProfilePrintScoped::~ProfilePrintScoped()
{
	_section.end();
	DebuggerPrintf("profiler", "%s took %.8f", _id, _section.GetSeconds());
}