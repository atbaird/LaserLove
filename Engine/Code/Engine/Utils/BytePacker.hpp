#pragma once

#ifndef BYTEPACKER_HPP
#define BYTEPACKER_HPP
#include "Engine/Utils/CommonUtils.hpp"
#include "Engine/Math/Specialty/Max.hpp"
#include "Engine/Math/Specialty/Min.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include <string>

/*
PING
PONG
HEARTBEAT
AACK
JOIN_REQUEST
JOIN_RESPONSE
*/

class BytePacker
{
private:
protected:
	//Variables
	void* buffer;
	size_t write_size_max;
	size_t read_size_max;
	eEndianess endianness;
	mutable size_t _offset; //write & Read offset
	bool deleteMydata;
	//mutable = editable anywhere
public:
	//--------------------------------------------------------------------------------
	//Constructor
	BytePacker();
	BytePacker(void* head, size_t size);
	virtual ~BytePacker();

	//--------------------------------------------------------------------------------
	//Setters
	void SetBuffer(void* buf, size_t size, bool deletePrevious = true);

	//--------------------------------------------------------------------------------
	//Write
	template<typename t>
	size_t WriteForward(const t* data, const size_t& datasize);

	template<typename t>
	size_t WriteBackward(const t* data, const size_t& datasize);

	template<typename t>
	size_t Write(const t* data, const size_t& datasize);
	void WriteEndiannes();
	void WriteString(const char* str);

	template<typename t>
	void* ReserveWriteBookmark(t* defaultVal, size_t size);

	template<typename t>
	void WriteBookmark(void* bookmark, t* val, size_t size);

	//--------------------------------------------------------------------------------
	//Read
	template<typename t>
	size_t ReadForwards(t* data, const size_t& datasize) const;

	template<typename t>
	size_t ReadBackwards(t* data, const size_t& datasize) const;

	template<typename t>
	size_t Read(t* data, const size_t& read_size) const;

	void ReadEndianness();
	const char* ReadString() const;

	//--------------------------------------------------------------------------------
	//Advance
	size_t Advance(size_t amt);
	size_t AdvanceRead(size_t amt) const;
	void SetOffset(size_t offset);
	void ResetOffset() const;
	void SetDeleteMyData(bool deleteMe);

	//--------------------------------------------------------------------------------
	//Endianness
	void SetEndianness(eEndianess endian);
	eEndianess GetEndianness() const;

	//--------------------------------------------------------------------------------
	//Readable Bytes
	void SetReadableBytes(size_t readableByteCount);
	uint16_t GetReadableBytes() const;
	uint16_t GetTotalReadableBytes() const;

	//--------------------------------------------------------------------------------
	//Writable Bytes
	uint16_t GetWriteableBytes() const;
	uint16_t GetTotalWriteableBytes() const;

	//--------------------------------------------------------------------------------
	//Getters
	void* GetTail() const;
	char* GetHead() const;
	size_t GetOffset() const;
	void ClearData(size_t amt);
};

#endif


//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//Write
template<typename t>
size_t BytePacker::WriteForward(const t* data, const size_t& datasize)
{
	char* head = GetHead();
	if (GetWriteableBytes() < datasize || head == nullptr)
	{
		return 0;
	}
	memcpy((void*)head, data, datasize);
	Advance(datasize);
	return datasize;
}
template<typename t>
size_t BytePacker::WriteBackward(const t* data, const size_t& datasize)
{
	char* head = GetHead();
	if (GetWriteableBytes() < datasize || head == nullptr)
	{
		return 0;
	}
	ReverseMemCpy(head, data, datasize);
	Advance(datasize);
	return datasize;
}

template<typename t>
size_t BytePacker::Write(const t* data, const size_t& datasize)
{
	if (GetLocalEndianess() == endianness)
	{
		return WriteForward<t>(data, datasize);
	}
	else
	{
		return WriteBackward<t>(data, datasize);
	}
}


template<typename t>
void* BytePacker::ReserveWriteBookmark(t* defaultVal, size_t size)
{
	void* head = GetHead();
	Write<t>(defaultVal, size);

	return head;
}
template<typename t>
void BytePacker::WriteBookmark(void* bookmark, t* val, size_t size)
{
	if (GetLocalEndianess() == endianness)
	{
		memcpy(bookmark, val, size);
	}
	else
	{
		ReverseMemCpy(bookmark, val, size);
	}
}

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//Read
template<typename t>
size_t BytePacker::ReadForwards(t* data, const size_t& datasize) const
{
	if (GetReadableBytes() < datasize || buffer == nullptr)
	{
		return 0;
	}
	char* head = GetHead();
	memcpy(data, head, datasize);
	AdvanceRead(datasize);
	return datasize;
}

template<typename t>
size_t BytePacker::ReadBackwards(t* data, const size_t& datasize) const
{
	if (GetReadableBytes() < datasize || buffer == nullptr)
	{
		return 0;
	}
	char* head = GetHead();
	ReverseMemCpy(data, head, datasize);
	AdvanceRead(datasize);
	return datasize;
}

template<typename t>
size_t BytePacker::Read(t* data, const size_t& read_size) const
{
	if (GetLocalEndianess() == endianness)
	{
		return ReadForwards<t>(data, read_size);
	}
	else
	{
		return ReadBackwards<t>(data, read_size);
	}
}