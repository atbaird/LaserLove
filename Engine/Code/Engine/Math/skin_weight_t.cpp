#include "Engine/Math/skin_weight_t.hpp"

//static variables
const skin_weight_t skin_weight_t_default = skin_weight_t(Vector4::vec4_defSkinWeight, UnsignedIntVector4::uintvec4_zeros);

//constructors
skin_weight_t::skin_weight_t(const Vector4& weight, const UnsignedIntVector4& indices)
	: bone_weights(weight),
	bone_indices(indices)
{

}
skin_weight_t::skin_weight_t(const skin_weight_t& other)
	: bone_weights(other.bone_weights),
	bone_indices(other.bone_indices)
{

}


const bool skin_weight_t::operator==(const skin_weight_t& other) const
{
	if (bone_weights == other.bone_weights
		&& bone_indices == other.bone_indices)
	{
		return true;
	}
	return false;
}
const bool skin_weight_t::operator!=(const skin_weight_t& other) const
{
	if (bone_weights != other.bone_weights
		|| bone_indices != other.bone_indices)
	{
		return true;
	}
	return false;
}