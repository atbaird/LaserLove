#pragma once

#ifndef UNSIGNEDINTVECTOR4_HPP
#define UNSIGNEDINTVECTOR4_HPP

class UnsignedIntVector4
{
public:
	//variables
	unsigned int x;
	unsigned int y;
	unsigned int z;
	unsigned int w;

	//static variables
	static const UnsignedIntVector4 uintvec4_zeros;
	static const UnsignedIntVector4 uintvec4_ones;

	//Constructors
	UnsignedIntVector4(const UnsignedIntVector4& other);
	UnsignedIntVector4(const unsigned int& _x = 0.f, const unsigned int& _y = 0.f, const unsigned int& _z = 0.f, const unsigned int& _w = 0.f);

	//setters
	void SetXYZW(const unsigned int& _x, const unsigned int& _y, const unsigned int& _z, const unsigned int& _w);

	//getters
	void GetWXYZ(unsigned int& x_out, unsigned int& y_out, unsigned int& z_out, unsigned int& w_out);
	const float GetLength() const;

	//operators
	const UnsignedIntVector4 operator+(const UnsignedIntVector4&) const;
	const UnsignedIntVector4 operator-(const UnsignedIntVector4&) const;
	const float operator*(const UnsignedIntVector4&) const;
	const UnsignedIntVector4 operator*(const int&) const;
	const UnsignedIntVector4 operator/(const int&) const;
	const void operator*=(const int& other);
	const bool operator==(const UnsignedIntVector4&) const;
	const bool operator!=(const UnsignedIntVector4&) const;
	const void operator+=(const UnsignedIntVector4&);
	const void operator-=(const UnsignedIntVector4&);
};
#endif // !UNSIGNEDINTVECTOR4_HPP