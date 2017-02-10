#include "Engine/Utils/BytePacker.hpp"

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//Constructor
BytePacker::BytePacker()
	: buffer(nullptr),
	read_size_max(0),
	write_size_max(0),
	endianness(BIG_ENDIAN),
	_offset(0),
	deleteMydata(true)
{
}

BytePacker::BytePacker(void* head, size_t size)
	: buffer(head),
	read_size_max(size),
	write_size_max(size),
	endianness(BIG_ENDIAN),
	_offset(0),
	deleteMydata(true)
{
}

BytePacker::~BytePacker()
{
	if (buffer != nullptr && deleteMydata)
	{
		delete[] buffer;
		buffer = nullptr;
	}
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//Setters
void BytePacker::SetBuffer(void* buf, size_t size, bool deletePrevious)
{
	if (deletePrevious == true && buffer != nullptr)
	{
		delete[] buffer;
	}
	buffer = buf;
	read_size_max = size;
	write_size_max = size;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//Write


void BytePacker::WriteEndiannes()
{
	const size_t i = 1;
	WriteForward(&i, sizeof(size_t));
}

void BytePacker::WriteString(const char* str)
{
	if (str == nullptr)
	{
		const byte_t byt = 0xff;
		Write<byte_t>(&byt, sizeof(byte_t));
	}
	else
	{
		size_t len = strlen(str) + 1;
		WriteForward<const char>(str, len);
	}
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//Read
void BytePacker::ReadEndianness()
{
	if (buffer == nullptr)
	{
		return;
	}
	size_t i;
	ReadForwards<size_t>(&i, sizeof(size_t));
	endianness = GetLocalEndianess();
	if (i != 1)
	{
		if (endianness == BIG_ENDIAN)
		{
			endianness = LITTLE_ENDIAN;
		}
		else
		{
			endianness = BIG_ENDIAN;
		}
	}
}

const char* BytePacker::ReadString() const
{
	if (GetReadableBytes() < 1 || buffer == nullptr)
	{
		return nullptr;
	}
	byte_t c;
	size_t size = Read<byte_t>(&c, sizeof(byte_t));
	GUARANTEE_OR_DIE(size == sizeof(byte_t), "Read byte in byte packer does NOT equal the size of a byte_t!");

	if (c == 0xff)
	{
		return nullptr;
	}
	else
	{
		char* buf = GetHead() - 1;
		size_t max_size = GetReadableBytes() + 1;
		size_t len = 0;
		char* e = buf;
		while ((e != nullptr) && (len < max_size))
		{
			++len;
			++e;
		}
		if (len < max_size)
		{
			AdvanceRead(len);
			return buf;
		}
		else
		{
			return nullptr;
		}
	}
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//Advance
size_t BytePacker::Advance(size_t amt)
{
	_offset += amt;
	return _offset;
}

size_t BytePacker::AdvanceRead(size_t amt) const
{
	_offset += amt;
	return _offset;
}

void BytePacker::SetOffset(size_t offset)
{
	_offset = offset;
}

void BytePacker::ResetOffset() const
{
	_offset = 0;
}

void BytePacker::SetDeleteMyData(bool deleteMe)
{
	deleteMydata = deleteMe;
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//Endianness
void BytePacker::SetEndianness(eEndianess endian)
{
	endianness = endian;
}

eEndianess BytePacker::GetEndianness() const
{
	return endianness;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//Readable Bytes
void BytePacker::SetReadableBytes(size_t readableByteCount)
{
	read_size_max = readableByteCount;
}

uint16_t BytePacker::GetReadableBytes() const
{
	return (uint16_t)(GetTotalReadableBytes() - _offset);
}

uint16_t BytePacker::GetTotalReadableBytes() const
{
	return (uint16_t)Max::Max<uint16_t>((uint16_t)_offset, (uint16_t)read_size_max);
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//Writable Bytes
uint16_t BytePacker::GetWriteableBytes() const
{
	return (uint16_t)(GetTotalWriteableBytes() - _offset);
}

uint16_t BytePacker::GetTotalWriteableBytes() const
{
	return (uint16_t)Max::Max(_offset, write_size_max);
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//Getters
void* BytePacker::GetTail() const
{
	return buffer;
}

char* BytePacker::GetHead() const
{
	return (char*)buffer + _offset;
}

size_t BytePacker::GetOffset() const
{
	return _offset;
}

void BytePacker::ClearData(size_t amt)
{
	char nul = '\0';
	for (size_t i = 0; i < amt && i < read_size_max; i++)
	{
		memcpy(((char*)GetTail()) + i, &nul, 1);
	}
}