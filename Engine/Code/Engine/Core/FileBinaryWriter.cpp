#include "Engine/Core/FileBinaryWriter.hpp"


const void FileBinaryWriter::AddConsoleFileBinaryWriterCommand()
{

}

const bool FileBinaryWriter::open(const char* filename, const bool& append)
{
	const char* mode;
	if (append)
	{
		mode = "ab"; // append binary mode (adds to end of file)
	}
	else
	{
		mode = "wb"; // write binary mode (new file) (overwrites file)
	}

	errno_t error = fopen_s(&file_handle, filename, mode);
	if (error != 0)
	{
		return false;
	}

	return true;
}

void FileBinaryWriter::close()
{
	if (file_handle != nullptr)
	{
		fclose(file_handle);
		file_handle = nullptr;
	}

}

const size_t FileBinaryWriter::WriteBytes(const void* src, const size_t& numbytes)
{
	if (src == NULL || numbytes <= 0)
	{
		return numbytes;
	}
	return fwrite(src, sizeof(byte_t), numbytes, file_handle);
}