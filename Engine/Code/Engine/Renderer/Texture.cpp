//---------------------------------------------------------------------------
// Based on code written by Squirrel Eiserloh
//
#include "Texture.hpp"

#include "Engine/Renderer/OpenGLExtensions.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Core/FileUtils.hpp"
#include <vector>


//---------------------------------------------------------------------------
STATIC std::map< std::string, Texture > s_textureRegistry;

//-----------------------------------------------------------------------------


Texture::Texture()
{
	//default constructor
}

//---------------------------------------------------------------------------
Texture::Texture(const std::string& imageFilePath)
: m_openglTextureID(0)
, m_texelSize(0, 0),
m_imageFilePath(imageFilePath)
{
	//the real constructor
	int numComponents = 0; // Filled in for us to indicate how many color/alpha components the image had (e.g. 3=RGB, 4=RGBA)
	int numComponentsRequested = 0; // don't care; we support 3 (RGB) or 4 (RGBA)
	unsigned char* imageData = stbi_load(imageFilePath.c_str(), &m_texelSize.x, &m_texelSize.y, &numComponents, numComponentsRequested);

	// Enable texturing
	glEnable(GL_TEXTURE_2D);

	// Tell OpenGL that our pixel data is single-byte aligned
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Ask OpenGL for an unused texName (ID number) to use for this texture
	glGenTextures(1, (GLuint*)&m_openglTextureID);

	// Tell OpenGL to bind (set) this as the currently active texture
	glBindTexture(GL_TEXTURE_2D, m_openglTextureID);

	// Set texture clamp vs. wrap (repeat)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); // one of: GL_CLAMP or GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); // one of: GL_CLAMP or GL_REPEAT

	// Set magnification (texel > pixel) and minification (texel < pixel) filters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // one of: GL_NEAREST, GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);  // one of: GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_LINEAR

	bufferFormat = GL_RGBA; // the format our source pixel data is currently in; any of: GL_RGB, GL_RGBA, GL_LUMINANCE, GL_LUMINANCE_ALPHA, ...
	if (numComponents == 3)
	{
		bufferFormat = GL_RGB;
	}
	else if (numComponents == 1)
	{
		bufferFormat = GL_ALPHA;
	}

	// #FIXME: What happens if numComponents is neither 3 nor 4?

	GLenum internalFormat = bufferFormat; // the format we want the texture to me on the card; allows us to translate into a different texture format as we upload to OpenGL

	glTexImage2D(			// Upload this pixel data to our new OpenGL texture
		GL_TEXTURE_2D,		// Creating this as a 2d texture
		0,					// Which mipmap level to use as the "root" (0 = the highest-quality, full-res image), if mipmaps are enabled
		internalFormat,		// Type of texel format we want OpenGL to use for this texture internally on the video card
		m_texelSize.x,		// Texel-width of image; for maximum compatibility, use 2^N + 2^B, where N is some integer in the range [3,10], and B is the border thickness [0,1]
		m_texelSize.y,		// Texel-height of image; for maximum compatibility, use 2^M + 2^B, where M is some integer in the range [3,10], and B is the border thickness [0,1]
		0,					// Border size, in texels (must be 0 or 1)
		bufferFormat,		// Pixel format describing the composition of the pixel data in buffer
		GL_UNSIGNED_BYTE,	// Pixel color components are unsigned bytes (one byte per color/alpha channel)
		imageData);		// Location of the actual pixel data bytes/buffer

	stbi_image_free(imageData);
	glDisable(GL_TEXTURE_2D);
}

Texture::Texture(uint32_t width, uint32_t height, eTextureFormat format)
{
	//GLuint texture_type = GL_TEXTURE_2D;
	glGenTextures(1, (GLuint*)&m_openglTextureID);
	GLenum buffer_channels = GL_RGBA;
	GLenum buffer_format = GL_UNSIGNED_INT_8_8_8_8;
	GLenum internal_format = GL_RGBA8;

	if (format == eTextureFormat_RGBA8)
	{

	}
	else if(format == eTextureFormat_D24S8)
	{
		buffer_channels = GL_DEPTH_STENCIL;
		buffer_format = GL_UNSIGNED_INT_24_8;
		internal_format = GL_DEPTH24_STENCIL8;
	}
	else
	{
		ASSERT_RECOVERABLE(false, "Unsupported texture format");
		return;
	}
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_openglTextureID);
	glTexImage2D(GL_TEXTURE_2D, 
		0, //level - mipmaplevel, set to 0,
		internal_format, //how texture is stored in mem
		width, height,
		0, //border, again set to 0, we want not 0
		buffer_channels, //channels used by image pass in
		buffer_format, //format of data of image passed in
		NULL); //no actual data passed in, defaults black/white
	m_texelSize.x = width;
	m_texelSize.y = height;
	bufferFormat = buffer_format;
}

Texture::~Texture()
{

}

//---------------------------------------------------------------------------
// Returns a pointer to the already-loaded texture of a given image file,
//	or nullptr if no such texture/image has been loaded.
//
STATIC Texture* Texture::GetTextureByName(const std::string& imageFilePath)
{
	//write code here; // #FIXME: Implement this method yourself, based on the description above
	std::map<std::string, Texture>::iterator it;
	it = s_textureRegistry.find(imageFilePath);
	if (it != s_textureRegistry.end())
	{
		return &s_textureRegistry.at(imageFilePath);
	}
	return nullptr;
}


//---------------------------------------------------------------------------
// Finds the named Texture among the registry of those already loaded; if
//	found, returns that Texture*.  If not, attempts to load that texture,
//	and returns a Texture* just created (or nullptr if unable to load file).
//
STATIC Texture* Texture::CreateOrGetTexture(const std::string& imageFilePath)
{
	//write code here; // #FIXME: Implement this method yourself, based on the description above
	std::map<std::string, Texture>::iterator it;
	it = s_textureRegistry.find(imageFilePath);
	if (it != s_textureRegistry.end())
	{
		return &s_textureRegistry.at(imageFilePath);
	}
	else 
	{
		if (FileUtils::VerifyFileExists(imageFilePath))
		{
			s_textureRegistry[imageFilePath] = Texture(imageFilePath);
			return &s_textureRegistry.at(imageFilePath);

		}
		else
		{
			return nullptr;
		}
	}
}

STATIC const bool Texture::DeleteTexture(Texture*& tex)
{
	for (std::map < std::string, Texture>::iterator it = s_textureRegistry.begin(); it != s_textureRegistry.end(); it++)
	{
		if (&it->second == tex)
		{
			std::vector<GLuint> arr;
			arr.reserve(1);
			arr.push_back(it->second.m_openglTextureID);
			glDeleteTextures(arr.size(), &arr[0]);
			s_textureRegistry.erase(it);
			delete tex;
			tex = nullptr;
			return true;
		}
	}
	return false;
}

STATIC void Texture::DeleteAllTextures()
{
	std::vector<GLuint> arr;
	arr.reserve(s_textureRegistry.size());
	for (std::map<std::string, Texture>::iterator it = s_textureRegistry.begin(); it != s_textureRegistry.end(); it++)
	{
		arr.push_back((it->second.m_openglTextureID));
	}
	if (!arr.empty())
	{
		glDeleteTextures(arr.size(), &arr[0]);
	}
	s_textureRegistry.clear();
}