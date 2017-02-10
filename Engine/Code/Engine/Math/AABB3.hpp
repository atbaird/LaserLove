#pragma once
#include "Engine/Math/Vector3.hpp"

#ifndef AABB3_HPP
#define AABB3_HPP

class AABB3
{
public:
	//static variables
	//use this to store the minimum and maximum corners for the tank.
	static const AABB3 AABB3::ZERO_TO_ONE;

	//constructors
	AABB3(float minx = 0.f, float miny = 0.f, float minz = 0.f, float maxx = 0.f, float maxy = 0.f, float maxZ = 0.f);
	AABB3(const AABB3& other);
	explicit AABB3(const Vector3& initialMins, const Vector3& initialMaxs);

	//mutators
	void setMinsMaxs(const Vector3& newMins, const Vector3& newMaxs);

	//operations
	const bool isPointInThis(const Vector3& point) const;
	const bool doesAABB3Overlap(const AABB3& other) const;

	//variables
	Vector3 mins;
	Vector3 maxs;
};
#endif // !AABB3_HPP
