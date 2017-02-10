#include "Engine/Core/FileBinaryReader.hpp"


const void FileBinaryReader::AddConsoleFileBinaryReaderCommand()
{

}

const bool FileBinaryReader::open(const char* filename)
{
	const char* mode;
	mode = "rb";

	errno_t error = fopen_s(&file_handle, filename, mode);
	if (error != 0)
	{
		return false;
	}

	return true;
}

void FileBinaryReader::close()
{
	if (file_handle != nullptr)
	{
		fclose(file_handle);
		file_handle = nullptr;
	}

}

const size_t FileBinaryReader::ReadBytes(void* out, size_t& num_bytes)
{
	if (num_bytes == 0)
	{
		return 0;
	}
	return fread(out, sizeof(byte_t), num_bytes, file_handle);
}