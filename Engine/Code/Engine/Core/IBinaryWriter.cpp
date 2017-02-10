#include "Engine/Core/IBinaryWriter.hpp"
#include <string>

void IBinaryWriter::SetEndianness(eEndianess mode)
{
	m_endianness = mode;
}
const bool IBinaryWriter::WriteString(const char* string)
{
	//write length of the buffer
	if (nullptr == string)
	{
		Write<unsigned int>(0);
		return true;
	}
	size_t len = strlen(string);
	size_t buf_len = len + 1;
	bool firstSucceed = Write<unsigned int>(buf_len);
	bool succeed = (firstSucceed && WriteBytes(string, len) == len && WriteBytes('\0', 1) == 1);
	if (succeed == true)
	{
		//WriteBytes('\0', 1);
		return true;
	}
	return false;
}