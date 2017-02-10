#include "Engine/Renderer/CubeMap.hpp"
#include "Engine/Renderer/OpenGLExtensions.hpp"
#include "Engine/Math/IntVector2.hpp"

//-----------------------------

CubeMapDirs::CubeMapDirs(const std::string& Directory, const std::string& right, const std::string& left,
	const std::string& top, const std::string& bottom, const std::string& front, const std::string& back)
{
	m_filesLocs[0] = Directory + right;		//right
	m_filesLocs[1] = Directory + left;		//left
	m_filesLocs[2] = Directory + top;		//top
	m_filesLocs[3] = Directory + bottom;		//bottom
	m_filesLocs[4] = Directory + front;		//front
	m_filesLocs[5] = Directory + back;	//back
}

CubeMapDirs::CubeMapDirs(const CubeMapDirs& other)
{
	for (size_t index = 0; index < 6; index++)
	{
		m_filesLocs[index] = other.m_filesLocs[index];
	}
}
CubeMapDirs::~CubeMapDirs()
{
}
const bool CubeMapDirs::operator==(const CubeMapDirs& other) const
{
	for (size_t index = 0; index < 6; index++)
	{
		if (m_filesLocs[index] != other.m_filesLocs[index])
		{
			return false;
		}
	}
	return true;
}
const bool CubeMapDirs::operator<(const CubeMapDirs& other) const
{
	for (size_t index = 0; index < 6; index++)
	{
		if (m_filesLocs[index] >= other.m_filesLocs[index])
		{
			return false;
		}
	}
	return true;
}

//------------------------------
STATIC std::map< CubeMapDirs, CubeMap* > s_CubeMapRegistry = std::map<CubeMapDirs, CubeMap*>();

//-----------------------------
CubeMap::CubeMap()
	: m_textureObj(0),
	m_fileNames("", "", "", "", "", "", "")
{
}
CubeMap::CubeMap(const CubeMapDirs& imageLocs)
	: m_textureObj(0),
	m_fileNames(imageLocs)
{
	loadCubemap();
}
CubeMap::CubeMap(const std::string& Directory, const std::string& east, const std::string& west,
	const std::string& north, const std::string& south, const std::string& top, const std::string& bottom)
	: m_textureObj(0),
	m_fileNames(Directory, east, west, north, south, top, bottom)
{
	loadCubemap();
}
CubeMap::~CubeMap()
{
	glDeleteTextures(1, &m_textureObj);
}

const unsigned int CubeMap::loadCubemap()
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	m_textureObj = textureID;
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (unsigned int index = 0; index < 6; index++)
	{
		int numComponents = 0; // Filled in for us to indicate how many color/alpha components the image had (e.g. 3=RGB, 4=RGBA)
		int numComponentsRequested = 0; // don't care; we support 3 (RGB) or 4 (RGBA)
		IntVector2 texelSize;
		unsigned char* imageData = stbi_load(m_fileNames.m_filesLocs[index].c_str(), &texelSize.x, &texelSize.y, &numComponents, numComponentsRequested);

		unsigned int internalFormat = GL_RGBA8; // the format we want the texture to me on the card; allows us to translate into a different texture format as we upload to OpenGL
		unsigned int bufferFormat = GL_RGBA; // the format our source pixel data is currently in; any of: GL_RGB, GL_RGBA, GL_LUMINANCE, GL_LUMINANCE_ALPHA, ...
		if (numComponents == 3)
		{
			internalFormat = GL_RGB8; //how tex stored
			bufferFormat = GL_RGB; //how tex read.
		}

		// #FIXME: What happens if numComponents is neither 3 nor 4?

		glTexImage2D(			// Upload this pixel data to our new OpenGL texture
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + index,		// Creating this as a 2d texture
			0,					// Which mipmap level to use as the "root" (0 = the highest-quality, full-res image), if mipmaps are enabled
			internalFormat,		// Type of texel format we want OpenGL to use for this texture internally on the video card
			texelSize.x,		// Texel-width of image; for maximum compatibility, use 2^N + 2^B, where N is some integer in the range [3,10], and B is the border thickness [0,1]
			texelSize.y,		// Texel-height of image; for maximum compatibility, use 2^M + 2^B, where M is some integer in the range [3,10], and B is the border thickness [0,1]
			0,					// Border size, in texels (must be 0 or 1)
			bufferFormat,		// Pixel format describing the composition of the pixel data in buffer
			GL_UNSIGNED_BYTE,	// Pixel color components are unsigned bytes (one byte per color/alpha channel)
			imageData);		// Location of the actual pixel data bytes/buffer
		stbi_image_free(imageData);
	}
	glBindTexture(/*GL_TEXTURE_CUBE_MAP*/ GL_TEXTURE_CUBE_MAP, 0);

	return textureID;
}
CubeMap* CubeMap::CreateOrGetCubeMap(const CubeMapDirs& imageFilePath)
{
	//write code here; // #FIXME: Implement this method yourself, based on the description above
	std::map<CubeMapDirs, CubeMap*>::iterator it;
	it = s_CubeMapRegistry.find(imageFilePath);
	if (it != s_CubeMapRegistry.end())
	{
		return s_CubeMapRegistry.at(imageFilePath);
	}
	else
	{
		s_CubeMapRegistry[imageFilePath] = new CubeMap(imageFilePath);
		return s_CubeMapRegistry.at(imageFilePath);
	}
}