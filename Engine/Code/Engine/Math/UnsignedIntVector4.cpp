#include "Engine/Math/UnsignedIntVector4.hpp"
#include <cmath>


//Static variables
const UnsignedIntVector4 UnsignedIntVector4::uintvec4_zeros = UnsignedIntVector4(0, 0, 0, 0);
const UnsignedIntVector4 UnsignedIntVector4::uintvec4_ones = UnsignedIntVector4(1, 1, 1, 1);


//------------------------------------------------------------------------------------
//Constructors
UnsignedIntVector4::UnsignedIntVector4(const UnsignedIntVector4& other)
	: x(other.x),
	y(other.y),
	z(other.z),
	w(other.w)
{

}
UnsignedIntVector4::UnsignedIntVector4(const unsigned int& _x, const unsigned int& _y, const unsigned int& _z, const unsigned int& _w)
	: x(_x),
	y(_y),
	z(_z),
	w(_w)
{

}

//------------------------------------------------------------------------------------
//setters
void UnsignedIntVector4::SetXYZW(const unsigned int& _x, const unsigned int& _y, const unsigned int& _z, const unsigned int& _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

//------------------------------------------------------------------------------------
//getters
void UnsignedIntVector4::GetWXYZ(unsigned int& x_out, unsigned int& y_out, unsigned int& z_out, unsigned int& w_out)
{
	x_out = x;
	y_out = y;
	z_out = z;
	w_out = w;
}
const float UnsignedIntVector4::GetLength() const
{
	float length = sqrt((float)(x*x) + (float)(y*y) + (float)(z*z) + (float)(w*w));
	return length;
}

//------------------------------------------------------------------------------------
//operators
const UnsignedIntVector4 UnsignedIntVector4::operator+(const UnsignedIntVector4& other) const
{
	return UnsignedIntVector4(x + other.x, y + other.y, z + other.z, w + other.w);
}
const UnsignedIntVector4 UnsignedIntVector4::operator-(const UnsignedIntVector4& other) const
{
	return UnsignedIntVector4(x - other.x, y - other.y, z - other.z, w - other.w);
}
const float UnsignedIntVector4::operator*(const UnsignedIntVector4& other) const
{
	return (float)((x * other.x) + (y * other.y) + (z* other.z) + (w * other.w));
}
const UnsignedIntVector4 UnsignedIntVector4::operator*(const int& mult) const
{
	return UnsignedIntVector4(x * mult, y * mult, z * mult, w *mult);
}
const UnsignedIntVector4 UnsignedIntVector4::operator/(const int& div) const
{
	return UnsignedIntVector4(x / div, y / div, z / div, w / div);
}
const void UnsignedIntVector4::operator*=(const int& other)
{
	x *= other;
	y *= other;
	z *= other;
	w *= other;
}
const bool UnsignedIntVector4::operator==(const UnsignedIntVector4& other) const
{
	if (x == other.x && y == other.y && z == other.z && w == other.w)
	{
		return true;
	}
	return false;
}
const bool UnsignedIntVector4::operator!=(const UnsignedIntVector4& other) const
{

	if (x != other.x || y != other.y || z != other.z || w != other.w)
	{
		return true;
	}
	return false;
}
const void UnsignedIntVector4::operator+=(const UnsignedIntVector4& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
	w += other.w;
}
const void UnsignedIntVector4::operator-=(const UnsignedIntVector4& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
	w -= other.w;
}