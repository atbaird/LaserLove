#pragma once
#include "Engine/Math/Vertices/Vertex_PCT.hpp"

#ifndef DEBUGPOINT_HPP
#define DEBUGPOINT_HPP

struct DebugPoint : public Vertex_PCT
{
	DebugPoint(const Vector3& pos = Vector3(0.f, 0.f, 0.f), const Rgba& color = Rgba(1.f, 1.f, 1.f, 1.f), const Vector2& texCoords = Vector2(0.f, 0.f), const float& currentTimeSeconds = 0.f, const float& duration = 1.f, const bool& depthTesting = true);
	DebugPoint(const DebugPoint& other);

	float timeSecondsInstantiated;
	float durationSeconds;
	bool enableDepthTesting;
	bool createdThisFrame;
};

#endif