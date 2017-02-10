#include "Engine/Utils/CommonUtils.hpp"
#include <stdint.h>
#include <string.h>



const eEndianess GetLocalEndianess()
{
	//if (htonl(1) == 1) //converts from host endian to local endian.
	//{
	//	//0x00000001 -> [0x00][0x00][0x00][0x01]
	//	return BIG_ENDIAN;
	//}
	//else
	//{
	//	//0x00000001 -> [0x01][0x00][0x00][0x00]
	//	return LITTLE_ENDIAN;
	//}

	union
	{
		byte_t bdata[4];
		uint32_t uidata;
	} data;

	data.uidata = 0x04030201;
	return (data.bdata[0] == 0x01)
		? LITTLE_ENDIAN
		: BIG_ENDIAN;

}

void ByteSwap(void* data, const size_t& data_size)
{
	byte_t* copy = reinterpret_cast<byte_t*>(data);
	byte_t* copy2 = new byte_t[data_size];
	for (size_t i = 0; i < data_size; i++)
	{
		byte_t val = copy[i];
		copy2[data_size - (1 + i)] = val;
	}
	memcpy(data, copy2, data_size);
	delete copy2;
}