#include "Engine/Math/Vector4.hpp"
#include "Engine//Renderer/Rgba.hpp"
#include <cmath>

//-----------------------------------------------------------------------------------------
//static vars

const Vector4 Vector4::Forward = Vector4(1.f, 0.f, 0.f, 0.f);
const Vector4 Vector4::Up = Vector4(0.f, 0.f, 1.f, 0.f);
const Vector4 Vector4::Right = Vector4(0.f, -1.f, 0.f, 0.f);
const Vector4 Vector4::vec4_zeros = Vector4(0.f, 0.f, 0.f, 0.f);
const Vector4 Vector4::vec4_ones = Vector4(1.f, 1.f, 1.f, 1.f);
const Vector4 Vector4::vec4_defMatRow = Vector4(0.f, 0.f, 0.f, 1.f);
const Vector4 Vector4::vec4_defSkinWeight = Vector4(1.f, 0.f, 0.f, 0.f);

//-----------------------------------------------------------------------------------------
//constructors
Vector4::Vector4(const Vector4& other)
: x(other.x)
, y(other.y)
, z(other.z)
, w(other.w)
{
}

Vector4::Vector4(const Rgba& rgba)
	: x(rgba.Red),
	y(rgba.Green),
	z(rgba.Blue),
	w(rgba.Alpha)
{

}
Vector4::Vector4(const float& _x, const float& _y, const float& _z, const float& _w)
: x(_x)
, y(_y)
, z(_z)
, w(_w)
{
}

//-----------------------------------------------------------------------------------------
//setters
void Vector4::SetWXYZ(const float& _x, const float& _y, const float& _z, const float& _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

//-----------------------------------------------------------------------------------------
//getters
void Vector4::GetWXYZ(float& x_out, float& y_out, float& z_out, float& w_out) const
{
	x_out = x;
	y_out = y;
	z_out = z;
	w_out = w;
}

const Vector3 Vector4::XYZ() const
{
	return Vector3(x, y, z);
}
const float Vector4::GetLength() const
{
	return (float)sqrt((w * w) + (x * x) + (y * y) + (z * z));
}

//-----------------------------------------------------------------------------------------
//operations
const float Vector4::normalize()
{
	//returns a Vector2 of this Vector2 but normalized
	//Consider this as "what direction is up" from the Vertex_PCT.
	//can average all the normals of an object's a shape to get the up vector
	//need to interpolate
	float xSq = x * x;
	float ySq = y * y;
	float zSq = z * z;
	float wSq = w *w;
	float length = sqrt(xSq + ySq + zSq + wSq);
	if (length != 0)
	{
		x = x / length;
		y = y / length;
		z = z / length;
		w = w / length;
	}
	return length;
}

const void Vector4::floorAll()
{
	w = floor(w);
	x = floor(x);
	y = floor(y);
	z = floor(z);
}

const void Vector4::ceilAll()
{
	w = ceil(w);
	x = ceil(x);
	y = ceil(y);
	z = ceil(z);
}
const void Vector4::roundAll()
{
	w = round(w);
	x = round(x);
	y = round(y);
	z = round(z);
}
const void Vector4::absAll()
{
	w = abs(w);
	x = abs(x);
	y = abs(y);
	z = abs(z);
}
const Vector4 Vector4::operator+(const Vector4& other) const
{
	return Vector4(x + other.x, y + other.y, z + other.z, w + other.w);
}
const Vector4 Vector4::operator-(const Vector4& other) const
{
	return Vector4(x - other.x, y - other.y, z - other.z, w - other.w);
}
const float Vector4::operator*(const Vector4& other) const
{
	return (x * other.x) + (y * other.y) + (z * other.z) + (w * other.w);
}

const Vector4 Vector4::operator*(const float& theFloat) const
{
	return Vector4(x * theFloat, y * theFloat, z * theFloat, w * theFloat);
}

const Vector4 Vector4::operator/(const float& theFloat) const
{
	return Vector4(x / theFloat, y / theFloat, z / theFloat, w / theFloat);
}

const void Vector4::operator+=(const Vector4& other)
{
	x = x + other.x;
	y = y + other.y;
	z = z + other.z;
	w = w + other.w;
}

const void Vector4::operator-=(const Vector4& other)
{
	x = x - other.x;
	y = y - other.y;
	z = z - other.z;
	w = w - other.w;
}
const void Vector4::operator*=(const float& other)
{
	x *= other;
	y *= other;
	z *= other;
	w *= other;
}


const bool Vector4::operator==(const Vector4& other) const
{
	if (other.w == w && other.x == x && other.y == y && other.z == z)
	{
		return true;
	}
	return false;
}

const bool Vector4::operator!=(const Vector4& other) const
{
	if (other.w != w || other.x != x || other.y != y || other.z != z)
	{
		return true;
	}
	return false;
}
const bool Vector4::operator<(const Vector4& other) const
{
	if (GetLength() < other.GetLength())
	{
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------------------
//special functions
const float Vector4::dotProduct(const Vector4& other) const
{
	return (other.w * w) + (other.x * x) + (other.y * y) + (other.z * z);
}

const bool Vector4::orthogonalTest(const Vector4& other) const
{
	float test = dotProduct(other);
	if (test == 0)
	{
		return true;
	}
	return false;
}


const Vector4 Vector4::crossProduct(const Vector4& other, const Vector4& other2) const
{
	//take a look at this link: http://www.gamedev.net/topic/456301-cross-product-vector-4d/
	Vector4 toReturn;
	toReturn.x = (y * ((other.z * other2.w) - (other.w * other2.z)))
		- (z * ((other.y * other2.w) - (other.w * other2.y)))
		+ (w * ((other.y * other2.z) - (other.z * other2.y)));
	toReturn.y = (-x * ((other.z * other2.w) - (other.w * other2.z)))
		+ (z * ((other.x * other2.w) - (other.w * other2.x)))
		- (w * ((other.x * other2.z) - (other.z * other.x)));
	toReturn.z = (x * ((other.y * other2.w) - (other.w * other2.y)))
		- (y * ((other.x * other2.w) - (other.w * other2.x)))
		+ (w * ((other.x * other2.y) - (other2.x * other.y)));
	toReturn.w = (-x * ((other.y * other2.z) - (other.z * other2.y)))
		+ (y * ((other.x * other2.z) - (other.z * other2.z)))
		- (z * ( (other.x * other2.y) - (other.y * other2.x)));
	return toReturn;
}

const Vector4 Vector4::projection(const Vector4& other) const
{
	float dotProductQP = (other.w * w) + (other.x * x) + (other.y * y) + (other.z * z);
	float dotProductPP = (w * w) + (x * x) + (y * y) + (z * z);
	float fraction = dotProductQP / dotProductPP;
	return Vector4(w, x, y, z) * fraction;
}