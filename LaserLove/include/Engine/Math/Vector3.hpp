#pragma once

#ifndef VECTOR3_HPP
#define VECTOR3_HPP
#include "Engine/Math/Vector2.hpp"

class Vector3
{
public:
	static const Vector3 vec3_zeros;
	static const Vector3 vec3_ones;
	static const Vector3 vec3_negOnes;

	Vector3(const Vector3& other);
	Vector3(const Vector2& vec2, const float& _z = 0.f);
	Vector3(const float& _x = 0.f, const float& _y = 0.f, const float& _z = 0.f);

	//setters
	void SetXYZ(const float& _x, const float& _y, const float& _z);

	//getters
	void GetXYZ(float& x_out, float& y_out, float& z_out) const;
	const float GetLength() const;
	const Vector2 GetXY() const;

	//operators
	const float normalize();
	const void floorAll();
	const void ceilAll();
	const void roundAll();
	const void absAll();
	const void ACosAll();
	const void ASinAll();
	const void ATanAll();
	const void CosAll();
	const void SinAll();
	const void TanAll();
	const void CosFAll();
	const void SinFAll();
	const void TanFAll();
	const Vector3 operator+(const Vector3&) const;
	const Vector3 operator-(const Vector3&) const;
	const Vector3 operator*(const float&) const;
	const Vector3 operator/(const float&) const;
	const void operator*=(const float&);
	const bool operator==(const Vector3&) const;
	const bool operator!=(const Vector3&) const;
	const bool operator<=(const Vector3&) const;
	const bool operator>=(const Vector3&) const;
	const bool operator<(const Vector3&) const;
	const bool operator>(const Vector3&) const;
	const void operator+=(const Vector3&);
	const void operator-=(const Vector3&);


	static const Vector3 Lerp(const Vector3& v1, const Vector3& v2, const float& time, const float& minTime = 0.f,
		const float& maxTime = 1.f, const float& ignoreWhenLessThan = 0.000001f);
	static const Vector3 Slerp(const Vector3& v1, const Vector3& v2, const float& time, const float& minTime = 0.f,
		const float& maxTime = 1.f, const float& ignoreWhenLessThan = 0.000001f);
	//special functions
	const Vector3 crossProduct(const Vector3&) const;
	const Vector3 projection(const Vector3&) const;
	const float dotProduct(const Vector3& other) const;
	const bool orthogonalTest(const Vector3&) const;

	//variables
	float x;
	float y;
	float z;
};
#endif // !VECTOR3_HPP
