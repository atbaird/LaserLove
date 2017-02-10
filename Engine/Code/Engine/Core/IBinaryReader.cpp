#include "Engine/Core/IBinaryReader.hpp"
#include <string>

void IBinaryReader::SetEndianness(eEndianess mode)
{
	m_endianness = mode;
}

const bool IBinaryReader::ReadString(char*& out)
{
	//write length of the buffer
	unsigned int len = 0;
	unsigned int size = sizeof(unsigned int);
	bool readSize = Read<unsigned int>(len, size);
	if (len == 0)
	{
		return true;
	}
	unsigned int len_unbuf = len - 1;

	bool readString = ReadBytes(out, len_unbuf) == len_unbuf;
	return readSize && readString;
}