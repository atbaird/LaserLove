#include "Engine/Core/FileUtils.hpp"
#define WIN32_LEAN_AND_MEAN
#include "Engine/Core/StringUtils.hpp"
#include <Windows.h>
#include <io.h>
#include <sstream>

FileUtils::FileUtils()
{

}


bool FileUtils::LoadBinaryFileToBuffer(const std::string& filepath, std::vector<unsigned char>& out_buffer)
{
	FILE* file = nullptr;
	fopen_s(&file, filepath.c_str(), "r");
	if (file == nullptr)
	{
		return false;
	}
	unsigned char cha;
	while (fread(&cha, sizeof(unsigned char), 1, file))
	{
		out_buffer.push_back(cha);
	}
	fclose(file);
	return true;
}

bool FileUtils::SaveBufferToBinaryFile(const std::string& filepath, std::vector<unsigned char>& in_buffer)
{
	filepath;
	in_buffer;
	return false;
}

bool FileUtils::LoadTextFileToBuffer(const std::string& filepath, std::string& out_buffer)
{
	FILE* file = nullptr;
	fopen_s(&file, filepath.c_str(), "r");
	if (file == nullptr)
	{
		return false;
	}
	fseek(file, 0, SEEK_SET);
	unsigned char cha;
	while (fread(&cha, sizeof(unsigned char), 1, file))
	{
		out_buffer += cha;
	}
	fclose(file);
	return true;
}
bool FileUtils::SaveBufferToTextFile(const std::string& filepath, std::string& in_buffer)
{
	FILE* file = nullptr;
	fopen_s(&file, filepath.c_str(), "w");
	if (file == nullptr)
	{
		return false;
	}

	fwrite(in_buffer.c_str(), sizeof(unsigned char), in_buffer.size(), file);
	fclose(file);
	return true;
}

bool FileUtils::CreateDirectoryA(const std::string& directory_name)
{
	directory_name;
	//if ((CreateDirectoryA((LPCSTR)directory_name.c_str(), nullptr) || GetLastError() == ERROR_ALREADY_EXISTS))
	//{
	//	return true;
	//}
	return false;
}

STATIC bool FileUtils::VerifyFileExists(const std::string& fileOrFolder)
{
	//made in reference to: http://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c

	//After testing, it does not appear as though this verision is good enough; it seems selective about what types of files it can read/consider valid.
	struct stat buffer;
	int val = stat(fileOrFolder.c_str(), &buffer);
	return (val == 0);
}



///-----------------------------------------------------------------------------------
// This Snippet of Code was written by Ken Harward
///-----------------------------------------------------------------------------------
const std::vector< std::string > FileUtils::EnumerateFilesInDirectory(const std::string& relativeDirectoryPath, const std::string& filePattern, bool recurseFolders)
{
	std::string	searchPathPattern = relativeDirectoryPath + "/" + filePattern;
	std::vector< std::string > foundFiles;
	std::vector<std::string> folderNames;

	int filesIterated = 0;
	int error = 0;
	struct _finddata_t fileInfo;
	intptr_t searchHandle = _findfirst(searchPathPattern.c_str(), &fileInfo);
	while (searchHandle != -1 && !error)
	{
		filesIterated++;
		std::string fileName = fileInfo.name;
		if (strcmp(fileName.c_str(), ".") == 0 //current directory
			|| strcmp(fileName.c_str(), "..") == 0) //parent directory
		{
			error = _findnext(searchHandle, &fileInfo);
			continue;
		}
		std::ostringstream stringstream;
		stringstream << relativeDirectoryPath.c_str();
		stringstream << "/";
		stringstream << fileName;
		std::string relativePathToFile = stringstream.str();
		bool		isDirectory = fileInfo.attrib & _A_SUBDIR ? true : false;
		bool		isHidden = fileInfo.attrib & _A_HIDDEN ? true : false;

		if (!isDirectory && !isHidden)
			foundFiles.push_back(relativePathToFile);
		else if (recurseFolders && isDirectory && !isHidden)
		{
			folderNames.push_back(relativePathToFile);
		}


		error = _findnext(searchHandle, &fileInfo);
	}
	_findclose(searchHandle);

	if (recurseFolders)
	{
		for (size_t i = 0; i < folderNames.size(); i++)
		{
			std::vector<std::string> children = EnumerateFilesInDirectory(folderNames[i], filePattern, true);
			foundFiles.insert(foundFiles.end(), children.begin(), children.end());
		}
	}

	return foundFiles;
}
//-------------------------------------------------------------------------------

#include <stdio.h>
#include <direct.h>
std::string FileUtils::GetProjectFilePath()
{
	char* path = nullptr;
	size_t size = MAX_PATH;
	path = _getcwd(path, size);

	std::string pathStr = std::string(path);
	pathStr = ReplaceCharInString(pathStr, '\\', '/');

	return pathStr;
}