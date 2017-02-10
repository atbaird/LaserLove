#pragma once
#include <string>

#ifndef RGBA_HPP
#define RGBA_HPP
#include "Engine/Math/Vector3.hpp"

#define STATIC
class Rgba
{
public:
	//Static Variables
	static const Rgba s_White;
	static const Rgba s_Black;
	static const Rgba s_Blue;
	static const Rgba s_Gray;
	static const Rgba s_Gray6;
	static const Rgba s_lightGray;
	static const Rgba s_Red;
	static const Rgba s_Green;
	static const Rgba s_Yellow;

	//variables
	float Red;
	float Green;
	float Blue;
	float Alpha;

	//constructors
	Rgba(const float& red = 0.0f, const float& green = 0.0f, const float& blue = 0.0f, const float& alpha = 1.0f);
	Rgba(const Vector3& rgb, const float& alpha = 1.0f);

	//setters
	void SetRGBA(const float& in_r, const float& in_g, const float& in_b, const float& in_a);
	void SetRGB(const float& in_r, const float& in_g, const float& in_b);
	void setAlpha(const float& in_a);

	//getters
	void GetRGBA(float& out_r, float& out_g, float& out_b, float& out_a) const;
	const std::string ToString() const;
	const Vector3 GetRGB() const;

	//operators
	const bool operator==(const Rgba& other) const;
	const bool operator!=(const Rgba& other) const;
	const Rgba operator-(const Rgba& other) const;
	const Rgba operator+(const Rgba& other) const;
	const Rgba operator*(const float& mult) const;
	const Rgba operator/(const float& div) const;

	//Static Functions
	static int ConvertPercentageFloatTo255Int(float percentage);
	static float Convert255IntToPercentageFloat(int val);
	static int ClampedConvertPercentageFloatTo255Int(float percentage);
	static float ClampedConvert255IntToPercentageFloat(int val);

};


Rgba ReadColorOutOfString(const std::string& str);

#endif // !RGBA_HPP
