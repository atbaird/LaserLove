#include "Engine/2D/SpriteVert.hpp"

const SpriteVert SpriteVert::s_EmptyVert = SpriteVert();


SpriteVert::SpriteVert(const Vector2& pos, const Rgba& tint, const Vector2& uv)
	: m_pos(pos),
	m_color(tint),
	m_texCoords(uv)
{

}


const bool SpriteVert::operator==(const SpriteVert& other) const
{
	if (other.m_pos == m_pos
		&& other.m_color == m_color
		&& other.m_texCoords == m_texCoords)
	{
		return true;
	}
	return false;
}
const bool SpriteVert::operator!=(const SpriteVert& other) const
{
	if (other.m_pos != m_pos
		|| other.m_color != m_color
		|| other.m_texCoords != m_texCoords)
	{
		return true;
	}
	return false;
}