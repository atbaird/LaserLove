#include "Engine/Debug/DebugRenderring.hpp"

DebugRenderring* g_debugRenderer = nullptr;


const size_t DebugRenderring::s_numOfAdditionalPointsForPointRendering = 12;

//----------------------------
//constructors
DebugRenderring::DebugRenderring()
	: secondsSinceGameStartUp(0.f),
	m_dotSize(1.f)
{
}
DebugRenderring::~DebugRenderring()
{
}

//----------------------------
//add debug object
void DebugRenderring::addDebugPoint(const Vector3& position, const Rgba& color, const float& durationSeconds, const bool& enableDepthTest)
{
	m_debugPoints.resize(m_debugPoints.size() + 1);
	m_debugPoints[m_debugPoints.size() - 1] = DebugPoint(position, color, Vector2(0.f, 0.f), secondsSinceGameStartUp, durationSeconds, enableDepthTest);
}

void DebugRenderring::addDebugLine(const Vector3& startPosition, const Vector3& endPosition, const Rgba& startColor, const Rgba& endColor, const float& duration, const bool& depthTest, const float& lineThickness)
{
	m_debugLines.resize(m_debugLines.size() + 1);
	m_debugLines[m_debugLines.size() - 1] = DebugLine(startPosition, endPosition, startColor, endColor, secondsSinceGameStartUp, duration, depthTest, lineThickness);
}

void DebugRenderring::addDebugArrow(const Vector3& startPosition, const Vector3& endPosition, const Rgba& startColor, const Rgba& endColor, const float& duration, const bool& depthTest, const float& lineThickness)
{
	m_debugArrows.resize(m_debugArrows.size() + 1);
	m_debugArrows[m_debugArrows.size() - 1] = DebugArrow(startPosition, endPosition, startColor, endColor, secondsSinceGameStartUp, duration, depthTest, lineThickness);
}

void DebugRenderring::addDebugAABB3(const Vector3& center, const Vector3& size, const bool& solid, const bool& depthTesting,
	const Rgba& color1, const Rgba& color2, const Rgba& color3, const Rgba& color4, const Rgba& color5,
	const Rgba& color6, const Rgba& color7, const Rgba& color8, const float& duration)
{
	m_debugAABB3s.resize(m_debugAABB3s.size() + 1);
	m_debugAABB3s[m_debugAABB3s.size() - 1] = DebugAABB3(center, size, secondsSinceGameStartUp, duration, color1, color2,
		color3, color4, color5, color6,
		color7, color8, depthTesting, solid);
}

void DebugRenderring::addDebugSphere(const Vector3& center, const float& radius, const float& duration, const bool& depthTest, const Rgba& color)
{
	m_debugSpheres.resize(m_debugSpheres.size() + 1);
	m_debugSpheres[m_debugSpheres.size() - 1] = DebugSphere(center, radius, secondsSinceGameStartUp, duration, depthTest, color);
}

//----------------------------
//Update
void DebugRenderring::Update(float deltaSeconds)
{
	secondsSinceGameStartUp += deltaSeconds;
	for (size_t index = 0; index < m_debugPoints.size(); index++)
	{
		if (m_debugPoints[index].createdThisFrame == false && m_debugPoints[index].durationSeconds >= 0.f)
		{
			float deltaTimeSinceInstantiation = secondsSinceGameStartUp - m_debugPoints[index].timeSecondsInstantiated;
			if (deltaTimeSinceInstantiation >= m_debugPoints[index].durationSeconds)
			{
				m_debugPoints.erase(m_debugPoints.begin() + index);
				index--;
			}
		}
		else if (m_debugPoints[index].createdThisFrame == true)
		{
			m_debugPoints[index].createdThisFrame = false;
		}
	}

	for (size_t index = 0; index < m_debugLines.size(); index++)
	{
		if (m_debugLines[index].createdThisFrame == false && m_debugLines[index].durationSeconds >= 0.f)
		{
			float deltaTimeSinceInstantiation = secondsSinceGameStartUp - m_debugLines[index].timeSecondsInstantiated;
			if (deltaTimeSinceInstantiation >= m_debugLines[index].durationSeconds)
			{
				m_debugLines.erase(m_debugLines.begin() + index);
				index--;
			}
		}
		else if (m_debugLines[index].createdThisFrame == true)
		{
			m_debugLines[index].createdThisFrame = false;
		}
	}

	for (size_t index = 0; index < m_debugArrows.size(); index++)
	{
		if (m_debugArrows[index].createdThisFrame == false && m_debugArrows[index].durationSeconds >= 0.f)
		{
			float deltaTimeSinceInstantiation = secondsSinceGameStartUp - m_debugArrows[index].timeSecondsInstantiated;
			if (deltaTimeSinceInstantiation >= m_debugArrows[index].durationSeconds)
			{
				m_debugArrows.erase(m_debugArrows.begin() + index);
				index--;
			}
			else if (m_debugArrows[index].createdThisFrame == true)
			{
				m_debugArrows[index].createdThisFrame = false;
			}
		}
	}

	for (size_t index = 0; index < m_debugAABB3s.size(); index++)
	{
		if (m_debugAABB3s[index].createdThisFrame == false && m_debugAABB3s[index].durationSeconds >= 0.f)
		{
			float deltaTimeSinceInstantiation = secondsSinceGameStartUp - m_debugAABB3s[index].timeInstantiatedSeconds;
			if (deltaTimeSinceInstantiation >= m_debugAABB3s[index].durationSeconds)
			{
				m_debugAABB3s.erase(m_debugAABB3s.begin() + index);
				index--;
			}
		}
		else if (m_debugAABB3s[index].createdThisFrame == true)
		{
			m_debugAABB3s[index].createdThisFrame = false;
		}
	}

	for (size_t index = 0; index < m_debugSpheres.size(); index++)
	{
		if (m_debugSpheres[index].createdThisFrame == false && m_debugSpheres[index].durationSeconds >= 0.f)
		{
			float deltaTimeSinceInstantiation = secondsSinceGameStartUp - m_debugSpheres[index].timeSecondsInstantiated;
			if (deltaTimeSinceInstantiation >= m_debugSpheres[index].durationSeconds)
			{
				m_debugSpheres.erase(m_debugSpheres.begin() + index);
				index--;
			}
		}
		else if (m_debugSpheres[index].createdThisFrame == true)
		{
			m_debugSpheres[index].createdThisFrame = false;
		}
	}
}

