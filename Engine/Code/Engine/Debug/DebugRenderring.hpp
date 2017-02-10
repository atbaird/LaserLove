#pragma once
#include "Engine/Debug/DebugSphere.hpp"
#include "Engine/Debug/DebugAABB3.hpp"
#include "Engine/Debug/DebugArrow.hpp"
#include "Engine/Debug/DebugLine.hpp"
#include "Engine/Debug/DebugPoint.hpp"
#include <vector>

#ifndef DEBUGRENDERRING_HPP
#define DEBUGRENDERRING_HPP

//------------------------------
class DebugRenderring;

//-------------------------------
extern DebugRenderring* g_debugRenderer;

//-------------------------------
class DebugRenderring
{
public:
	//constructors
	DebugRenderring();
	~DebugRenderring();

	//add debug object
	void addDebugPoint(const Vector3& position, const Rgba& color, const float& durationSeconds = -1.f, const bool& enableDepthTest = true);
	void addDebugLine(const Vector3& startPosition = Vector3(0.f,0.f,0.f), const Vector3& endPosition = Vector3(1.f,1.f,1.f),
		const Rgba& startColor = Rgba(1.f, 1.f, 1.f, 1.f), const Rgba& endColor = Rgba(1.f, 1.f, 1.f, 1.f), const float& duration = -1.f, const bool& depthTest = true, const float& lineThickness = 1.f);
	void addDebugArrow(const Vector3& startPosition = Vector3(0.f, 0.f, 0.f), const Vector3& endPosition = Vector3(1.f, 1.f, 1.f),
		const Rgba& startColor = Rgba(1.f, 1.f, 1.f, 1.f), const Rgba& endColor = Rgba(1.f, 1.f, 1.f, 1.f), const float& duration = -1.f, const bool& depthTest = true, const float& lineThickness = 1.f);
	void addDebugAABB3(const Vector3& center, const Vector3& size, const bool& solid, const bool& depthTesting, const Rgba& color1, const Rgba& color2, const Rgba& color3 = Rgba(1.f,1.f,1.f,1.f),
		const Rgba& color4 = Rgba(1.f,1.f,1.f,1.f), const Rgba& color5 = Rgba(1.f,1.f,1.f,1.f),
		const Rgba& color6 = Rgba(1.f, 1.f, 1.f, 1.f), const Rgba& color7 = Rgba(1.f, 1.f, 1.f, 1.f), const Rgba& color8 = Rgba(1.f, 1.f, 1.f, 1.f), const float& duration = -1.f);
	void DebugRenderring::addDebugSphere(const Vector3& center = Vector3(0.f,0.f,0.f), const float& radius = 1.f, const float& duration = -1.f, const bool& depthTest = true, 
		const Rgba& color = Rgba(1.f,1.f,1.f,1.f));


	void DrawGridAlongXYPlane(const Vector3& bottomLeftCornerPosition, const Vector2& gridSize, const Vector2& lengthWidth, const Rgba& color);

	//setters
	void setPointSize(const float& dotSize = 1.f) { m_dotSize = dotSize; };

	//update
	void Update(float deltaSeconds);
	void Render() const;
private:
	void RenderDebugPoints() const;
	void RenderDebugLines() const;
	void RenderDebugArrows() const;
	void RenderDebugAABB3s() const;
	void RenderDebugSpheres() const;

public:
	//getters
	const float GetTimeSecondsSinceStartUp() const { return secondsSinceGameStartUp; };

private:
	float secondsSinceGameStartUp;
	float m_dotSize;
	std::vector<DebugPoint> m_debugPoints;
	std::vector<DebugLine> m_debugLines;
	std::vector<DebugArrow> m_debugArrows;
	std::vector<DebugAABB3> m_debugAABB3s;
	std::vector<DebugSphere> m_debugSpheres;

	static const size_t s_numOfAdditionalPointsForPointRendering;
};
#endif