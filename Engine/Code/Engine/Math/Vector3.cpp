#include "Engine/Math/Vector3.hpp"
#include <cmath>

const Vector3 Vector3::vec3_zeros = Vector3(0.f, 0.f, 0.f);
const Vector3 Vector3::vec3_ones = Vector3(1.f, 1.f, 1.f);
const Vector3 Vector3::vec3_negOnes = Vector3(-1.f, -1.f, -1.f);

//constructors

Vector3::Vector3(const Vector3& other)
: x(other.x)
, y(other.y)
, z(other.z)
{
}

Vector3::Vector3(const Vector2& vec2, const float& _z)
	: x(vec2.x),
	y(vec2.y),
	z(_z)
{
}
Vector3::Vector3(const float& _x, const float& _y, const float& _z)
: x(_x)
, y(_y)
,z(_z)
{
}



//setters
void Vector3::SetXYZ(const float& _x, const float& _y, const float& _z)
{
	x = _x;
	y = _y;
	z = _z;
}

//getters

void Vector3::GetXYZ(float& x_out, float& y_out, float& z_out) const
{
	x_out = x;
	y_out = y;
	z_out = z;
}
const float Vector3::GetLength() const
{
	return (float)sqrt((x * x) + (y * y) + (z * z));
}
const Vector2 Vector3::GetXY() const
{
	return Vector2(x, y);
}

//operations
const float Vector3::normalize()
{
	//returns a Vector2 of this Vector2 but normalized
	//Consider this as "what direction is up" from the Vertex_PCT.
	//can average all the normals of an object's a shape to get the up vector
	//need to interpolate
	float length = (float)sqrt((x * x) + (y * y) + (z * z));
	if (length != 0)
	{
		x /= length;
		y /= length;
		z /= length;
	}
	return length;
}

const void Vector3::floorAll()
{
	x = floor(x);
	y = floor(y);
z = floor(z);
}

const void Vector3::ceilAll()
{
	x = ceil(x);
	y = ceil(y);
	z = ceil(z);
}
const void Vector3::roundAll()
{
	x = round(x);
	y = round(y);
	z = round(z);
}
const void Vector3::absAll()
{
	x = abs(x);
	y = abs(y);
	z = abs(z);
}
const void Vector3::ACosAll()
{
	x = acos(x);
	y = acos(y);
	z = acos(z);
}
const void Vector3::ASinAll()
{
	x = asin(x);
	y = asin(y);
	z = asin(z);
}
const void Vector3::ATanAll()
{
	x = atan(x);
	y = atan(y);
	z = atan(z);
}
const void Vector3::CosAll()
{
	x = cos(x);
	y = cos(y);
	z = cos(z);
}
const void Vector3::SinAll()
{
	x = sin(x);
	y = sin(y);
	z = sin(z);
}
const void Vector3::TanAll()
{
	x = tan(x);
	y = tan(y);
	z = tan(z);
}
const void Vector3::CosFAll()
{
	x = cosf(x);
	y = cosf(y);
	z = cosf(z);
}
const void Vector3::SinFAll()
{
	x = sinf(x);
	y = sinf(y);
	z = sinf(z);
}
const void Vector3::TanFAll()
{
	x = tanf(x);
	y = tanf(y);
	z = tanf(z);
}
const Vector3 Vector3::operator+(const Vector3& other) const
{
	return Vector3(x + other.x, y + other.y, z + other.z);
}
const Vector3 Vector3::operator-(const Vector3& other) const
{
	return Vector3(x - other.x, y - other.y, z - other.z);
}
const Vector3 Vector3::operator*(const float& theFloat) const
{
	return Vector3(x * theFloat, y * theFloat, z * theFloat);
}

const Vector3 Vector3::operator/(const float& theFloat) const
{
	return Vector3(x / theFloat, y / theFloat, z / theFloat);
}

const void Vector3::operator+=(const Vector3& other)
{
	x = x + other.x;
	y = y + other.y;
	z = z + other.z;
}

const void Vector3::operator-=(const Vector3& other)
{
	x = x - other.x;
	y = y - other.y;
	z = z - other.z;
}

