#pragma once

#ifndef CIRCLECOLLISION_HPP
#define CIRCLECOLLISION_HPP
#include "Game/Collision/CollisionIntersection.hpp"

class Mesh;
class Material;
class GameEntity;

class EllipseCollision
{
private:
	Vector2 m_xyRadius;
	GameEntity* m_entity;
	float m_rotation;
	Mesh* m_mesh;
	Material* m_material;
	bool m_render;
public:
	//Constructors
	EllipseCollision(const float& radius, GameEntity* entity = nullptr, const bool& render = false);
	EllipseCollision(const Vector2& xyRadius, GameEntity* entity = nullptr, const bool& render = false);
	~EllipseCollision();

	virtual void Render() const;

	//Setters
	void SetRadius(const float& radius);
	void SetXYRadius(const Vector2& xyRadius);
	void SetGameEntity(GameEntity* entity);
	void SetRotation(const float& rotationDeg);

	//Getters
	const Vector2& GetXYRadius() const;
	const GameEntity* GetGameEntity() const;
	GameEntity* GetEditableGameEntity() const;
	const float GetRotation() const; //This is in Degrees
	const float GetRotationRadians() const;

	//Static methods
	static const CollisionIntersection TestCircleCollisionsIntersecting(const EllipseCollision* ownerCollision, const EllipseCollision* otherCollision);

private:
	//Private Methods
	void SetUpMesh();
};
#endif