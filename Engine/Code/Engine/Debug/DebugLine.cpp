#include "Engine/Debug/DebugLine.hpp"

DebugLine::DebugLine(const Vector3& start, const Vector3& end, const Rgba& startColor, const Rgba& endColor, const float& startTime,
	const float& duration, const bool& depthTest, const float& lineThickness)
	: m_start(start, startColor),
	m_end(end, endColor),
	timeSecondsInstantiated(startTime),
	durationSeconds(duration),
	enableDepthTesting(depthTest),
	m_lineThickness(lineThickness),
	createdThisFrame(true)
{
}

void DebugLine::Render() const
{
	g_Renderer->DepthTestingTrueFalse(enableDepthTesting);
	g_Renderer->setLineSize(m_lineThickness);
	{
		std::vector<Vertex_PCT> line;
		line.resize(2);
		line[0] = m_start;
		line[1] = m_end;
		g_Renderer->DrawVertexArray(line, PRIMITIVE_LINES, nullptr);
	}
}