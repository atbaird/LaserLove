#pragma once

#ifndef CASCALLS_HPP
#define CASCALLS_HPP

class CASCalls
{
public:
	static size_t CAS(size_t* ptr, size_t value, size_t comparand);
	static int CAS(int* ptr, int value, int comparand);
	static bool CAS(bool* ptr, bool value, bool comparand);
};
#endif