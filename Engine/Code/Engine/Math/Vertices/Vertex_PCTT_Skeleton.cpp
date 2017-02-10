#include "Engine/Math/Vertices/Vertex_PCTT_Skeleton.hpp"
Vertex_PCTT_Skeleton Vertex_PCTT_Skeleton::s_EmptyVert = Vertex_PCTT_Skeleton();

Vertex_PCTT_Skeleton::Vertex_PCTT_Skeleton(const Vector3& pos, const Rgba& color, const Vector2& texCoords, const Vector3& tang,
	const Vector3& biTang, const Vector3& norm, const skin_weight_t& weightAndIndices)
	: m_pos(pos),
	m_color(color),
	m_texCoords(texCoords),
	m_tangent(tang),
	m_biTangent(biTang),
	m_normal(norm),
	m_boneWeightsAndIndicies(weightAndIndices)
{

}
Vertex_PCTT_Skeleton::Vertex_PCTT_Skeleton(const Vertex_PCTT_Skeleton& other)
	: m_pos(other.m_pos),
	m_color(other.m_color),
	m_texCoords(other.m_texCoords),
	m_tangent(other.m_tangent),
	m_biTangent(other.m_biTangent),
	m_normal(other.m_normal),
	m_boneWeightsAndIndicies(other.m_boneWeightsAndIndicies)
{

}
Vertex_PCTT_Skeleton::~Vertex_PCTT_Skeleton()
{
}



const bool Vertex_PCTT_Skeleton::operator==(const Vertex_PCTT_Skeleton& other) const
{
	if (m_pos == other.m_pos
		&& m_color == other.m_color
		&& m_texCoords == other.m_texCoords
		&& m_tangent == other.m_tangent
		&& m_biTangent == other.m_biTangent
		&& m_normal == other.m_normal
		&& m_boneWeightsAndIndicies == other.m_boneWeightsAndIndicies)
	{
		return true;
	}
	return false;
}
const bool Vertex_PCTT_Skeleton::operator!=(const Vertex_PCTT_Skeleton& other) const
{
	if (m_pos != other.m_pos
		|| m_color != other.m_color
		|| m_texCoords != other.m_texCoords
		|| m_tangent != other.m_tangent
		|| m_biTangent != other.m_biTangent
		|| m_normal != other.m_normal
		|| m_boneWeightsAndIndicies != other.m_boneWeightsAndIndicies)
	{
		return true;
	}
	return false;
}