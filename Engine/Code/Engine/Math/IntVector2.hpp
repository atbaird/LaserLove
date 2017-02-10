#pragma once

#ifndef INTVECTOR2_HPP
#define INTVECTOR2_HPP

class IntVector2
{
public:
	//static variables
	static const IntVector2 ivec2_zeros;
	static const IntVector2 ivec2_ones;

	//constructors
	IntVector2(const int& newX = 0, const int& newY = 0);
	IntVector2(const IntVector2& other);

	//getters
	const float getLength() const;


	//operators
	const float normalize();

	const IntVector2 operator+(const IntVector2&) const;
	const IntVector2 operator-(const IntVector2&) const;
	const IntVector2 operator*(const float&) const;
	const IntVector2 operator/(const float&) const;
	const bool operator==(const IntVector2&) const;
	const bool operator!=(const IntVector2&) const;
	const bool operator<(const IntVector2&) const;
	const bool operator>(const IntVector2&) const;
	const bool operator>=(const IntVector2&) const;
	const bool operator<=(const IntVector2&) const;
	const void operator+=(const IntVector2&);
	const void operator-=(const IntVector2&);
	const bool operatorLessThanByLength(const IntVector2&) const;

	//variables
	int x;
	int y;
};


#endif // !INTVECTOR2_HPP