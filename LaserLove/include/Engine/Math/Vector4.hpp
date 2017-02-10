#pragma once
#include "Engine/Math/Vector3.hpp"

#ifndef VECTOR4_HPP
#define VECTOR4_HPP

class Rgba;

class Vector4
{
public:
	//static variables
	static const Vector4 Forward;
	static const Vector4 Up;
	static const Vector4 Right;
	static const Vector4 vec4_zeros;
	static const Vector4 vec4_ones;
	static const Vector4 vec4_defMatRow;
	static const Vector4 vec4_defSkinWeight;

	Vector4(const Vector4& other);
	Vector4(const Rgba& rgba);
	Vector4(const float& _x = 0.f, const float& _y = 0.f, const float& _z = 0.f, const float& _w = 1.f);

	//setters
	void SetWXYZ(const float& _x, const float& _y, const float& _z, const float& _w);

	//getters
	void GetWXYZ(float& x_out, float& y_out, float& z_out, float& w_out) const;
	const Vector3 XYZ() const;
	const float GetLength() const;

	//operators
	const float normalize();
	const void floorAll();
	const void ceilAll();
	const void roundAll();
	const void absAll();
	const Vector4 operator+(const Vector4&) const;
	const Vector4 operator-(const Vector4&) const;
	const float operator*(const Vector4&) const;
	const Vector4 operator*(const float&) const;
	const Vector4 operator/(const float&) const;
	const void operator*=(const float& other);
	const bool operator==(const Vector4&) const;
	const bool operator!=(const Vector4&) const;
	const bool operator<(const Vector4& other) const;
	const void operator+=(const Vector4&);
	const void operator-=(const Vector4&);

	//special functions
	const Vector4 projection(const Vector4&) const;
	const float dotProduct(const Vector4& other) const;
	const Vector4 crossProduct(const Vector4& other, const Vector4& other2) const;
	const bool orthogonalTest(const Vector4&) const;

	//variables
	float x;
	float y;
	float z;
	float w;

};
#endif // !VECTOR3_HPP
