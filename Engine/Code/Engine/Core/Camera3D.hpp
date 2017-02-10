#pragma once
#include "Engine/Math/Vector3.hpp"
#include "Engine/Math/EulerAngles.hpp"

#ifndef CAMERA3D_HPP
#define CAMERA3D_HPP

class Camera3D
{
public:
	//don't wory about GimbleLock
	//switching yaw and pitch = starfox angles. DO NOT USE FOR FIRST PERSON.
	Camera3D(const Vector3& position = Vector3(0.f, 0.f, 0.f), const EulerAngles& orientation = EulerAngles(0.f, 0.f, 0.f), const bool& isActive = false);
	virtual ~Camera3D();

	//Update
	virtual void CameraUpdate(const float& deltaSeconds, const Vector3& position, const EulerAngles& orientation); // for following something
	virtual void CameraUpdate(const float& deltaSeconds);
	const void Render() const;
	const void CustomRender(const Vector3& customPosition, const EulerAngles& customOrientation) const;
	const void FixAndClampAngles();

	//setters
	void ChangePosition(const Vector3& position);
	void ChangeOrientation(const EulerAngles& orientation);
	void SetActive(const bool& isActive);

	//getters
	const Vector3 GetForwardXYZ() const;
	const Vector3 GetForwardXY() const;
	const Vector3 GetLeftXY() const;

	//variables
	Vector3 m_position;
	EulerAngles m_orientation; //This is in DEGREES!
	bool m_active;

	//static variables
	static const EulerAngles s_XAxisOnly;
	static const EulerAngles s_YAxisOnly;
	static const EulerAngles s_ZAxisOnly;
};

#endif // !CAMERA3D_HPP
