#pragma once
#include <string>
#include <vector>

#ifndef FILEUTILS_HPP
#define FILEUTILS_HPP
#define STATIC

class FileUtils
{
public:
	FileUtils();

	static bool LoadBinaryFileToBuffer(const std::string& filepath, std::vector<unsigned char>& out_buffer);
	static bool SaveBufferToBinaryFile(const std::string& filepath, std::vector<unsigned char>& in_buffer);

	static bool LoadTextFileToBuffer(const std::string& filepath, std::string& out_buffer);
	static bool SaveBufferToTextFile(const std::string& filepath, std::string& in_buffer);

	bool CreateDirectoryA(const std::string& directory_name);

	static bool VerifyFileExists(const std::string& fileOrFolder);

	static const std::vector< std::string > EnumerateFilesInDirectory(const std::string& relativeDirectoryPath, const std::string& filePattern, bool recurseFolders = false);

	static std::string GetProjectFilePath();
};

#endif FILEUTILS_HPP
