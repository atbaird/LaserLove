#include "Engine/Math/Specialty/Min.hpp"

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//Vector2 
#include "Engine/Math/Vector2.hpp"
template <>
Vector2 Min<Vector2>(Vector2 const& a, Vector2 const& b)
{
	Vector2 toReturn;
	if (a.x < b.x)
	{
		toReturn.x = a.x;
	}
	else
	{
		toReturn.x = b.x;
	}
	if (a.y < b.y)
	{
		toReturn.y = a.y;
	}
	else
	{
		toReturn.y = b.y;
	}
	return toReturn;
}

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//Vector3
#include "Engine/Math/Vector3.hpp"
template<>
Vector3 Min<Vector3>(Vector3 const& a, Vector3 const& b)
{
	Vector3 toReturn;
	if (a.x < b.x)
	{
		toReturn.x = a.x;
	}
	else
	{
		toReturn.x = b.x;
	}
	if (a.y < b.y)
	{
		toReturn.y = a.y;
	}
	else
	{
		toReturn.y = b.y;
	}
	if (a.z < b.z)
	{
		toReturn.z = a.z;
	}
	else
	{
		toReturn.z = b.z;
	}
	return toReturn;
}

//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
//Vector4
#include "Engine/Math/Vector4.hpp"
template<>
Vector4 Min<Vector4>(Vector4 const& a, Vector4 const& b)
{
	Vector4 toReturn;
	if (a.x < b.x)
	{
		toReturn.x = a.x;
	}
	else
	{
		toReturn.x = b.x;
	}
	if (a.y < b.y)
	{
		toReturn.y = a.y;
	}
	else
	{
		toReturn.y = b.y;
	}
	if (a.z < b.z)
	{
		toReturn.z = a.z;
	}
	else
	{
		toReturn.z = b.z;
	}
	if (a.w < b.w)
	{
		toReturn.w = a.w;
	}
	else
	{
		toReturn.w = b.w;
	}
	return toReturn;
}