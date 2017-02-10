#include "Engine/Core/Camera3D.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Math/ExtendedMath.hpp"
#include "Engine/Math/Vector4.hpp"

//static variables
const EulerAngles Camera3D::s_XAxisOnly = EulerAngles(1.f, 0.f, 0.f);
const EulerAngles Camera3D::s_YAxisOnly = EulerAngles(0.f, 1.f, 0.f);
const EulerAngles Camera3D::s_ZAxisOnly = EulerAngles(0.f, 0.f, 1.f);

//constructors
Camera3D::Camera3D(const Vector3& position, const EulerAngles& orientation, const bool& isActive)
: m_position(position), m_orientation(orientation), m_active(isActive)
{
}
Camera3D::~Camera3D()
{
}

//Update
void Camera3D::CameraUpdate(const float& deltaSeconds, const Vector3& position, const EulerAngles& orientation)
{
	deltaSeconds;
	m_position = position;
	m_orientation = orientation;
}
void Camera3D::CameraUpdate(const float& deltaSeconds)
{
	deltaSeconds;
}
const void Camera3D::Render() const
{
	if (m_active == true)
	{
		g_Renderer->Rotate(-m_orientation.m_xRoll, s_XAxisOnly);
		g_Renderer->Rotate(-m_orientation.m_yPitch, s_YAxisOnly);
		g_Renderer->Rotate(-m_orientation.m_zYaw, s_ZAxisOnly);

		g_Renderer->Translate(m_position * -1.f);
	}
}

const void Camera3D::CustomRender(const Vector3& customPosition, const EulerAngles& customOrientation) const
{
	if (m_active == true)
	{
		g_Renderer->Rotate(-customOrientation.m_xRoll, s_XAxisOnly);
		g_Renderer->Rotate(-customOrientation.m_yPitch, s_YAxisOnly);
		g_Renderer->Rotate(-customOrientation.m_zYaw, s_ZAxisOnly);

		g_Renderer->Translate(customPosition * -1.f);
	}
}

const void Camera3D::FixAndClampAngles()
{
	if (m_orientation.m_yPitch > 89.f)
	{
		m_orientation.m_yPitch = 89.f;
	}
	else if (m_orientation.m_yPitch < -89.f)
	{
		m_orientation.m_yPitch = -89.f;
	}
	if (m_orientation.m_zYaw > 180.f)
	{
		m_orientation.m_zYaw = m_orientation.m_zYaw - 360.f;
	}
	else if (m_orientation.m_zYaw < -180.f)
	{
		m_orientation.m_zYaw = m_orientation.m_zYaw + 360.f;
	}
	if (m_orientation.m_xRoll > 180.f)
	{
		m_orientation.m_xRoll = m_orientation.m_xRoll - 360.f;
	}
	else if (m_orientation.m_xRoll < -180.f)
	{
		m_orientation.m_xRoll = m_orientation.m_xRoll + 360.f;
	}
}

//setters
void Camera3D::ChangePosition(const Vector3& position)
{
	m_position = position;
}
void Camera3D::ChangeOrientation(const EulerAngles& orientation)
{
	m_orientation = orientation;
}
void Camera3D::SetActive(const bool& isActive)
{
	m_active = isActive;
}

//getters
const Vector3 Camera3D::GetForwardXYZ() const
{
	Vector3 forwardXYZ = GetForwardXY();
	//Cos(theta) = XYlength/ 1.f; // aka, Cos(theta) = XYLength.
	float cosOrien = CosDegrees(m_orientation.m_yPitch);
	forwardXYZ.x = forwardXYZ.x * cosOrien;
	forwardXYZ.y = forwardXYZ.y * cosOrien;
	forwardXYZ.z = SinDegrees(-m_orientation.m_yPitch);
	return forwardXYZ;
}
const Vector3 Camera3D::GetForwardXY() const
{
	Vector4 forward = (Vector4::Forward * (CosDegrees(m_orientation.m_zYaw))) + (Vector4::Right * -1.f * (SinDegrees(m_orientation.m_zYaw)));
	return forward.XYZ();
}
const Vector3 Camera3D::GetLeftXY() const
{
	Vector3 forwardXY = GetForwardXY();
	return Vector3(-forwardXY.y, forwardXY.x, 0.f);
}