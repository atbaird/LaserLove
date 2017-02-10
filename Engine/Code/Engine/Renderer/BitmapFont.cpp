#include "Engine/Renderer/BitmapFont.hpp"
#include <cstdlib>

std::map< std::string, BitmapFont* >* BitmapFont::s_fontRegistry = nullptr;

BitmapFont::BitmapFont()
	: m_fontName("")
{

}

BitmapFont::BitmapFont(const std::string& bitmapFontName)
	: m_fontName(bitmapFontName),
	m_spriteSheet(bitmapFontName, 16, 16)
{
}
BitmapFont::~BitmapFont()
{

}

STATIC BitmapFont* BitmapFont::CreateOrGetFont(const std::string& bitmapFontName, const std::string& folderLoc)
{
	if (s_fontRegistry == nullptr)
	{
		s_fontRegistry = new std::map<std::string, BitmapFont *>();
	}

	std::string lowerCaseName = bitmapFontName;
	auto foundIter = s_fontRegistry->find(lowerCaseName);
	if (foundIter != s_fontRegistry->end())
		return foundIter->second;//foundIter->seconds;

	//Create and register it
	std::string glyphsTexturePath = Stringf(folderLoc.c_str(), bitmapFontName.c_str());
	BitmapFont* glyphsTexture = new BitmapFont(glyphsTexturePath);
	if (glyphsTexture != nullptr)
	{
		s_fontRegistry->insert(std::pair<std::string, BitmapFont* > (bitmapFontName, new BitmapFont(bitmapFontName)));
		return glyphsTexture;
	}

	if (s_fontRegistry->size() == 0)
	{
		delete s_fontRegistry;
		s_fontRegistry = nullptr;
	}

	return nullptr;
}

STATIC void BitmapFont::ClearBitfonts()
{
	if (s_fontRegistry == nullptr)
	{
		return;
	}
	for (std::map<std::string, BitmapFont*>::iterator it = s_fontRegistry->begin(); it != s_fontRegistry->end(); it++)
	{
		if (it->second != nullptr)
		{
			delete it->second;
			it->second = nullptr;
		}
	}
	delete s_fontRegistry;
	s_fontRegistry = nullptr;
}

const Texture* BitmapFont::GetTexture() const
{
	return m_spriteSheet.getSpriteSheetTexture();
}
const AABB2 BitmapFont::GetTexCoordsForGlyph(int glyphUnicode) const
{
	
	return m_spriteSheet.GetTexCoordsForSpriteIndex(glyphUnicode);
}


const std::string BitmapFont::GetAsLowerCase(const std::string& text) const
{
	return text;
}