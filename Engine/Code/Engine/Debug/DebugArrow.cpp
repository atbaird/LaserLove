#include "Engine/Debug/DebugArrow.hpp"
#include "Engine/Math/ExtendedMath.hpp"

DebugArrow::DebugArrow(const Vector3& start, const Vector3& end, const Rgba& startColor, const Rgba& endColor,
	const float& startTime, const float& duration, const bool& depthTest, const float& lineThickness)
	: m_start(start, startColor),
	m_end(end, endColor),
	timeSecondsInstantiated(startTime),
	durationSeconds(duration),
	enableDepthTesting(depthTest),
	m_lineThickness(lineThickness),
	createdThisFrame(true)
{
	setUpSidePoints();
}

void DebugArrow::Render() const 
{
	g_Renderer->DepthTestingTrueFalse(enableDepthTesting);
	g_Renderer->setLineSize(m_lineThickness);
	g_Renderer->DrawVertexArray(vertices, PRIMITIVE_LINES, nullptr);
	g_Renderer->setLineSize((m_lineThickness * 1.5f) + 3.f);
	g_Renderer->DrawVertexArray(verticesArrowHead, PRIMITIVE_LINES, nullptr);
}
// 
// const Vertex_PCT DebugArrow::CalcNewPosition(const float& pitch, const float& yaw, const float& length) const
// {
// 	//Only for arrow edge points.
// 	/*
// 	float cosYaw = CosDegrees(m_orientation.m_zYaw);
// 	float sinYaw = SinDegrees(m_orientation.m_zYaw);
// 	return Vector3(cosYaw, sinYaw, 0.f);
// 	----
// 	Vector3 forwardXYZ = GetForwardXY();
// 	//Cos(theta) = XYlength/ 1.f; // aka, Cos(theta) = XYLength.
// 	float cosOrien = CosDegrees(m_orientation.m_yPitch);
// 	forwardXYZ.x = forwardXYZ.x * cosOrien;
// 	forwardXYZ.y = forwardXYZ.y * cosOrien;
// 	forwardXYZ.z = SinDegrees(-m_orientation.m_yPitch);
// 	*/
// 	Vertex_PCT newPoint = m_end;
// 	newPoint.m_pos = Vector3(0.f, 0.f, 0.f);
// 	newPoint.m_pos.x = CosDegrees(yaw);
// 	newPoint.m_pos.y = SinDegrees(yaw);
// 	newPoint.m_pos.z = 0.f;
// 	float cosOrien = CosDegrees(pitch);
// 	newPoint.m_pos.x = newPoint.m_pos.x * cosOrien;
// 	newPoint.m_pos.y = newPoint.m_pos.y * cosOrien;
// 	newPoint.m_pos.z = SinDegrees(-pitch);
// 	newPoint.m_pos = newPoint.m_pos * -1.f;
// 
// 	newPoint.m_pos = m_end.m_pos + (newPoint.m_pos * (length * 0.1f));
// 
// 	return newPoint;
// }
void DebugArrow::setUpSidePoints()
{
 	Vector3 difference = m_end.m_pos - m_start.m_pos;
	Vector3 normal = difference;
	float length = normal.normalize();
	length;
	vertices.clear();
	verticesArrowHead.clear();
	vertices.resize(2);
	verticesArrowHead.resize(2);
	vertices[0] = m_start;
	vertices[1] = m_end;
	vertices[0].m_color.Alpha = m_start.m_color.Alpha / 2.f;
	vertices[1].m_color.Alpha = m_end.m_color.Alpha / 2.f;

	verticesArrowHead[0] = m_start;
	verticesArrowHead[0].m_color = m_end.m_color;
	verticesArrowHead[0].m_pos += (difference * 0.9f);
	verticesArrowHead[1] = m_end;

// 	vertices.resize(10);
// 	for (size_t i = 0; i < vertices.size(); i++)
// 	{
// 		vertices[i] = m_end;
// 	}
// 
// 	float pitch = -ASinDegrees(normal.z);
// 	float cosOrien = CosDegrees(pitch);
// 	float yaw = normal.x / cosOrien;
// 	yaw = ACosDegrees(yaw);
// 
// 	float leftYaw = yaw + 30.f;
// 	float rightYaw = yaw - 30.f;
// 	float upPitch = pitch + 30.f;
// 	float downPitch = pitch - 30.f;
// 
// 	Vertex_PCT left = CalcNewPosition(pitch, leftYaw, length);
// 	Vertex_PCT right = CalcNewPosition(pitch, rightYaw, length);
// 	Vertex_PCT down = CalcNewPosition(upPitch, yaw, length);
// 	Vertex_PCT up = CalcNewPosition(downPitch, yaw, length);
// 	//newPoint.m_pos = m_end.m_pos - (newPoint.m_pos * (length * 0.1f));
// 
// 	vertices[0] = m_start;
// 	vertices[1] = m_end;
// 	vertices[2] = m_end;
// 	vertices[3] = left;
// 	vertices[4] = m_end;
// 	vertices[5] = right;
// 	vertices[6] = m_end;
// 	vertices[7] = down;
// 	vertices[8] = m_end;
// 	vertices[9] = up;
}