#pragma once
#include "Engine/Math/Vector2.hpp"

#ifndef CAMERA2D_HPP
#define CAMERA2D_HPP

class Camera2D
{
public:

	//Constructors and Destructor
	Camera2D(const Vector2& position, const bool& isActive);
	virtual ~Camera2D();

	//Update
	virtual void CameraUpdate(const float& deltaSeconds, const Vector2& position, const float& angularDisplace); // for following something
	const void Render() const;

	//setters
	void ChangePosition(const Vector2& position);
	void SetActive(const bool& isActive);
	void SetXYAngularDisplacement(const float& angularDisplace);

	//getters
	const float getXYANgularDisplacement() const;

	//variables
	Vector2 m_position;
	Vector2 m_screen_size;
	float xy_Angular_Displacement;
	bool m_active;

};

#endif // !CAMERA2D_HPP
