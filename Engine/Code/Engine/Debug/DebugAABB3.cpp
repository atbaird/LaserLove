#include "Engine/Debug/DebugAABB3.hpp"
#include "Engine/Math/Vertices/Vertex_PCT.hpp"


DebugAABB3::DebugAABB3(const Vector3& center, const Vector3& totalLengthXYZ, const float& currentTimeSeconds, const float& duration, const Rgba& color, const bool& depthTesting, const bool& solid)
	: isSolid(solid),
	enableDepthTesting(depthTesting),
	timeInstantiatedSeconds(currentTimeSeconds),
	durationSeconds(duration),
	createdThisFrame(true)
{
	points.resize(8);
	AABB3 aabb = calculateTruePositions(center, totalLengthXYZ);
	plugInColorsToAABB3(aabb, color, color, color, color, color, color, color, color);
}

DebugAABB3::DebugAABB3(const Vector3& center, const Vector3& totalLengthXYZ, const float& currentTimeSeconds, const float& duration, const Rgba& color1, const Rgba& color2,
	const Rgba& color3, const Rgba& color4, const Rgba& color5, const Rgba& color6,
	const Rgba& color7, const Rgba& color8, const bool& depthTesting, const bool& solid)
	: isSolid(solid),
	enableDepthTesting(depthTesting),
	timeInstantiatedSeconds(currentTimeSeconds),
	durationSeconds(duration)
{
	points.resize(8);
	AABB3 aabb = calculateTruePositions(center, totalLengthXYZ);
	plugInColorsToAABB3(aabb, color1, color2, color3, color4, color5, color6, color7, color8);
}

DebugAABB3::DebugAABB3(const AABB3& aabb, const float& currentTimeSeconds, const float& duration, const Rgba& color, const bool& depthTesting, const bool& solid)
	: isSolid(solid),
	enableDepthTesting(depthTesting),
	timeInstantiatedSeconds(currentTimeSeconds),
	durationSeconds(duration)
{
	points.resize(8);
	plugInColorsToAABB3(aabb, color, color, color, color, color, color, color, color);
}

DebugAABB3::DebugAABB3(const AABB3& aabb, const float& currentTimeSeconds, const float& duration, const Rgba& color1, const Rgba& color2,
	const Rgba& color3, const Rgba& color4, const Rgba& color5, const Rgba& color6,
	const Rgba& color7, const Rgba& color8, const bool& depthTesting, const bool& solid)
	: isSolid(solid),
	enableDepthTesting(depthTesting),
	timeInstantiatedSeconds(currentTimeSeconds),
	durationSeconds(duration)
{
	points.resize(8);
	plugInColorsToAABB3(aabb, color1, color2, color3, color4, color5, color6, color7, color8);
}

DebugAABB3::DebugAABB3(const DebugAABB3& other)
	: points(other.points),
	enableDepthTesting(other.enableDepthTesting),
	timeInstantiatedSeconds(other.timeInstantiatedSeconds),
	durationSeconds(other.durationSeconds),
	isSolid(other.isSolid)
{
}


void DebugAABB3::Render() const
{
	g_Renderer->DepthTestingTrueFalse(enableDepthTesting);
	if (isSolid == false)
	{
		DrawLinedPrism();
	}
	else
	{
		DrawSolidPrism();
	}
}


const AABB3 DebugAABB3::calculateTruePositions(const Vector3& center, const Vector3& totalLengthXYZ) const
{
	AABB3 aabb;
	Vector3 halfTotalLength = totalLengthXYZ / 2.f;
	aabb.mins = center - halfTotalLength;
	aabb.maxs = center + halfTotalLength;
	return aabb;
}

void DebugAABB3::plugInColorsToAABB3(const AABB3& aabb, const Rgba& color1, const Rgba& color2, const Rgba& color3, const Rgba& color4, const Rgba& color5, const Rgba& color6, const Rgba& color7, const Rgba& color8)
{
	//bottom
	points[0] = Vertex_PCT(Vector3(aabb.mins.x, aabb.mins.y, aabb.mins.z), color1);
	points[1] = Vertex_PCT(Vector3(aabb.maxs.x, aabb.mins.y, aabb.mins.z), color2);
	points[2] = Vertex_PCT(Vector3(aabb.maxs.x, aabb.maxs.y, aabb.mins.z), color3);
	points[3] = Vertex_PCT(Vector3(aabb.mins.x, aabb.maxs.y, aabb.mins.z), color4);

	//top
	points[4] = Vertex_PCT(Vector3(aabb.mins.x, aabb.mins.y, aabb.maxs.z), color5);
	points[5] = Vertex_PCT(Vector3(aabb.maxs.x, aabb.mins.y, aabb.maxs.z), color6);
	points[6] = Vertex_PCT(Vector3(aabb.maxs.x, aabb.maxs.y, aabb.maxs.z), color7);
	points[7] = Vertex_PCT(Vector3(aabb.mins.x, aabb.maxs.y, aabb.maxs.z), color8);
}
void DebugAABB3::DrawSolidPrism() const
{
	std::vector<Vertex_PCT> quads;

	{
		//bottom
		quads.push_back(points[0]);
		quads.push_back(points[3]);
		quads.push_back(points[2]);
		quads.push_back(points[1]);

		//top
		quads.push_back(points[4]);
		quads.push_back(points[5]);
		quads.push_back(points[6]);
		quads.push_back(points[7]);

		//West
		quads.push_back(points[0]);
		quads.push_back(points[4]);
		quads.push_back(points[7]);
		quads.push_back(points[3]);

		//East
		quads.push_back(points[2]);
		quads.push_back(points[6]);
		quads.push_back(points[5]);
		quads.push_back(points[1]);

		//South
		quads.push_back(points[1]);
		quads.push_back(points[5]);
		quads.push_back(points[4]);
		quads.push_back(points[0]);

		//North
		quads.push_back(points[3]);
		quads.push_back(points[7]);
		quads.push_back(points[6]);
		quads.push_back(points[2]);
	}
	g_Renderer->DrawVertexArray(quads, PRIMITIVE_QUADS, nullptr);
}
void DebugAABB3::DrawLinedPrism() const
{
	std::vector<Vertex_PCT> RectPrism;
	//Bottom
	RectPrism.push_back(points[0]);
	RectPrism.push_back(points[3]);

	RectPrism.push_back(points[3]);
	RectPrism.push_back(points[2]);

	RectPrism.push_back(points[2]);
	RectPrism.push_back(points[1]);

	RectPrism.push_back(points[1]);
	RectPrism.push_back(points[0]);


	//Top
	RectPrism.push_back(points[4]);
	RectPrism.push_back(points[7]);

	RectPrism.push_back(points[7]);
	RectPrism.push_back(points[6]);

	RectPrism.push_back(points[6]);
	RectPrism.push_back(points[5]);

	RectPrism.push_back(points[5]);
	RectPrism.push_back(points[4]);

	//Sides
	RectPrism.push_back(points[0]);
	RectPrism.push_back(points[4]);

	RectPrism.push_back(points[3]);
	RectPrism.push_back(points[7]);

	RectPrism.push_back(points[2]);
	RectPrism.push_back(points[6]);

	RectPrism.push_back(points[1]);
	RectPrism.push_back(points[5]);

	g_Renderer->setLineSize(1.0f);
	g_Renderer->DrawVertexArray(RectPrism, PRIMITIVE_LINES, nullptr);
}