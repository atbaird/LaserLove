#include "Game/Collision/EllipseCollision.hpp"
#include "Game/GameEntity/GameEntity.hpp"
#include "Engine/Math/ExtendedMath.hpp"
#include "Engine/Renderer/3DModelling/Material.hpp"
#include "Engine/Renderer/3DModelling/Mesh.hpp"
#include "Engine/Renderer/3DModelling/MeshBuilder.hpp"
#include "Game/SpriteLoader/CommonVariables.hpp"


//Constructors
EllipseCollision::EllipseCollision(const float& radius, GameEntity* entity, const bool& render)
	: m_xyRadius(radius, radius),
	m_entity(entity),
	m_rotation(0.f),
	m_material(nullptr),
	m_mesh(nullptr),
	m_render(render)
{
	m_material = new Material();
	m_material->SetProgram(CommonVariables::GetCollisionShaderProgram());
	m_mesh = new Mesh();
	m_mesh->CreateVAOID();
	SetUpMesh();
}
EllipseCollision::EllipseCollision(const Vector2& xyRadius, GameEntity* entity, const bool& render)
	: m_xyRadius(xyRadius),
	m_entity(entity),
	m_rotation(0.f),
	m_material(nullptr),
	m_mesh(nullptr),
	m_render(render)
{
	m_material = new Material();
	m_material->SetProgram(CommonVariables::GetCollisionShaderProgram());
	m_mesh = new Mesh();
	m_mesh->CreateVAOID();
	SetUpMesh();
}
EllipseCollision::~EllipseCollision()
{
	m_entity = nullptr;
	if (m_mesh != nullptr)
	{
		delete m_mesh;
		m_mesh = nullptr;
	}
	if (m_material != nullptr)
	{
		delete m_material;
		m_material = nullptr;
	}
}
void EllipseCollision::Render() const
{
	if (m_render == true)
	{
		m_mesh->DrawWith(m_material);
	}
}

//Setters
void EllipseCollision::SetRadius(const float& radius)
{
	m_xyRadius.x = radius;
	m_xyRadius.y = radius;
	SetUpMesh();
}
void EllipseCollision::SetXYRadius(const Vector2& xyRadius)
{
	m_xyRadius = xyRadius;
	SetUpMesh();
}
void EllipseCollision::SetGameEntity(GameEntity* entity)
{
	m_entity = nullptr;
	m_entity = entity;
}
void EllipseCollision::SetRotation(const float& rotationDeg)
{
	m_rotation = rotationDeg;
}

//Getters
const Vector2& EllipseCollision::GetXYRadius() const
{
	return m_xyRadius;
}
const GameEntity* EllipseCollision::GetGameEntity() const
{
	return m_entity;
}
GameEntity* EllipseCollision::GetEditableGameEntity() const
{
	return m_entity;
}
const float EllipseCollision::GetRotation() const
{
	return m_rotation;
}
const float EllipseCollision::GetRotationRadians() const
{
	return DegreesToRadians(m_rotation);
}

//Static methods
const CollisionIntersection EllipseCollision::TestCircleCollisionsIntersecting(const EllipseCollision* ownerCollision, const EllipseCollision* otherCollision)
{
	CollisionIntersection intersection;
	intersection.m_intersected = false;
	if (ownerCollision == nullptr || otherCollision == nullptr
		|| ownerCollision->GetEditableGameEntity() == nullptr || otherCollision->GetEditableGameEntity() == nullptr
		|| ownerCollision->GetEditableGameEntity()->GetTeamID() == otherCollision->GetEditableGameEntity()->GetTeamID())
	{
		return intersection;
	}
	/*
	bool m_intersected;
	Vector2 m_ownerPosition;
	Vector2 m_otherPosition;
	Vector2 m_ownerRadius;
	Vector2 m_otherRadius;
	GameEntity* m_owner;
	GameEntity* m_other;
	float m_distanceFromOwnerToOther;
	Vector2 m_directionFromOwnerToOther;
	*/

	intersection.m_owner = ownerCollision->GetEditableGameEntity();
	intersection.m_other = otherCollision->GetEditableGameEntity();
	intersection.m_ownerPosition = intersection.m_owner->GetPosition();
	intersection.m_otherPosition = intersection.m_other->GetPosition();
	intersection.m_ownerRadius = ownerCollision->GetXYRadius();
	intersection.m_otherRadius = otherCollision->GetXYRadius();

	Vector2 distance = intersection.m_ownerPosition - intersection.m_otherPosition;
	Vector2 distanceFromOwnerToOther = distance;
	intersection.m_distanceFromOwnerToOther = distanceFromOwnerToOther.normalize();
	intersection.m_directionFromOwnerToOther = distanceFromOwnerToOther;
	intersection.m_ownerRadiusInDirection = intersection.m_ownerRadius.x;
	intersection.m_otherRadiusInDirection = intersection.m_otherRadius.x;
	float combinedRadius = abs(intersection.m_ownerRadiusInDirection) + abs(intersection.m_otherRadiusInDirection);
	float xCombined = distance.x * distance.x;
	float yCombined = distance.y * distance.y;
	float distancesCombined = (xCombined + yCombined);
	float multRadius = combinedRadius * combinedRadius;

	if (distancesCombined < multRadius)
	{
		intersection.m_intersected = true;
	}

	//Polar Form of the equation for an ellipse
	//r = (ab) / (sqrt(a ^ 2 * sin ^ 2(theta)+b ^ 2 * cos ^ 2(theta));



	return intersection;
}

//------------------------------------------------------------------------------------
//Private Methods
void EllipseCollision::SetUpMesh()
{
	std::vector<SpriteVert> verts;
	MeshBuilder meshBuilder;
	meshBuilder.BuildCircle(m_xyRadius.x);
	meshBuilder.CopyOnToMeshSpriteVert(m_mesh);
}