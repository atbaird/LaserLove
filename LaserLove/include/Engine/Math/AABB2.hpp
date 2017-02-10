#pragma once
#include "Engine/Math/Vector2.hpp"

#ifndef AABB2_HPP
#define AABB2_HPP

class AABB2
{
public:
	//static variables
	//use this to store the minimum and maximum corners for the tank.
	static const AABB2 AABB2::ZERO_TO_ONE;

	//constructors
	AABB2(float minx = 0.f, float miny = 0.f, float maxx = 0.f, float maxy = 0.f);
	AABB2(const AABB2& other);
	explicit AABB2(const Vector2& initialMins, const Vector2& initialMaxs);

	//mutators
	void setMinsMaxs(const Vector2& newMins, const Vector2& newMaxs);

	//operations
	const bool isPointInThis(const Vector2& point) const;
	
	//variables
	Vector2 mins;
	Vector2 maxs;
};

#endif // !AABB2_HPP
