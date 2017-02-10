#pragma once
#include "Engine/Renderer/Renderer.hpp"

#ifndef DEBUGSPHERE_HPP
#define DEBUGSPHERE_HPP

class DebugSphere
{
public:
	DebugSphere(const Vector3& center = Vector3(0.f,0.f,0.f), const float& radius = 1.f, const float& startTime = 0.f, 
		const float& duration = 0.f, const bool& depthTest = true, Rgba color = Rgba(1.f,1.f,1.f,1.f));

	void Render() const;
private:
	void calcVertices();
	std::vector<Vertex_PCT> vertices;

	Vector3 m_center;
	Rgba m_color;

public:
	float m_radius;
	float timeSecondsInstantiated;
	float durationSeconds;
	bool createdThisFrame;
	bool enableDepthTesting;
};
#endif // !DEBUGSPHERE_HPP
