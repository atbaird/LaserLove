#pragma once
#include "Engine/Math/AABB3.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include <vector>

#ifndef DEBUGAABB3_HPP
#define DEBUGAABB3_HPP

class DebugAABB3
{
public:
	DebugAABB3(const Vector3& center = Vector3(0.f,0.f,0.f), const Vector3& totalLengthXYZ = Vector3(1.f,1.f,1.f), const float& currentTimeSeconds = 0.f, const float& duration = 1.f, const Rgba& color = Rgba(1.f,1.f,1.f,1.f), const bool& depthTesting = true,
		const bool& solid = true);
	DebugAABB3(const Vector3& center, const Vector3& totalLengthXYZ, const float& currentTimeSeconds, const float& duration, const Rgba& color1, const Rgba& color2,
		const Rgba& color3 = Rgba(1.f, 1.f, 1.f, 1.f), const Rgba& color4 = Rgba(1.f, 1.f, 1.f, 1.f), const Rgba& color5 = Rgba(1.f, 1.f, 1.f, 1.f), const Rgba& color6 = Rgba(1.f, 1.f, 1.f, 1.f),
		const Rgba& color7 = Rgba(1.f, 1.f, 1.f, 1.f), const Rgba& color8 = Rgba(1.f,1.f,1.f,1.f), const bool& depthTesting = true, const bool& solid = true);
	DebugAABB3(const AABB3& aabb, const float& currentTimeSeconds = 0.f, const float& duration = 1.f, const Rgba& color = Rgba(1.f, 1.f, 1.f, 1.f), const bool& depthTesting = true, const bool& solid = true);
	DebugAABB3(const AABB3& aabb, const float& currentTimeSeconds, const float& duration, const Rgba& color1, const Rgba& color2,
		const Rgba&color3 = Rgba(1.f, 1.f, 1.f, 1.f), const Rgba& color4 = Rgba(1.f, 1.f, 1.f, 1.f), const Rgba& color5 = Rgba(1.f, 1.f, 1.f, 1.f), const Rgba& color6 = Rgba(1.f, 1.f, 1.f, 1.f),
		const Rgba&color7 = Rgba(1.f, 1.f, 1.f, 1.f), const Rgba& color8 = Rgba(1.f, 1.f, 1.f, 1.f), const bool& depthTesting = true, const bool& solid = true);
	DebugAABB3(const DebugAABB3& other);

	void Render() const;

private:
	const AABB3 calculateTruePositions(const Vector3& center, const Vector3& totalLengthXYZ) const;
	void plugInColorsToAABB3(const AABB3& aabb, const Rgba& color1, const Rgba& color2, const Rgba& color3, const Rgba& color4, const Rgba& color5, const Rgba& color6, const Rgba& color7, const Rgba& color8);

	void DrawSolidPrism() const;
	void DrawLinedPrism() const;

public:
	float durationSeconds;
	float timeInstantiatedSeconds;
	bool enableDepthTesting;
	bool isSolid;
	bool createdThisFrame;
	std::vector<Vertex_PCT> points;
};
#endif