#pragma once

#ifndef FILEBINARYWRITER_HPP
#define FILEBINARYWRITER_HPP

#include "Engine/Core/IBinaryWriter.hpp"
#include <stdio.h>

class FileBinaryWriter : public IBinaryWriter
{
public:
	static const void AddConsoleFileBinaryWriterCommand();
	const bool open(const char* filename, const bool& append = false);
	void close();

	virtual const size_t WriteBytes(const void* src, const size_t& num_bytes) override;

	FILE* file_handle;
};
#endif