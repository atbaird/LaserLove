#include "Engine/Utils/ThreadSafe/CASCalls.hpp"

size_t CASCalls::CAS(size_t* ptr, size_t value, size_t comparand)
{
	size_t old_value = *ptr;
	if (old_value == comparand)
	{
		*ptr = value;
	}
	return old_value;
}

int CASCalls::CAS(int* ptr, int value, int comparand)
{
	int old_value = *ptr;
	if (old_value == comparand)
	{
		*ptr = value;
	}
	return old_value;
}

bool CASCalls::CAS(bool* ptr, bool value, bool comparand)
{
	bool old_value = *ptr;
	if (old_value == comparand)
	{
		*ptr = value;
	}
	return old_value;
}