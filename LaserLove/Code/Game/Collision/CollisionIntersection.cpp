#include "Game/Collision/CollisionIntersection.hpp"


CollisionIntersection::CollisionIntersection()
	: m_intersected(false),
	m_other(nullptr),
	m_owner(nullptr),
	m_otherPosition(Vector2::vec2_zeros),
	m_otherRadius(Vector2::vec2_zeros),
	m_ownerPosition(Vector2::vec2_zeros),
	m_ownerRadius(Vector2::vec2_zeros),
	m_distanceFromOwnerToOther(0.f),
	m_directionFromOwnerToOther(Vector2::vec2_zeros),
	m_ownerRadiusInDirection(0.f),
	m_otherRadiusInDirection(0.f)
{

}