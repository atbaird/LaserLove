#include "Engine/Debug/DebugSphere.hpp"
#include "Engine/Math/ExtendedMath.hpp"
#include "Engine/Math/Vertices/Vertex_PCT.hpp"


DebugSphere::DebugSphere(const Vector3& center, const float& radius, const float& startTime, const float& duration, const bool& depthTest, Rgba color)
	: m_center(center),
	m_color(color),
	m_radius(radius),
	timeSecondsInstantiated(startTime),
	durationSeconds(duration),
	enableDepthTesting(depthTest),
	createdThisFrame(true)
{
	calcVertices();
}
void DebugSphere::calcVertices()
{
	for (float pitch = 0.f; pitch < (2 * PI); pitch += (float)(PI / 24.f))
	{
		float z = sin(-pitch) * m_radius;
		float newRadius = m_radius * cos(pitch);
		std::vector<Vertex_PCT> currVertices;
		for (float yaw = 0.f; yaw < (2 * PI); yaw += (float)(PI / 8.f)) {
			float x = (cos(yaw) * newRadius) + m_center.x;
			float y = (sin(yaw) * newRadius) + m_center.y;
			currVertices.resize(currVertices.size() + 1);
			currVertices[currVertices.size() - 1] = (Vertex_PCT(Vector3(x, y, z + m_center.z), m_color));
			if (currVertices.size() > 2)
			{
				currVertices.resize(currVertices.size() + 1);
				currVertices[currVertices.size() - 1] = currVertices[currVertices.size() - 3];
			}
			if ((yaw + (float)(PI / 8.f)) > (2 * PI))
			{
				currVertices.resize(currVertices.size() + 2);
				currVertices[currVertices.size() - 1] = currVertices[currVertices.size() - 3];
				currVertices[currVertices.size() - 2] = currVertices[0];
			}
		}
		for (size_t i = 0; i < currVertices.size(); i++)
		{
			vertices.resize(vertices.size() + 1);
			vertices[vertices.size() - 1] = currVertices[i];
		}
	}

}

void DebugSphere::Render() const
{
	g_Renderer->DepthTestingTrueFalse(enableDepthTesting);
	g_Renderer->setLineSize(1.0f);
	g_Renderer->DrawVertexArray(vertices, PRIMITIVE_LINES, nullptr);
}