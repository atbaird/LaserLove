#pragma once
#include <string>
#include <map>


#ifndef CUBEMAP_HPP
#define CUBEMAP_HPP

#define STBI_HEADER_FILE_ONLY
#include "stb_image.c"

#define STATIC // Do-nothing indicator that method/member is static in class definition

struct CubeMapDirs
{
public:
	CubeMapDirs(const std::string& Directory = "", const std::string& right = "", const std::string& left = "",
		const std::string& top = "", const std::string& bottom = "", const std::string& front = "", const std::string& back = "");
	CubeMapDirs(const CubeMapDirs& other);
	~CubeMapDirs();

	const bool operator==(const CubeMapDirs& other) const;
	const bool operator<(const CubeMapDirs& other) const;
	std::string m_filesLocs[6];
};

class CubeMap;
extern STATIC std::map< CubeMapDirs, CubeMap* > s_CubeMapRegistry;

class CubeMap
{
public:
	CubeMap();
	CubeMap(const CubeMapDirs& imageLocs);
	CubeMap(const std::string& Directory, const std::string& east, const std::string& west, const std::string& north, const std::string& south, const std::string& top, const std::string& bottom);
	~CubeMap();

	const unsigned int loadCubemap();
	const unsigned int getTextureObj() { return m_textureObj; };
	static CubeMap* CreateOrGetCubeMap(const CubeMapDirs& imageFilePath);

private:
	CubeMapDirs m_fileNames;
	unsigned int m_textureObj;
};
#endif // !CUBEMAP_HPP
