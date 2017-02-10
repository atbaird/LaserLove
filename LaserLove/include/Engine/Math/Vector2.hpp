#pragma once

class IntVector2;
//-------------------------------------------------------------------------------------------
class Vector2{
	//variables
	public:
		float x;
		float y;

	public:
		static const Vector2 vec2_zeros;
		static const Vector2 vec2_ones;

		//constructors
		Vector2(const Vector2&);
		Vector2(const IntVector2&);
		Vector2(const float& x = 0.f, const float& y = 0.f);

		//setters
		void SetXY(const float& x, const float& y);

		//Getters
		void GetXY(float& x_out, float& y_out) const;
		const float GetLength() const;

		//functions
		const float normalize();
		const void floorAll();
		const void ceilAll();
		const void roundAll();
		const Vector2 operator+(const Vector2&) const;
		const Vector2 operator-(const Vector2&) const;
		const Vector2 operator*(const float&) const;
		const Vector2 operator/(const float&) const;
		const void operator*=(const float&);
		const bool operator==(const Vector2&) const;
		const bool operator!=(const Vector2&) const;
		const void operator+=(const Vector2&);
		const void operator-=(const Vector2&);

		//special functions
		const Vector2 crossProduct(const Vector2&) const;
		const Vector2 projection(const Vector2&) const;
		const float dotProduct(const Vector2&) const;
		const bool orthogonalTest(const Vector2&) const;
};