void DebugRenderring::Render() const
{
	RenderDebugLines();
	RenderDebugArrows();
	RenderDebugSpheres();
	RenderDebugAABB3s();
	RenderDebugPoints();
}
void DebugRenderring::RenderDebugPoints() const
{
	g_Renderer->setLineSize(m_dotSize);
	float pointDisplace = 0.1f * m_dotSize;
	for (size_t i = 0; i < m_debugPoints.size(); i++)
	{
		std::vector<Vertex_PCT> points;
		points.resize(s_numOfAdditionalPointsForPointRendering);
		g_Renderer->DepthTestingTrueFalse(m_debugPoints[i].enableDepthTesting);
		Vertex_PCT curVert = (Vertex_PCT)(m_debugPoints[i]);
		size_t curInd = i * s_numOfAdditionalPointsForPointRendering;
		curInd;

		//preinitialize
		points[0] = curVert;
		points[1] = curVert;
		points[2] = curVert;
		points[3] = curVert;
		
		points[4] = curVert;
		points[5] = curVert;
		points[6] = curVert;
		points[7] = curVert;

		points[8] = curVert;
		points[9] = curVert;
		points[10] = curVert;
		points[11] = curVert;

		//change positions
		points[0].m_pos += Vector3(pointDisplace, pointDisplace, pointDisplace);
		points[1].m_pos += Vector3(-pointDisplace, -pointDisplace, -pointDisplace);
		points[2].m_pos += Vector3(pointDisplace, -pointDisplace, pointDisplace);
		points[3].m_pos += Vector3(-pointDisplace, pointDisplace, -pointDisplace);

		points[4].m_pos += Vector3(-pointDisplace, pointDisplace, pointDisplace);
		points[5].m_pos += Vector3(pointDisplace, -pointDisplace, -pointDisplace);
		points[6].m_pos += Vector3(-pointDisplace, -pointDisplace, pointDisplace);
		points[7].m_pos += Vector3(pointDisplace, pointDisplace, -pointDisplace);

		points[8].m_pos += Vector3(0.f, -pointDisplace, 0.f);
		points[9].m_pos += Vector3(0.f, pointDisplace, 0.f);
		points[10].m_pos += Vector3(pointDisplace, 0.f, 0.f);
		points[11].m_pos += Vector3(-pointDisplace, 0.f, 0.f);
		g_Renderer->DrawVertexArray(points, PRIMITIVE_LINES, nullptr);
	}
}

void DebugRenderring::RenderDebugLines() const 
{
	for (size_t i = 0; i < m_debugLines.size(); i++)
	{
		m_debugLines[i].Render();
	}
}

void DebugRenderring::RenderDebugArrows() const
{
	for (size_t i = 0; i < m_debugArrows.size(); i++)
	{
		m_debugArrows[i].Render();
	}
}

void DebugRenderring::RenderDebugAABB3s() const
{
	for (size_t i = 0; i < m_debugAABB3s.size(); i++)
	{
		m_debugAABB3s[i].Render();
	}
}

void DebugRenderring::RenderDebugSpheres() const
{
	for (size_t i = 0; i < m_debugSpheres.size(); i++)
	{
		m_debugSpheres[i].Render();
	}
}

