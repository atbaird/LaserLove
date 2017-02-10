#pragma once
#include <string>
#include "Engine/Renderer/Texture.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Renderer/Rgba.hpp"
#include "Engine/Math/Vertex_PCT.hpp"
#include <string>
#include <sstream>
#include <vector>
#include <map>

#ifndef FONT_HPP
#define FONT_HPP

class Font;

//#define STATIC // Do-nothing indicator that method/member is static in class definition

extern STATIC std::map< std::string, Font* > s_fontRegistry;

class Glyph
{
public:
	Glyph();
	const AABB2 getPixelCoords() const;
	unsigned char id; //char
	Vector2 pixelStartPosition; //x,y pixel position
	Vector2 size; //width, height
	Vector2 offset; //amountPixelsToDisplaceX, amountPixelsToDisplaceY
	float xAdvancement; //#amountToAdvanceAfterDraw
	int pageNum; //page#
	int chnl; //channel #
};

struct Kerning
{
public:
	Kerning();
	unsigned char firstId; //first char id
	unsigned char secondId; //second char id
	float cursorAdjustment; //amount to displace
};

enum Font_Alignment
{
	Font_LeftHand_Aligned,
	Font_Center_Aligned,
	Font_RightHand_Aligned,
	NUMBER_OF_FONT_ALIGNMENTS
};

class Font
{
public:
	Font(const std::string& fileDirectoryAllAreIn, const std::string& fontFnt = "");
	~Font();

	static Font* GetFontByNameWithDirectoryLoc(const std::string& fontFntWithDirectory);
	static Font* CreateOrGetFont(const std::string& fileDirectory, const std::string& fontFnt);
	static void ClearFonts();
	static void DeleteFont(Font* font);

	const Texture* RequestPage(size_t pageNumber) const;
	const Glyph* GetGlyph(const unsigned char& character) const;
	const float GetKerningAdjustment(const unsigned char& char1, const unsigned char& char2) const;
	const std::vector<std::vector<Vertex_PCT>> GetTextToRender(const Vector3& position, const std::string& toDraw, const float& fontSize = 1.f, const Vector3& direction = Vector3(1.f, 0.f, 0.f), const Vector3& up = Vector3(0.f, 1.f, 0.f),
		const Rgba& color = Rgba(1.f, 1.f, 1.f, 1.f), const Font_Alignment& alignment = Font_LeftHand_Aligned, const bool& enableNewLines = false);
	void RenderText(const Vector3& position, const std::string& toDraw, const float& fontSize = 1.f, const Vector3& direction = Vector3(1.f, 0.f, 0.f), const Vector3& up = Vector3(0.f,1.f, 0.f),
		const Rgba& color = Rgba(1.f, 1.f, 1.f, 1.f), const Font_Alignment& alignment = Font_LeftHand_Aligned, const bool& enableNewLines = false) const;
	void RenderText(const std::vector<std::vector<Vertex_PCT>>& vertices);
	const float CalcTextWidth(const std::string& text, const float& fontSize = 1.f) const;
	const float CalcTextHeight(const std::string& text, const float& fontSize = 1.f) const;
	const int GetPageCount() const;

private:
	void AdjustForAlignment(const Font_Alignment& alignment, const std::string& currentTrueString, const float& fontSize, const Vector3& direction,
		std::vector<unsigned int>& startIndex, std::vector<unsigned int>& endIndex, std::vector<std::vector<Vertex_PCT>>* vertices) const;
	Texture* LoadPageData(const std::string& ftnPage, size_t& curIndexPosition, int& returnedId, const std::string& fileDirectory);
	Glyph* LoadGlyphData(const std::string& ftnPage, size_t& curIndexPosition);
	Kerning* LoadKerningData(const std::string& ftnPage, size_t& curIndexPosition);
	float tallestChar;

private:
	std::string m_fontName;
	std::vector<Texture*> fontPages;
	std::vector<Glyph*> m_glyphs;
	std::vector<Kerning*> m_kernings;

	static const std::string allLetters;
};
#endif