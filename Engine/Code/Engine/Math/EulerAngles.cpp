#include "Engine/Math/EulerAngles.hpp"
#include "Engine/Math/Vector4.hpp"
#include "Engine/Math/ExtendedMath.hpp"

//Static Variables
const EulerAngles EulerAngles::eulerAng_Default = EulerAngles(0.f, 0.f, 0.f);


//constructors
EulerAngles::EulerAngles(const EulerAngles& other)
: m_xRoll(other.m_xRoll), m_yPitch(other.m_yPitch), m_zYaw(other.m_zYaw)
{

}
EulerAngles::EulerAngles(const float& roll, const float& pitch, const float& yaw)
: m_xRoll(roll), m_yPitch(pitch), m_zYaw(yaw)
{
}
EulerAngles::~EulerAngles()
{
}

void EulerAngles::setAllAngles(const float& roll, const float& pitch, const float& yaw)
{
	m_xRoll = roll;
	m_yPitch = pitch;
	m_zYaw = yaw;
}

//operators
const EulerAngles EulerAngles::operator+(const EulerAngles& other) const
{
	return EulerAngles(m_xRoll + other.m_xRoll, m_yPitch + other.m_yPitch, m_zYaw + other.m_zYaw);
}
const EulerAngles EulerAngles::operator-(const EulerAngles& other) const
{
	return EulerAngles(m_xRoll - other.m_xRoll, m_yPitch - other.m_yPitch, m_zYaw - other.m_zYaw);
}

const EulerAngles EulerAngles::operator*(const float& multiplier) const
{
	return EulerAngles(m_xRoll * multiplier, m_yPitch * multiplier, m_zYaw * multiplier);
}

const EulerAngles EulerAngles::operator/(const float& multiplier) const
{
	return EulerAngles(m_xRoll / multiplier, m_yPitch / multiplier, m_zYaw / multiplier);

}

const bool EulerAngles::operator==(const EulerAngles& other) const
{
	if (m_xRoll == other.m_xRoll && m_yPitch == other.m_yPitch && m_zYaw == other.m_zYaw)
	{
		return true;
	}
	return false;
}
const bool EulerAngles::operator!=(const EulerAngles& other) const
{
	if (m_xRoll != other.m_xRoll || m_yPitch != other.m_yPitch || m_zYaw != other.m_zYaw)
	{
		return true;
	}
	return false;
}
const void EulerAngles::operator+=(const EulerAngles& other)
{
	m_xRoll = m_xRoll + other.m_xRoll;
	m_yPitch = m_yPitch + other.m_yPitch;
	m_zYaw = m_zYaw + other.m_zYaw;
}
const void EulerAngles::operator-=(const EulerAngles& other)
{
	m_xRoll = m_xRoll - other.m_xRoll;
	m_yPitch = m_yPitch - other.m_yPitch;
	m_zYaw = m_zYaw - other.m_zYaw;
}


const Vector3 EulerAngles::GetForwardXYZ() const
{
	Vector3 forwardXYZ = GetForwardXY();
	//Cos(theta) = XYlength/ 1.f; // aka, Cos(theta) = XYLength.
	float cosOrien = CosDegrees(m_yPitch);
	forwardXYZ.x = forwardXYZ.x * cosOrien;
	forwardXYZ.y = forwardXYZ.y * cosOrien;
	forwardXYZ.z = SinDegrees(-m_yPitch);
	return forwardXYZ;
}
const Vector3 EulerAngles::GetForwardXY() const
{
	Vector4 forward = (Vector4::Forward * (CosDegrees(m_zYaw))) + (Vector4::Right * -1.f * (SinDegrees(m_zYaw)));
	return forward.XYZ();
}
const Vector3 EulerAngles::GetLeftXY() const
{
	Vector3 forwardXY = GetForwardXY();
	return Vector3(-forwardXY.y, forwardXY.x, 0.f);
}