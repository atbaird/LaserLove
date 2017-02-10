#pragma once

#ifndef SPRITE_HPP
#define SPRITE_HPP
#include "Engine/2D/SpriteResource.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/Renderer/Texture.hpp"
#include "Engine/2D/SpriteRenderer.hpp"
#include "Engine/Utils/AllocationsSpecial/untrackedallocations.hpp"

class SpriteDatabase;

class Sprite;
struct JobSystemSpriteStruc
{
	JobSystemSpriteStruc(size_t startSprite, size_t endSprite, float dt);
	size_t m_startSprite;
	size_t m_endSprite;
	float m_dt;
};

static void UpdateSpritesJob(void* data);

class Sprite
{
private:
	static size_t s_currentlyAvailableSpriteID;
	static std::vector<Sprite*>* s_sprites;
	static size_t s_numSpritesPerJob;
	static bool s_wantToUseJobSystem;

	const SpriteResource* m_resource;
	std::vector<SpriteVert> m_vertices;
	std::vector<unsigned int> m_indices;
	std::vector<Sprite*> m_children;
	Matrix44 m_model;
	Sprite* m_parent;
	Material* m_material;
	Mesh* m_mesh;

	Rgba m_tint;
	Vector2 m_position;
	Vector2 m_localPosition;
	Vector2 m_scale;
	float m_yawDegrees;
	float m_addedRotationForImage;

	size_t m_layerID;
	size_t m_spriteID;
	bool m_enable;
	bool m_resourceChanged;
	bool m_spriteColorChanged;
	bool m_deleteMaterial;
	bool m_destroyMe;
	bool m_onScreen;
public:
	//Constructors
	Sprite();
	Sprite(const SpriteResource* resource);
	Sprite(const size_t& spriteResourceID);
	virtual ~Sprite();

	//Static Methods
	static void UpdateAll(const float& dt);
	static void UpdateAllJob(size_t start, size_t end, const float& dt);
	static void SetWantToUseJobSystem(bool use);

	//Updates and Render
	virtual void Update(const float& dt);
	virtual void UpdateMesh();
	virtual void UpdateMaterials();
	virtual void FinalCleanup();
	virtual void Render() const;

	//Children and Parent related
	void AddChild(Sprite* child);
	void RemoveChild(Sprite* child);

	//Setters
	virtual void SetMaterial(Material* mat);
	virtual void SetPosition(const Vector2& position);
	virtual void SetLocalPosition(const Vector2& locPos);
	virtual void SetScale(const Vector2& scale);
	virtual void SetRotation(const float& yawDegrees);
	virtual void SetEnabled(const bool& enabled);
	virtual void SetTint(const Rgba& tint);
	virtual void SetParent(Sprite* parent);
	virtual void ChangeLayers(const size_t& layer);
	virtual void ChangeSpriteResource(const SpriteResource* resource);
	virtual const bool CopyMaterialFromResource();
	virtual const bool ClearMaterialFromSprite();
	virtual const bool CopyMaterialFromResourceAndSetShaderProgram(ShaderProgram* prog);
	virtual void SetAddedRotationForImage(const float& rotation);
	virtual void SetOnScreen(const bool& onScreen);

	//Getters
	virtual const Mesh* GetMesh() const;
	virtual Material* GetEditableMaterial() const;
	virtual const Material* GetMaterial() const;
	virtual const SpriteResource* GetSpriteResource() const;
	virtual const size_t GetSpriteID() const;
	virtual const size_t GetLayerID() const;
	virtual const Vector2 GetPosition() const;
	virtual const Vector2 GetLocalPosition() const;
	virtual const Vector2 GetScale() const;
	virtual const float GetRotation() const;
	virtual const Rgba GetTint() const;
	virtual Sprite* GetParent() const;
	virtual const bool GetEnabled() const;
	virtual const float GetAddedRotationForImage() const;
	virtual const bool GetOnScreen() const;
	virtual const Vector2 GetTrueScale() const;

	static const Vector2 CalcWorldPositionFromLocal(Sprite* sprite);
	static void DeleteAllSprites();

private:
	void RecalcPositionAndRotationFromParent();
};

#endif