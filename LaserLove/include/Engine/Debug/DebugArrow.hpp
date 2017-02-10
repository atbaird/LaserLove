#pragma once
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Math/Vertex_PCT.hpp"

#ifndef DEBUGARROW_HPP
#define DEBUGARROW_HPP

class DebugArrow
{
public:
	DebugArrow(const Vector3& start = Vector3(0.f, 0.f, 0.f), const Vector3& end = Vector3(1.f, 1.f, 1.f),
		const Rgba& startColor = Rgba(1.f, 1.f, 1.f, 1.f), const Rgba& endColor = Rgba(1.f, 1.f, 1.f, 1.f),
		const float& startTime = 0.f, const float& duration = -1.f, const bool& depthTest = true, const float& lineThickness = 1.f);

	void Render() const;

private:
	//const Vertex_PCT CalcNewPosition(const float& pitch, const float& yaw, const float& length) const;
	void setUpSidePoints();

	std::vector<Vertex_PCT> vertices;
	std::vector<Vertex_PCT> verticesArrowHead;
	Vertex_PCT m_start;
	Vertex_PCT m_end;

public:
	float timeSecondsInstantiated;
	float durationSeconds;
	bool enableDepthTesting;
	float m_lineThickness;
	bool createdThisFrame;
};
#endif