#pragma once
#include "Engine/Renderer/SpriteSheet.hpp"
#include "Engine/Core/StringUtils.hpp"
#include <map>

#ifndef BITMAPFONT_HPP
#define BITMAPFONT_HPP

class BitmapFont
{
public:
	BitmapFont();
	virtual ~BitmapFont();

	static BitmapFont* CreateOrGetFont(const std::string& bitmapFontName, const std::string& folderLoc = "Data/Fonts/%s.png");
	static BitmapFont* CreateOrGetFontWithoutFNT(const std::string& bitmapFontName);
	static void ClearBitfonts();
	const Texture* GetTexture() const;
	const AABB2 GetTexCoordsForGlyph(int glyphUnicode) const;

private:
	BitmapFont(const std::string& bitmapFontName);
	const std::string GetAsLowerCase(const std::string& text) const;
private:
	static std::map< std::string, BitmapFont* >* s_fontRegistry;
	SpriteSheet m_spriteSheet;
	std::string m_fontName;

};

#endif
