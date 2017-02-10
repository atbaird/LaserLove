#include "Engine/Utils/CBuffer.hpp"


CBuffer::CBuffer()
	: buffer(nullptr),
	offset(0),
	total_size(0)
{

}

void* CBuffer::alloc(const size_t& size)
{
	char* ret = buffer + offset;
	offset += size;
	return ret;
}
void* CBuffer::get_front()
{
	return buffer + offset;
}
void CBuffer::init(void* ptr, const size_t& size)
{
	buffer = (char*)ptr;
	offset = 0;
	total_size = size;
}