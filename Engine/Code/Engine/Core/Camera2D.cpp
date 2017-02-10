#include "Engine/Core/Camera2D.hpp"
#include "Engine/Renderer/Renderer.hpp"

//constructors and destructors
Camera2D::Camera2D(const Vector2& position, const bool& isActive)
{
	//constructor
	m_position = position;
	m_active = isActive;
	xy_Angular_Displacement = 0.f;
}
Camera2D::~Camera2D()
{
	//destructor
}

//updates
void Camera2D::CameraUpdate(const float& deltaSeconds, const Vector2& position, const float& angularDisplace)
{
	//updates the camera position
	deltaSeconds;
	m_position = position;
	xy_Angular_Displacement = angularDisplace;
}

const void Camera2D::Render() const
{
	//if the camera's active, it will translate to the camera's position.
	if (m_active == true)
	{
		g_Renderer->Translate(m_position);
		g_Renderer->Rotate(xy_Angular_Displacement);
	}
}

//setters
void Camera2D::ChangePosition(const Vector2& position)
{
	//changes the camera position
	m_position = position;
}
void Camera2D::SetActive(const bool& isActive)
{
	//sets whether or not the camera is active
	m_active = isActive;
}
void Camera2D::SetXYAngularDisplacement(const float& angularDisplace)
{
	//sets the angular displacement of the camera
	xy_Angular_Displacement = angularDisplace;
}

//getters
const float Camera2D::getXYANgularDisplacement() const
{
	//returns the angular displacement of the camera
	return xy_Angular_Displacement;
}