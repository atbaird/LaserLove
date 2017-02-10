#pragma once
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Renderer/Rgba.hpp"

#ifndef Vertex_PCT_HPP
#define Vertex_PCT_HPP

struct Vertex_PCT
{
	static Vertex_PCT s_EmptyVert;
	Vertex_PCT(const Vector3& pos = Vector3(0.f,0.f,0.f), const Rgba& color = Rgba(1.f,1.f,1.f,1.f), const Vector2& texCoords = Vector2(0.f,0.f));
	Vertex_PCT(const Vertex_PCT& other);

	Vector3 m_pos;
	Vector2 m_texCoords;
	Rgba m_color;
};

#endif