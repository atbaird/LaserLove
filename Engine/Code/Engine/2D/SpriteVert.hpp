#pragma once

#ifndef SPRITEVERT_HPP
#define SPRITEVERT_HPP
#include "Engine/Math/Vector2.hpp"
#include "Engine/Renderer/Rgba.hpp"

struct SpriteVert
{
	Vector2 m_pos;
	Vector2 m_texCoords;
	Rgba m_color; //uniform sampler2D gTexDiffuse

	static const SpriteVert s_EmptyVert;

	SpriteVert(const Vector2& pos = Vector2::vec2_zeros, const Rgba& tint = Rgba::s_White, const Vector2& uv = Vector2::vec2_zeros);

	const bool operator==(const SpriteVert& other) const;
	const bool operator!=(const SpriteVert& other) const;
};

#endif