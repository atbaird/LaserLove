#include "Engine/Math/AABB3.hpp"

const AABB3 AABB3::ZERO_TO_ONE(0.f, 0.f, 0.f, 1.f, 1.f, 1.f);

AABB3::AABB3(float minx, float miny, float minz, float maxx, float maxy, float maxZ)
: mins(minx, miny, minz), maxs(maxx, maxy, maxZ)
{
}
AABB3::AABB3(const AABB3& other)
: mins(other.mins), maxs(other.maxs)
{
}
AABB3::AABB3(const Vector3& initialMins, const Vector3& initialMaxs)
: mins(initialMins), maxs(initialMaxs)
{
}


//mutators
void AABB3::setMinsMaxs(const Vector3& newMins, const Vector3& newMaxs)
{
	mins = newMins;
	maxs = newMaxs;
}

//operations
const bool AABB3::isPointInThis(const Vector3& point) const
{
	Vector3 rightSideUpMins = mins;
	Vector3 rightSideUpMaxs = maxs;
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
	if (rightSideUpMins.z > rightSideUpMaxs.z)
	{
		float temp = rightSideUpMins.z;
		rightSideUpMins.z = rightSideUpMaxs.z;
		rightSideUpMaxs.z = temp;
	}
	if (point.x >= rightSideUpMins.x && point.x <= rightSideUpMaxs.x
		&& point.y >= rightSideUpMins.y && point.y <= rightSideUpMaxs.y
		&& point.z >= rightSideUpMins.z && point.z <= rightSideUpMaxs.z)
	{
		return true;
	}
	return false;
}

const bool AABB3::doesAABB3Overlap(const AABB3& other) const
{
	Vector3 actualParamSize = maxs - mins;
	Vector3 myCentralPosition = (mins + maxs) / 2.f;
	AABB3 combinedAABB3 = AABB3(other.mins - (actualParamSize / 2.f), other.maxs + (actualParamSize / 2.f));
	if (myCentralPosition.x >= combinedAABB3.mins.x && myCentralPosition.x <= combinedAABB3.maxs.x
		&& myCentralPosition.y >= combinedAABB3.mins.y && myCentralPosition.y <= combinedAABB3.maxs.y
		&& myCentralPosition.z >= combinedAABB3.mins.z && myCentralPosition.z <= combinedAABB3.maxs.z)
	{
		return true;
	}
	return false;
}