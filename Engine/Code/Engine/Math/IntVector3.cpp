#include "Engine/Math/IntVector3.hpp"
#include <cmath>

//static variables
const IntVector3 IntVector3::ivec3_zeros = IntVector3(0,0,0);
const IntVector3 IntVector3::ivec3_ones = IntVector3(1,1,1);

//----------------------------------------------------------------------------------------------------------------
//Constructors
IntVector3::IntVector3(const int& _x, const int& _y, const int& _z)
: x(_x), y(_y), z(_z)
{
}
IntVector3::IntVector3(const IntVector3& other)
: x(other.x), y(other.y), z(other.z)
{

}

//----------------------------------------------------------------------------------------------------------------
//getters
const float IntVector3::getLength() const
{
	return sqrt((float)((x*x) + (y*y) + (z * z)));
}


//----------------------------------------------------------------------------------------------------------------
//operators
const IntVector3 IntVector3::operator+(const IntVector3& other) const
{
	return IntVector3(x + other.x, y + other.y, z + other.z);
}
const IntVector3 IntVector3::operator-(const IntVector3& other) const
{
	return IntVector3(x - other.x, y - other.y, z - other.z);
}
const IntVector3 IntVector3::operator*(const float& multiplier) const
{
	return IntVector3((int)floor(x * multiplier), (int)floor(y * multiplier), (int)floor(z * multiplier));
}
const IntVector3 IntVector3::operator/(const float& multiplier) const
{
	return IntVector3((int)floor(x / multiplier), (int)floor(y / multiplier), (int)floor(z / multiplier));
}

const bool IntVector3::operator==(const IntVector3& other) const
{
	if (x == other.x && y == other.y && z == other.z)
	{
		return true;
	}
	return false;
}

const bool IntVector3::operator<(const IntVector3& other) const
{
	if (z < other.z)
	{
		return true;
	}
	else if (z > other.z)
	{
		return false;
	}
	else
	{
		if (y < other.y)
		{
			return true;
		}
		else if (y > other.y)
		{
			return false;
		}
		else
		{
			return (x < other.x);
		}
	}
}
const bool IntVector3::operator!=(const IntVector3& other) const
{
	if (x != other.x || y != other.y || z != other.z)
	{
		return true;
	}
	return false;
}

const void IntVector3::operator+=(const IntVector3& other)
{
	x = x + other.x;
	y = y + other.y;
	z = z + other.z;
}
const void IntVector3::operator-=(const IntVector3& other)
{
	x = x - other.x;
	y = y - other.y;
	z = z - other.z;
}


const bool IntVector3::operatorLessThanByLength(const IntVector3& other) const
{
	float length = getLength();
	float otherLength = other.getLength();
	if (length < otherLength)
	{
		return true;
	}
	else
	{
		return false;
	}
}