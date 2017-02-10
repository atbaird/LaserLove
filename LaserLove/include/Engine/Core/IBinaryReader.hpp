#pragma once

#ifndef IBINARYREADER_HPP
#define IBINARYREADER_HPP
#include "Engine/Utils/CommonUtils.hpp"

class IBinaryReader
{
private:
	eEndianess m_endianness = BIG_ENDIAN;
public:

	virtual const size_t ReadBytes(void* out, unsigned int& num_bytes) = 0;

	void SetEndianness(eEndianess mode);
	const bool ReadString(char*& out);


	template <typename T>
	const bool Read(T& out, unsigned int& byte_size)
	{

		byte_size = ReadBytes(&out, byte_size);

		if (GetLocalEndianess() != m_endianness)
		{
			T copy;
			memcpy(&copy, &out, byte_size);
			ByteSwap(&copy, sizeof(T));
			memcpy(&out, &copy, byte_size);
		}
		return true;
	}
	/*

	template <typename T>
	const bool Write(const T& v);
	*/
};
#endif