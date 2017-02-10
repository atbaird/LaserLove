#pragma once

#ifndef IBINARYWRITER_HPP
#define IBINARYWRITER_HPP
#include "Engine/Utils/CommonUtils.hpp"


//int32_t value = 0x01020304;
//byte_t *array = (byte_t*)&value;
// array[0] == 0x04;

//little endian = first one laid down in the byte.
//LITTLE_ENDIAN
//[0x04][0x03][0x02][0x01]

//BIG_ENDIAN
//[0x01][0x02][0x03][0x04]


class IBinaryWriter
{
private:
	eEndianess m_endianness = BIG_ENDIAN;
public:
	virtual const size_t WriteBytes(const void* src, const size_t& num_bytes) = 0;

	void SetEndianness(eEndianess mode);
	const bool WriteString(const char* string);

	template <typename T>
	const bool Write(const T& v)
	{
		T copy = v;
		if (m_endianness != GetLocalEndianess())
		{
			ByteSwap(&copy, sizeof(T));
		}
		return WriteBytes(&copy, sizeof(T)) == sizeof(T);
	}
};
#endif
