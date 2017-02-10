#pragma once
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Renderer/Rgba.hpp"

#ifndef VERTEX_PCTT_HPP
#define VERTEX_PCTT_HPP

struct Vertex_PCTT
{
public:
	static Vertex_PCTT s_EmptyVert;
	//constructors
	Vertex_PCTT(const Vector3& position = Vector3(0.f, 0.f, 0.f), const Rgba& color = Rgba(0.f, 0.f, 0.f, 1.f), const Vector2& texCoords = Vector2(0.f, 0.f),
		const Vector3& tangent = Vector3(0.f, 0.f, 0.f), const Vector3& biTangent = Vector3(0.f, 0.f, 0.f), const Vector3& normal = Vector3(0.f,0.f,0.f));
	Vertex_PCTT(const Vertex_PCTT& other);

	//operators
	const bool operator==(const Vertex_PCTT& other) const;
	const bool operator!=(const Vertex_PCTT& other) const;

	//variables
	Vector3 m_pos;
	Rgba m_color;
	Vector2 m_texCoords;
	Vector3 m_tangent;
	Vector3 m_biTangent;
	Vector3 m_normal;
};
#endif