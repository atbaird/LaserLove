#include "Engine/Math/AABB2.hpp"

const AABB2 AABB2::ZERO_TO_ONE(0.f,0.f,1.f,1.f);
AABB2::AABB2(float minx, float miny, float maxx, float maxy)
: mins(Vector2(minx, miny)), maxs(maxx, maxy)
{
}
AABB2::AABB2(const Vector2& initialMins, const Vector2& initialMaxs)
: mins(initialMins), maxs(initialMaxs)
{
}
AABB2::AABB2(const AABB2& other)
: mins(other.mins), maxs(other.maxs)
{
}

//mutators
void AABB2::setMinsMaxs(const Vector2& newMins, const Vector2& newMaxs)
{
	mins = newMins;
	maxs = newMaxs;
}

//operations
const bool AABB2::isPointInThis(const Vector2& point) const
{
	Vector2 rightSideUpMins = mins;
	Vector2 rightSideUpMaxs = maxs;
	if (rightSideUpMins.x > rightSideUpMaxs.x)
	{
		float temp = rightSideUpMins.x;
		rightSideUpMins.x = rightSideUpMaxs.x;
		rightSideUpMaxs.x = temp;
	}
	if (rightSideUpMins.y > rightSideUpMaxs.y)
	{
		float temp = rightSideUpMins.y;
		rightSideUpMins.y = rightSideUpMaxs.y;
		rightSideUpMaxs.y = temp;
	}
	if (point.x >= rightSideUpMins.x && point.x <= rightSideUpMaxs.x
		&& point.y >= rightSideUpMins.y && point.y <= rightSideUpMaxs.y)
	{
		return true;
	}
	return false;
}