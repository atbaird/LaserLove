#pragma once

#ifndef VERTEX_FONT_HPP
#define VERTEX_FONT_HPP
#include "Engine/Math/Vector3.hpp"
#include "Engine/Renderer/Rgba.hpp"

struct Vertex_Font
{
private:
protected:
public:
	//variables
	Rgba m_color = Rgba(1.f,1.f,1.f,1.f);
	Vector3 m_position = Vector3(0.f, 0.f, 0.f);
	Vector2 m_textureCoords = Vector2(0.f, 0.f);
	int m_textureIdx = 0;

	//Constructors
	Vertex_Font();
	virtual ~Vertex_Font();
};
#endif