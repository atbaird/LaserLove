#pragma once

#ifndef VERTEX_PCTT_SKELETON_HPP
#define VERTEX_PCTT_SKELETON_HPP
#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/Vector3.hpp"
#include "Engine/Renderer/Rgba.hpp"
#include "Engine/Math/skin_weight_t.hpp"

struct Vertex_PCTT_Skeleton
{
private:
public:
	static Vertex_PCTT_Skeleton s_EmptyVert;
	//variables
	Vector3 m_pos;
	Rgba m_color;
	Vector2 m_texCoords;
	Vector3 m_tangent;
	Vector3 m_biTangent;
	Vector3 m_normal;
	skin_weight_t m_boneWeightsAndIndicies;

	//constructors
	Vertex_PCTT_Skeleton(const Vector3& pos = Vector3(0.f, 0.f, 0.f), const Rgba& color = Rgba(0.f, 0.f, 0.f, 1.f),
		const Vector2& texCoords = Vector2(0.f, 0.f), const Vector3& tang = Vector3(0.f, 0.f, 0.f), 
		const Vector3& biTang = Vector3(0.f, 0.f, 0.f), const Vector3& norm = Vector3(0.f, 0.f, 0.f),
		const skin_weight_t& weightAndIndices = skin_weight_t(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0,0,0,0)));
	Vertex_PCTT_Skeleton(const Vertex_PCTT_Skeleton& other);
	~Vertex_PCTT_Skeleton();

	const bool operator==(const Vertex_PCTT_Skeleton& other) const;
	const bool operator!=(const Vertex_PCTT_Skeleton& other) const;
};
#endif