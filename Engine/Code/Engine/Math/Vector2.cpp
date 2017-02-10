#include "Engine/Math/Vector2.hpp"
#include "Engine/Math/IntVector2.hpp"
#include <cmath>

//this is the correct way to set up a constructor, because its faster to do this instead of calling SetXY
//Can not do this sort of thing for anything except for the constructor.

const Vector2 Vector2::vec2_zeros = Vector2(0.f, 0.f);
const Vector2 Vector2::vec2_ones = Vector2(1.f,1.f);

//-----------------------------------------------
//Copy Constructor
Vector2::Vector2(const Vector2& other) 
	: x(other.x)
	, y(other.y)
{
}

Vector2::Vector2(const IntVector2& iVec2)
	: x((float)iVec2.x),
	y((float)iVec2.y)
{

}

//-----------------------------------------------
Vector2::Vector2(const float& initialX, const float& initialY)
	: x(initialX)
	, y(initialY)
{
}

//-----------------------------------------------
void Vector2::SetXY(const float& newX, const float& newY)
{
	//sets both the x and y values to something different.
	x = newX;
	y = newY;
}

void Vector2::GetXY(float& x_out, float& y_out) const
{
	x_out = x;
	y_out = y;
}
const float Vector2::GetLength() const
{
	return (float)sqrt((x*x) + (y*y));
}

const float Vector2::normalize()
{
	//returns a Vector2 of this Vector2 but normalized
	//Consider this as "what direction is up" from the Vertex_PCT.
	//can average all the normals of an object's a shape to get the up vector
	//need to interpolate
	float length = (float)sqrt((x * x) + (y * y));
	if (length != 0)
	{
		x /= length;
		y /= length;
	}
	return length;
}

const void Vector2::floorAll()
{
	x = floor(x);
	y = floor(y);
}

const void Vector2::ceilAll()
{
	x = ceil(x);
	y = ceil(y);
}
const void Vector2::roundAll()
{
	x = round(x);
	y = round(y);
}

//-----------------------------------------------
const Vector2 Vector2::operator+(const Vector2& other) const
{
	//used when adding two Vector2's together
	return Vector2(x + other.x, y + other.y);
}

//-----------------------------------------------
const Vector2 Vector2::operator-(const Vector2& other) const
{
	//used when subtracting two Vector2's together
	return Vector2(x - other.x, y - other.y);
}
const Vector2 Vector2::operator*(const float& theFloat) const
{
	//used when multiplying a Vector2 by a float
	return Vector2(x * theFloat, y * theFloat);
}

const Vector2 Vector2::operator/(const float& theFloat) const
{
	//used when dividing a Vector2 by a float.
	return Vector2(x / theFloat, y / theFloat);
}

const void Vector2::operator+=(const Vector2& other)
{
	x = x + other.x;
	y = y + other.y;
}
const void Vector2::operator-=(const Vector2& other)
{
	x = x - other.x;
	y = y - other.y;
}
const void Vector2::operator*=(const float& other)
{
	x *= other;
	y *= other;
}


const bool Vector2::operator==(const Vector2& other) const
{
	if (other.x == x && other.y == y)
	{
		return true;
	}
	return false;
}
const bool Vector2::operator!=(const Vector2& other) const
{
	if (other.x != x || other.y != y)
	{
		return true;
	}
	return false;
}


const float Vector2::dotProduct(const Vector2& other) const
{
	//returns the dotProduct of this vector2 and the other
	return (x * other.x) + (y * other.y);
}
const Vector2 Vector2::crossProduct(const Vector2& other) const
{
	//returns the crossProduct of this vector2 and the other.
	return Vector2(x * other.y, (-1 * (y * other.x)));
}

const bool Vector2::orthogonalTest(const Vector2& other) const
{
	//tests whether this and the other vector2 are orthogonal.
	float test = this->dotProduct(other);
	if (test == 0)
	{
		return true;
	}
	return false;

}


const Vector2 Vector2::projection(const Vector2& other) const
{
	//projection of other into direction of this.
	// ((other dotproduct this)/ (this dotproduct this)) * (this)
	float dotProductQP = (other.x * x) + (other.y * y);
	float dotProductPP = (x * x) + (y * y);
	float fraction = dotProductQP / dotProductPP;
	return Vector2(x, y) * fraction;
}