#pragma once

#ifndef CBUFFER_HPP
#define CBUFFER_HPP

class CBuffer
{
protected:
	char* buffer;
	size_t offset;
	size_t total_size;
public:
	CBuffer();

	void* alloc(const size_t& size);
	void* get_front();
	void init(void* ptr, const size_t& size);
};
#endif