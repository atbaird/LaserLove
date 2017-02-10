#pragma once
#include <string>
#include <map>
#include "Engine/Math/IntVector2.hpp"

#define STBI_HEADER_FILE_ONLY
#include "stb_image.c"

#define STATIC // Do-nothing indicator that method/member is static in class definition

class Texture;
extern STATIC std::map< std::string, Texture > s_textureRegistry;

enum eTextureFormat
{
	eTextureFormat_RGBA8, //RGBA, 8 bits per channel
	eTextureFormat_D24S8 //Depth 24, Stencil 8
};

class Texture
{
public:
	//constructors
	Texture(); //DO NOT USE THIS FOR CREATING NEW TEXTURES!!! SPECIFICALLY FOR running internal methods.
	Texture(const std::string& imageFilePath);
	Texture(uint32_t width, uint32_t height, eTextureFormat format);
	~Texture();

	//getters
	static Texture* GetTextureByName(const std::string& imageFilePath);
	static Texture* CreateOrGetTexture(const std::string& imageFilePath);
	static const bool DeleteTexture(Texture*& tex);
	static void DeleteAllTextures();

	//variables
	int m_openglTextureID;
	unsigned int bufferFormat;
	IntVector2 m_texelSize;
	std::string m_imageFilePath;

	// (0,0) at top left, (1,1) at bottom right.
};