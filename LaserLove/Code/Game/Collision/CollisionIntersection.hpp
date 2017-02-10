#pragma once

#ifndef COLLISIONINTERSECTION_HPP
#define COLLISIONINTERSECTION_HPP
#include "Engine/Math/Vector2.hpp"

class GameEntity;

struct CollisionIntersection
{
public:
	CollisionIntersection();

	bool m_intersected;
	Vector2 m_ownerPosition;
	Vector2 m_otherPosition;
	Vector2 m_ownerRadius;
	Vector2 m_otherRadius;
	GameEntity* m_owner;
	GameEntity* m_other;
	float m_distanceFromOwnerToOther;
	Vector2 m_directionFromOwnerToOther;
	float m_ownerRadiusInDirection;
	float m_otherRadiusInDirection;
};
#endif