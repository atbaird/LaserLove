#include "Engine/Debug/DebugPoint.hpp"


DebugPoint::DebugPoint(const Vector3& pos, const Rgba& color, const Vector2& texCoords, const float& currentTimeSeconds, const float& duration, const bool& depthTesting)
	: Vertex_PCT(pos, color, texCoords),
	timeSecondsInstantiated(currentTimeSeconds),
	durationSeconds(duration),
	enableDepthTesting(depthTesting),
	createdThisFrame(true)
{
}

DebugPoint::DebugPoint(const DebugPoint& other)
	: Vertex_PCT(other.m_pos, other.m_color, other.m_texCoords),
	timeSecondsInstantiated(other.timeSecondsInstantiated),
	durationSeconds(other.durationSeconds),
	enableDepthTesting(other.enableDepthTesting)
{
}