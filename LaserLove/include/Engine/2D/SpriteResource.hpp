#pragma once

#ifndef SPRITERESOURCE_HPP
#define SPRITERESOURCE_HPP
#include "Engine/Renderer/3DModelling/Material.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Math/Vertex_PCTT.hpp"
#include "Engine/2D/SpriteVert.hpp"


class SpriteResource
{
private:
	Material* m_material;
	Texture* m_spriteSheet;
	Vector2 m_spriteResourceVirtualSize;
	AABB2 m_spriteCoords;
	size_t m_spriteID;

	static float s_importSize; //height
	static float s_screenSize; //height of window in units
	static Vector2 s_screenResolution; //actual size of screen.
	bool m_responsibleForMaterial;

public:
	//Constructors
	SpriteResource(Texture* texture = nullptr, const AABB2& spriteCoords = AABB2::ZERO_TO_ONE, const size_t& spriteID = 0, const Vector2& virtualSpaceSize = s_screenResolution);
	SpriteResource(const SpriteResource& other) = delete;
	~SpriteResource();
	
	//Setters
	void SetMaterial(Material* mat);
	void SetSpriteSheet(Texture* spriteSheet);
	void SetVirtualSpaceSize(const Vector2& size);
	void SetSpriteCoords(const AABB2& spriteCoords);
	void SetSpriteResourceID(const size_t& spriteID);

	//Getters
	static const Vector2 ConvertVirtualCoordsToScreenCords(const Vector2& virtVal, const Vector2& texelSize, const Vector2& scale);
	const std::vector<Vector2> GetRelativePositionsWithRotation(const Vector2& spriteCenter, const float& rotation, const Vector2& scale = Vector2::vec2_ones) const;
	const std::vector<Vector2> GetRelativePositionsWithoutRotation(const Vector2& scale = Vector2::vec2_ones) const;
	const std::vector<Vertex_PCTT> GetVertices(const Vector2& spriteCenter, const Vector2& scale = Vector2::vec2_ones, const Rgba& tint = Rgba::s_White) const;
	const std::vector<SpriteVert> GetSpriteVertices(const Vector2& spriteCenter, const float& rotation, const Vector2& scale = Vector2::vec2_ones, const Rgba& tint = Rgba::s_White) const;
	const std::vector<unsigned int> GetTriangularIndices() const;
	Material* GetEditableMaterial() const;
	const Material* GetMaterial() const;
	Texture* GetSpriteSheet() const;
	const Vector2 GetVirtualSpaceSize() const;
	const AABB2 GetSpriteCoords() const;
	const size_t GetSpriteResourceID() const;

	//static methods
	static void SetScreenResolution(const Vector2& screenResolution);
	static void SetImportSize(const float& importSize);
	static void SetScreenSize(const float& screenSize);
	static const Vector2 GetScreenResolution();
	static const float GetImportSize();
	static const float GetScreenSize();
	static const float GetInverseAspectRatio();
	static const float GetAspectRatio();


};
#endif