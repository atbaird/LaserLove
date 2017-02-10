#include "Engine/Math/Vertex_PCTT.hpp"

//-----------------------------------------------------------
//-----------------------------------------------------------
//Constructors
Vertex_PCTT Vertex_PCTT::s_EmptyVert = Vertex_PCTT();
Vertex_PCTT::Vertex_PCTT(const Vector3& position, const Rgba& color, const Vector2& texCoords,
	const Vector3& tangent, const Vector3& biTangent, const Vector3& normal)
	: m_pos(position),
	m_color(color),
	m_texCoords(texCoords),
	m_tangent(tangent),
	m_biTangent(biTangent),
	m_normal(normal)
{
}
Vertex_PCTT::Vertex_PCTT(const Vertex_PCTT& other)
	: m_pos(other.m_pos),
	m_color(other.m_color),
	m_texCoords(other.m_texCoords),
	m_tangent(other.m_tangent),
	m_biTangent(other.m_biTangent),
	m_normal(other.m_normal)
{
}


//-----------------------------------------------------------
//-----------------------------------------------------------
//Operators
const bool Vertex_PCTT::operator==(const Vertex_PCTT& other) const
{
	if (other.m_biTangent == m_biTangent
		&& other.m_color == m_color
		&& other.m_normal == m_normal
		&& other.m_pos == m_pos
		&& other.m_tangent == m_tangent
		&& other.m_texCoords == m_texCoords)
	{
		return true;
	}
	return false;
}
const bool Vertex_PCTT::operator!=(const Vertex_PCTT& other) const
{
	if (other.m_biTangent != m_biTangent
		|| other.m_color != m_color
		|| other.m_normal != m_normal
		|| other.m_pos != m_pos
		|| other.m_tangent != m_tangent
		|| other.m_texCoords != m_texCoords)
	{
		return true;
	}
	return false;
}