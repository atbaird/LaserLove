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

const eEndianess GetOppositeEndianness(eEndianess org)
{
	if (org == BIG_ENDIAN)
	{
		return LITTLE_ENDIAN;
	}
	return BIG_ENDIAN;
}

void ByteSwap(void* data, const size_t& data_size)
{
	byte_t* org = reinterpret_cast<byte_t*>(data);
	size_t halfSize = data_size / 2;
	size_t j = data_size - 1;
	for (size_t i = 0; i < halfSize; i++)
	{
		byte_t firstByte = org[i];
		byte_t secondByte = org[j];
		org[i] = secondByte;
		org[j] = firstByte;
		j--;
	}
	memcpy(data, org, data_size);
}

void ReverseMemCpy(void* dest, const void* src, const size_t& count)
{
	byte_t* poo = (byte_t*)dest;
	byte_t* read = (byte_t*)src + count;
	for (size_t index = 0; index < count; ++index)
	{
		--read;
		*((byte_t*)poo) = *((byte_t*)read);
		++poo;
	}
}