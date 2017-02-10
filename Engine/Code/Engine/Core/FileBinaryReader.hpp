#pragma once

#ifndef FILEBINARYREADER_HPP
#define FILEBINARYREADER_HPP

#include "Engine/Core/IBinaryReader.hpp"
#include <stdio.h>


class FileBinaryReader : public IBinaryReader
{
public:
	static const void AddConsoleFileBinaryReaderCommand();
	const bool open(const char* filename);
	void close();

	virtual const size_t ReadBytes(void* out, size_t& num_bytes) override;

	FILE* file_handle;
};
#endif // !FILEBINARYREADER_HPP