const void  Vector3::operator*=(const float& other)
{
	x *= other;
	y *= other;
	z *= other;
}

const bool Vector3::operator==(const Vector3& other) const
{
	if (other.x == x && other.y == y && other.z == z)
	{
		return true;
	}
	return false;
}

const bool Vector3::operator!=(const Vector3& other) const
{
	if (other.x != x || other.y != y || other.z != z)
	{
		return true;
	}
	return false;
}
const bool Vector3::operator<=(const Vector3& other) const
{
	if (x <= other.x && y <= other.y && z <= other.z)
	{
		return true;
	}
	return false;
}
const bool Vector3::operator>=(const Vector3& other) const
{
	if (x >= other.x && y >= other.y && z >= other.z)
	{
		return true;
	}
	return false;
}
const bool Vector3::operator<(const Vector3& other) const
{
	if (x < other.x && y < other.y && z < other.z)
	{
		return true;
	}
	return false;
}
const bool Vector3::operator>(const Vector3& other) const
{
	if (x > other.x && y > other.y && z > other.z)
	{
		return true;
	}
	return false;
}



const Vector3 Vector3::Lerp(const Vector3& v1, const Vector3& v2, const float& time, const float& minTime, const float& maxTime, const float& ignoreWhenLessThan)
{
	if (time > maxTime)
	{
		return v2;
	}
	else if (time < minTime || (minTime == maxTime))
	{
		return v1;
	}
	Vector3 diff = v2 - v1;
	if (abs(abs(diff.GetLength()) <= abs(ignoreWhenLessThan)))
	{
		return v1;
	}

	const float timePerc = ((time - minTime) / (maxTime - minTime));
	return v1 + (diff * timePerc);
}
const Vector3 Vector3::Slerp(const Vector3& v1, const Vector3& v2, const float& time, const float& minTime, const float& maxTime, const float& ignoreWhenLessThan)
{
	//Slerp made in reference to: https://en.wikipedia.org/wiki/Slerp
	if (time > maxTime)
	{
		return v2;
	}
	else if (time < minTime)
	{
		return v1;
	}
	Vector3 diff = v2 - v1;
	if (abs(diff.GetLength()) <= abs(ignoreWhenLessThan))
	{
		return v1;
	}

	Vector3 valCheck1 = v1;
	Vector3 valCheck2 = v2;
	float length1 = valCheck1.normalize();
	float length2 = valCheck2.normalize();

	const float timePerc = ((time - minTime) / (maxTime - minTime));
	const float opTimePerc = 1.f - timePerc;

	float Omega = valCheck1.dotProduct(valCheck2);
	Omega = acos(Omega);
	float sinOmega = sin(Omega);


	Vector3 val1 = Vector3(0.f,0.f,0.f);
	Vector3 val2 = Vector3(0.f, 0.f, 0.f);

	float v1SinCalc = sin(opTimePerc * Omega) / sinOmega;
	float v2SinCalc = sin(timePerc * Omega) / sinOmega;

	val1 = valCheck1 * v1SinCalc * length1;
	val2 = valCheck2 * v2SinCalc * length2;

	Vector3 combo = val1 + val2;
	
	return combo;
}

//special functions
const float Vector3::dotProduct(const Vector3& other) const
{
	return (other.x * x) + (other.y * y) + (other.z * z);
}

const bool Vector3::orthogonalTest(const Vector3& other) const
{
	float test = dotProduct(other);
	if (test == 0)
	{
		return true;
	}
	return false;
}

const Vector3 Vector3::crossProduct(const Vector3& other) const
{
	return Vector3((y * other.z) - (z * other.y), (z * other.x) - (x * other.z), (x * other.y) - (y * other.x));
}
const Vector3 Vector3::projection(const Vector3& other) const
{
	float dotProductQP = (other.x * x) + (other.y * y) + (other.z * z);
	float dotProductPP = (x * x) + (y * y) + (z * z);
	float fraction = dotProductQP / dotProductPP;
	return Vector3(x, y, z) * fraction;
}