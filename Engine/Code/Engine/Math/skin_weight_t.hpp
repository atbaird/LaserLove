#pragma once

#ifndef SKIN_WEIGHT_T_HPP
#define SKIN_WEIGHT_T_HPP
#include "Engine/Math/Vector4.hpp"
#include "Engine/Math/UnsignedIntVector4.hpp"

struct skin_weight_t
{
	//static variables
	static const skin_weight_t skin_weight_t_default;

	//Constructors
	skin_weight_t(const Vector4& weight = Vector4(1.f, 0.f, 0.f, 0.f), const UnsignedIntVector4& indices = UnsignedIntVector4(0,0,0,0));
	skin_weight_t(const skin_weight_t& other);

	const bool operator==(const skin_weight_t& other) const;
	const bool operator!=(const skin_weight_t& other) const;

	//variables
	Vector4 bone_weights;
	UnsignedIntVector4 bone_indices;
};
#endif