void DebugRenderring::DrawGridAlongXYPlane(const Vector3& bottomLeftCornerPosition, const Vector2& size, const Vector2& lengthWidth, const Rgba& color)
{
	std::vector<Vertex_PCT> majorLines;
	std::vector<Vertex_PCT> lines;

	{
		//Y Lines
		for (int i = 0; i <= size.y; i++)
		{
			if (i % 5 == 0)
			{
				majorLines.resize(majorLines.size() + 2);
				majorLines[majorLines.size() - 2] = Vertex_PCT(Vector3(bottomLeftCornerPosition.x + (i * lengthWidth.x), bottomLeftCornerPosition.y, bottomLeftCornerPosition.z), color);
				majorLines[majorLines.size() - 1] = Vertex_PCT(Vector3(bottomLeftCornerPosition.x + (i * lengthWidth.x), bottomLeftCornerPosition.y + (size.y * lengthWidth.y), bottomLeftCornerPosition.z), color);
			}
			else
			{
				lines.resize(lines.size() + 2);
				lines[lines.size() - 2] = Vertex_PCT(Vector3(bottomLeftCornerPosition.x + (i * lengthWidth.x), bottomLeftCornerPosition.y, bottomLeftCornerPosition.z), color);
				lines[lines.size() - 1] = Vertex_PCT(Vector3(bottomLeftCornerPosition.x + (i * lengthWidth.x), bottomLeftCornerPosition.y + (size.y * lengthWidth.y), bottomLeftCornerPosition.z), color);
			}
		}
		//X Lines
		for (int i = 0; i <= size.x; i++)
		{
			if (i % 5 == 0)
			{
				majorLines.resize(majorLines.size() + 2);
				majorLines[majorLines.size() - 2] = Vertex_PCT(Vector3(bottomLeftCornerPosition.x, bottomLeftCornerPosition.y + (i * lengthWidth.y), bottomLeftCornerPosition.z), color);
				majorLines[majorLines.size() - 1] = Vertex_PCT(Vector3(bottomLeftCornerPosition.x + (size.x * lengthWidth.x), bottomLeftCornerPosition.y + (i * lengthWidth.y), bottomLeftCornerPosition.z), color);
			}
			else
			{
				lines.resize(lines.size() + 2);
				lines[lines.size() - 2] = Vertex_PCT(Vector3(bottomLeftCornerPosition.x, bottomLeftCornerPosition.y + (i * lengthWidth.y), bottomLeftCornerPosition.z), color);
				lines[lines.size() - 1] = Vertex_PCT(Vector3(bottomLeftCornerPosition.x + (size.x * lengthWidth.x), bottomLeftCornerPosition.y + (i * lengthWidth.y), bottomLeftCornerPosition.z), color);
			}
		}
// 		Y lines.
// 				for (int i = 0; i <= size.y; i++)
// 				{
// 					if (i % 5 == 0)
// 					{
// 						majorLines.push_back(Vertex_PCT(Vector3(planeBottomLeftCorner.x + (i * lengthWidth.x), planeBottomLeftCorner.y, planeBottomLeftCorner.z), color));
// 						majorLines.push_back(Vertex_PCT(Vector3(planeBottomLeftCorner.x + (i * lengthWidth.x), planeBottomLeftCorner.y + (size.x * lengthWidth.y), planeBottomLeftCorner.z), color));
// 					}
// 					else
// 					{
// 						lines.push_back(Vertex_PCT(Vector3(planeBottomLeftCorner.x + (i * lengthWidth.x), planeBottomLeftCorner.y, planeBottomLeftCorner.z), color));
// 						lines.push_back(Vertex_PCT(Vector3(planeBottomLeftCorner.x + (i * lengthWidth.x), planeBottomLeftCorner.y + (size.x * lengthWidth.y), planeBottomLeftCorner.z), color));
// 					}
// 				}
// 		
// 		
// 				//X lines
// 				for (int i = 0; i <= size.x; i++)
// 				{
// 					if (i % 5 == 0)
// 					{
// 						majorLines.push_back(Vertex_PCT(Vector3(planeBottomLeftCorner.x, planeBottomLeftCorner.y + (i * lengthWidth.y), planeBottomLeftCorner.z), color));
// 						majorLines.push_back(Vertex_PCT(Vector3(planeBottomLeftCorner.x + (size.x * lengthWidth.x), planeBottomLeftCorner.y + (i * lengthWidth.y), planeBottomLeftCorner.z), color));
// 					}
// 					else
// 					{
// 						lines.push_back(Vertex_PCT(Vector3(planeBottomLeftCorner.x, planeBottomLeftCorner.y + (i * lengthWidth.y), planeBottomLeftCorner.z), color));
// 						lines.push_back(Vertex_PCT(Vector3(planeBottomLeftCorner.x + (size.x * lengthWidth.x), planeBottomLeftCorner.y + (i * lengthWidth.y), planeBottomLeftCorner.z), color));
// 					}
// 				}
	}
	g_Renderer->setLineSize(1.0f);
	g_Renderer->DrawVertexArray(lines, PRIMITIVE_LINES, nullptr);
	g_Renderer->setLineSize(5.0f);
	g_Renderer->DrawVertexArray(majorLines, PRIMITIVE_LINES, nullptr);
}