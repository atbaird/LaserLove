#include "Engine/Math/Vertices/Vertex_PCT.hpp"
Vertex_PCT Vertex_PCT::s_EmptyVert = Vertex_PCT();

Vertex_PCT::Vertex_PCT(const Vector3& pos, const Rgba& color, const Vector2& texCoords)
: m_pos(pos),
m_color(color),
m_texCoords(texCoords)
{
}

Vertex_PCT::Vertex_PCT(const Vertex_PCT& other)
	: m_pos(other.m_pos),
	m_color(other.m_color),
	m_texCoords(other.m_texCoords)
{
}