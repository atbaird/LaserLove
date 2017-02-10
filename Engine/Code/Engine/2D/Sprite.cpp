#include "Engine/2D/Sprite.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/2D/SpriteDatabase.hpp"
#include "Engine/Math/ExtendedMath.hpp"
#include "Engine/Core/JobSystem/JobSystem.hpp"

//------------------------------------------------------------
//------------------------------------------------------------
//Constructors
size_t Sprite::s_currentlyAvailableSpriteID = 0;
std::vector<Sprite*>* Sprite::s_sprites = nullptr;

void UpdateSpritesJob(void* data)
{
	JobSystemSpriteStruc* jobstruc = (JobSystemSpriteStruc*)(data);

	if (jobstruc == nullptr)
	{
		return;
	}

	Sprite::UpdateAllJob(jobstruc->m_startSprite, jobstruc->m_endSprite, jobstruc->m_dt);

	delete jobstruc;
}

JobSystemSpriteStruc::JobSystemSpriteStruc(size_t startSprite, size_t endSprite, float dt)
	: m_startSprite(startSprite),
	m_endSprite(endSprite),
	m_dt(dt)
{

}

size_t Sprite::s_numSpritesPerJob = 500;
bool Sprite::s_wantToUseJobSystem = false;

/*
const SpriteResource* m_resource;
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
bool m_deleteMaterial;
bool m_destroyMe;
bool m_onScreen;
*/

Sprite::Sprite()
	: m_resource(nullptr),
	m_parent(nullptr),
	m_material(nullptr),
	m_mesh(nullptr),
	m_tint(Rgba::s_White),
	m_position(Vector2::vec2_zeros),
	m_localPosition(Vector2::vec2_zeros),
	m_scale(Vector2::vec2_ones),
	m_yawDegrees(0.f),
	m_addedRotationForImage(0.f),
	m_layerID(0),
	m_spriteID(s_currentlyAvailableSpriteID),
	m_enable(false),
	m_resourceChanged(true),
	m_deleteMaterial(false),
	m_destroyMe(false),
	m_onScreen(false),
	m_spriteColorChanged(false)
{
	s_currentlyAvailableSpriteID++;
	m_mesh = new Mesh();
	m_mesh->CreateVAOID();
	SetEnabled(true);
	if (s_sprites == nullptr)
	{
		s_sprites = new std::vector<Sprite*>();
	}
	s_sprites->push_back(this);
}

Sprite::Sprite(const SpriteResource* resource)
	: m_resource(resource),
	m_parent(nullptr),
	m_material(nullptr),
	m_mesh(nullptr),
	m_tint(Rgba::s_White),
	m_position(Vector2::vec2_zeros),
	m_localPosition(Vector2::vec2_zeros),
	m_scale(Vector2::vec2_ones),
	m_yawDegrees(0.f),
	m_addedRotationForImage(0.f),
	m_layerID(0),
	m_spriteID(s_currentlyAvailableSpriteID),
	m_enable(false),
	m_resourceChanged(true),
	m_deleteMaterial(false),
	m_destroyMe(false),
	m_onScreen(false),
	m_spriteColorChanged(false)
{
	s_currentlyAvailableSpriteID++;
	m_mesh = new Mesh();
	m_mesh->CreateVAOID();
	SetEnabled(true);
	if (s_sprites == nullptr)
	{
		s_sprites = new std::vector<Sprite*>();
	}
	s_sprites->push_back(this);
}

Sprite::Sprite(const size_t& spriteResourceID)
	: m_resource(nullptr),
	m_parent(nullptr),
	m_material(nullptr),
	m_mesh(nullptr),
	m_tint(Rgba::s_White),
	m_position(Vector2::vec2_zeros),
	m_localPosition(Vector2::vec2_zeros),
	m_scale(Vector2::vec2_ones),
	m_yawDegrees(0.f),
	m_addedRotationForImage(0.f),
	m_layerID(0),
	m_spriteID(s_currentlyAvailableSpriteID),
	m_enable(false),
	m_resourceChanged(true),
	m_deleteMaterial(false),
	m_destroyMe(false),
	m_onScreen(false),
	m_spriteColorChanged(false)
{
	s_currentlyAvailableSpriteID++;
	m_mesh = new Mesh();
	m_mesh->CreateVAOID();
	SetEnabled(true);
	if (s_sprites == nullptr)
	{
		s_sprites = new std::vector<Sprite*>();
	}
	s_sprites->push_back(this);
	if (SpriteDatabase::g_SpriteDatabase == nullptr)
	{
		return;
	}
	m_resource = SpriteDatabase::g_SpriteDatabase->GetSprite(spriteResourceID);
}

Sprite::~Sprite()
{
	if (m_mesh != nullptr)
	{
		delete m_mesh;
		m_mesh = nullptr;
	}
	if (m_material != nullptr && m_deleteMaterial == true)
	{
		delete m_material;
		m_material = nullptr;
		m_deleteMaterial = false;
	}
	if(SpriteRenderer::g_SpriteRenderer != nullptr)
	{
		SpriteRenderer::g_SpriteRenderer->UnregisterSprite(this);
	}
	if (m_parent != nullptr)
	{
		m_parent->RemoveChild(this);
	}
	for (size_t i = 0; i < m_children.size(); i++)
	{
		if (m_children.at(i) != nullptr)
		{
			delete m_children[i];
			m_children[i] = nullptr;
		}
	}
	m_children.clear();
	if (s_sprites == nullptr)
	{
		return;
	}
	for (size_t i = 0; i < s_sprites->size(); i++)
	{
		if (s_sprites->at(i) == this)
		{
			s_sprites->erase(s_sprites->begin() + i);
			break;
		}
	}
	if (s_sprites->size() == 0)
	{
		delete s_sprites;
		s_sprites = nullptr;
	}
}


//------------------------------------------------------------
//------------------------------------------------------------
//Static Methods
void Sprite::UpdateAll(const float& dt)
{
	if (s_sprites == nullptr)
	{
		return;
	}
	bool jobSystemEnabled = false;
	if (JobSystem::s_jobSystem != nullptr && s_wantToUseJobSystem == true)
	{
		jobSystemEnabled = true;
	}
	if (jobSystemEnabled)
	{
		for (size_t cur = 0; cur < s_sprites->size(); cur = cur + s_numSpritesPerJob)
		{
			size_t end = cur + s_numSpritesPerJob;
			if (end > s_sprites->size())
			{
				end = s_sprites->size();
			}
			JobSystemSpriteStruc* data = new JobSystemSpriteStruc(cur, end, dt);
			JobSystem::CreateAndAddJob(0, UpdateSpritesJob, data);
		}
	}
	else
	{
		for (size_t i = 0; i < s_sprites->size(); i++)
		{
			if (s_sprites->at(i) != nullptr)
			{
				s_sprites->at(i)->Update(dt);
			}
		}
	}
}

void Sprite::UpdateAllJob(size_t start, size_t end, const float& dt)
{
	for (size_t i = start; i < end; i++)
	{
		if (s_sprites != nullptr && s_sprites->at(i) != nullptr)
		{
			s_sprites->at(i)->Update(dt);
		}
	}
}

void Sprite::SetWantToUseJobSystem(bool use)
{
	s_wantToUseJobSystem = use;
}

//------------------------------------------------------------
//------------------------------------------------------------
//Updates and Render
void Sprite::Update(const float& dt)
{
	dt;
	UpdateMesh();
	UpdateMaterials();
}

void Sprite::UpdateMesh()
{
	//Update method is suppose to make sure the model has its vertices and ibos.
	if (m_resourceChanged == true)
	{
		if (m_resource == nullptr)
		{
			m_indices.clear();
			m_vertices.clear();
		}
		else
		{
			m_vertices.clear();
			m_resource->GetSpriteVertices(m_vertices, Vector2::vec2_zeros,/*m_position, m_yawDegrees +*/ m_addedRotationForImage, m_scale, m_tint);
			m_indices.clear();
			m_resource->GetTriangularIndices(m_indices);
		}
		m_mesh->setVBO(m_vertices);
		m_mesh->setIBO(m_indices);
	}
	else if (m_spriteColorChanged == true)
	{
		for (size_t i = 0; i < m_vertices.size(); i++)
		{
			m_vertices.at(i).m_color = m_tint;
		}
		m_mesh->setVBO(m_vertices);
	}
	m_spriteColorChanged = false;
	m_resourceChanged = false;
	m_model.MatrixMakeIdentity();
	Matrix44::MakeEulerRotation(m_model, 0.f, 0.f, DegreesToRadians(m_yawDegrees), m_position);
}

void Sprite::UpdateMaterials()
{
	//This takes both materials and makes sure the current sprite sheet data is up to date prior to the Render Method.
	if (m_resource != nullptr)
	{
		Texture* spriteSheet = m_resource->GetSpriteSheet();
		Material* defMesh = m_resource->GetEditableMaterial();
		if (m_material != nullptr)
		{
			if (spriteSheet != nullptr)
			{
				bool isSet = m_material->SetTexture(spriteSheet, spriteSheet->m_openglTextureID, "gDiffuseTex", 0);
				if (isSet == false)
				{
					m_material->AddTexture(spriteSheet, spriteSheet->m_openglTextureID, "gDiffuseTex");
				}
			}
		}
		if (defMesh != nullptr)
		{
			if (spriteSheet != nullptr)
			{
				bool isSet = defMesh->SetTexture(spriteSheet, spriteSheet->m_openglTextureID, "gDiffuseTex", 0);
				if (isSet == false)
				{
					defMesh->AddTexture(spriteSheet, spriteSheet->m_openglTextureID, "gDiffuseTex");
				}
			}
		}
	}
}
void Sprite::FinalCleanup()
{

}

void Sprite::Render() const
{
	if (m_resource == nullptr
		|| m_mesh == nullptr
		|| m_onScreen == false)
	{
		return;
	}
	Texture* tex = nullptr;
	const Material* defMat = nullptr;
	if (m_resource != nullptr)
	{
		tex = m_resource->GetSpriteSheet();
		defMat = m_resource->GetMaterial();
		if (tex == nullptr)
		{
			tex = nullptr;
		}
	}
	if ((defMat == nullptr && m_material == nullptr)
		|| g_Renderer == nullptr)
	{
		return;
	}


	if (m_material != nullptr)
	{
		m_material->SetModelMatrix(m_model);
		m_mesh->DrawWith(m_material);
	}
	else
	{
		defMat->SetModelMatrix(m_model);
		m_mesh->DrawWith(defMat);
	}
}


//------------------------------------------------------------
//------------------------------------------------------------
//Children and Parent related
void Sprite::AddChild(Sprite* child)
{
	if (child == nullptr)
	{
		return;
	}
	child->SetParent(this);
	m_children.push_back(child);
}

void Sprite::RemoveChild(Sprite* child)
{
	if (child == nullptr)
	{
		return;
	}
	for (size_t i = 0; i < m_children.size(); i++)
	{
		if (m_children.at(i) == child)
		{
			m_children.erase(m_children.begin() + i);
			return;
		}
	}
}

//------------------------------------------------------------
//------------------------------------------------------------
//Setters
void Sprite::SetMaterial(Material* mat)
{
	if (m_deleteMaterial == true && m_material != nullptr)
	{
		delete m_material;
		m_material = nullptr;
	}
	m_deleteMaterial = false;
	m_material = nullptr;
	m_material = mat;
}

void Sprite::SetPosition(const Vector2& position)
{
	if (m_position == position)
	{
		return;
	}
	m_position = position;
	Vector2 parentPos = CalcWorldPositionFromLocal(m_parent);

	m_localPosition = position - parentPos;

	for (size_t i = 0; i < m_children.size(); i++)
	{
		if (m_children.at(i) != nullptr)
		{
			m_children[i]->RecalcPositionAndRotationFromParent();
		}
	}
}

void Sprite::SetLocalPosition(const Vector2& locPos)
{
	if (m_localPosition == locPos)
	{
		return;
	}
	m_localPosition = locPos;
	Vector2 myPos = CalcWorldPositionFromLocal(this);
	m_position = myPos;

	for (size_t i = 0; i < m_children.size(); i++)
	{
		if (m_children.at(i) != nullptr)
		{
			m_children[i]->RecalcPositionAndRotationFromParent();
		}
	}
}

void Sprite::SetScale(const Vector2& scale)
{
	if (m_scale == scale)
	{
		return;
	}
	m_scale = scale;
	m_resourceChanged = true;
}

void Sprite::SetRotation(const float& yawDegrees)
{
	if (m_yawDegrees == yawDegrees)
	{
		return;
	}
	m_yawDegrees = yawDegrees;
	while (m_yawDegrees >= 360.f)
	{
		m_yawDegrees -= 360.f;
	}
	while (m_yawDegrees < 0.f)
	{
		m_yawDegrees += 360.f;
	}

}

void Sprite::SetEnabled(const bool& enabled)
{
	if (enabled == m_enable)
	{
		return;
	}
	m_enable = enabled;

	if (SpriteRenderer::g_SpriteRenderer != nullptr)
	{
		if (m_enable == true)
		{
			SpriteRenderer::g_SpriteRenderer->RegisterSprite(this);
		}
		else
		{
			SpriteRenderer::g_SpriteRenderer->UnregisterSprite(this);
		}
	}
}

void Sprite::SetTint(const Rgba& tint)
{
	if (m_tint == tint)
	{
		return;
	}
	m_tint = tint;
	m_spriteColorChanged = true;
	//m_resourceChanged = true;
}

void Sprite::SetParent(Sprite* parent)
{
	m_parent = nullptr;
	m_parent = parent;
}

void Sprite::ChangeLayers(const size_t& layer)
{
	if (layer == m_layerID)
	{
		return;
	}

	if (m_enable == true && SpriteRenderer::g_SpriteRenderer != nullptr)
	{
		SpriteRenderer::g_SpriteRenderer->UnregisterSprite(this);
		m_layerID = layer;
		SpriteRenderer::g_SpriteRenderer->RegisterSprite(this);
	}
	else
	{
		m_layerID = layer;
	}
}

void Sprite::ChangeSpriteResource(const SpriteResource* resource)
{
	m_resource = nullptr;
	m_resource = resource;
	m_resourceChanged = true;
}

const bool Sprite::CopyMaterialFromResource()
{
	if (m_resource == nullptr)
	{
		m_deleteMaterial = false;
		return false;
	}
	if (m_material != nullptr)
	{
		delete m_material;
		m_material = nullptr;
	}
	m_material = Material::MakeCopy(m_resource->GetMaterial());
	m_deleteMaterial = true;
	return true;
}

const bool Sprite::ClearMaterialFromSprite()
{
	if (m_material == nullptr)
	{
		m_deleteMaterial = false;
		return false;
	}
	delete m_material;
	m_material = nullptr;
	m_deleteMaterial = false;
	return true;
}

const bool Sprite::CopyMaterialFromResourceAndSetShaderProgram(ShaderProgram* prog)
{
	CopyMaterialFromResource();
	if (m_material != nullptr)
	{
		m_material->SetProgram(prog);
		return true;
	}
	return false;
}

void Sprite::SetAddedRotationForImage(const float& rotation)
{
	m_addedRotationForImage = rotation;
}

void Sprite::SetOnScreen(const bool& onScreen)
{
	m_onScreen = onScreen;
}

//------------------------------------------------------------
//------------------------------------------------------------
//Getters
const Mesh* Sprite::GetMesh() const
{
	return m_mesh;
}

Material* Sprite::GetEditableMaterial() const
{
	return m_material;
}

const Material* Sprite::GetMaterial() const
{
	return m_material;
}

const SpriteResource* Sprite::GetSpriteResource() const
{
	return m_resource;
}

const size_t Sprite::GetSpriteID() const
{
	return m_spriteID;
}

const size_t Sprite::GetLayerID() const
{
	return m_layerID;
}

const Vector2 Sprite::GetPosition() const
{
	return m_position;
}

const Vector2 Sprite::GetLocalPosition() const
{
	return m_localPosition;
}

const Vector2 Sprite::GetScale() const
{
	return m_scale;
}

const float Sprite::GetRotation() const
{
	return m_yawDegrees;
}

const Rgba Sprite::GetTint() const
{
	return m_tint;
}

Sprite* Sprite::GetParent() const
{
	return m_parent;
}

const bool Sprite::GetEnabled() const
{
	return m_enable;
}

const float Sprite::GetAddedRotationForImage() const
{
	return m_addedRotationForImage;
}

const bool Sprite::GetOnScreen() const
{
	return m_onScreen;
}

const Vector2 Sprite::GetTrueScale() const
{
	if (m_resource == nullptr)
	{
		return m_scale;
	}
	Texture* tex = m_resource->GetSpriteSheet();
	Vector2 texel = Vector2::vec2_ones;
	if (tex != nullptr)
	{
		IntVector2 texelI = tex->m_texelSize;
		texel = Vector2((float)texelI.x, (float)texelI.y);
	}
	return SpriteResource::ConvertVirtualCoordsToScreenCords(m_resource->GetVirtualSpaceSize(), texel, m_scale);
}

const Vector2 Sprite::CalcWorldPositionFromLocal(Sprite* sprite)
{
	if (sprite == nullptr)
	{
		return Vector2::vec2_zeros;
	}

	Vector2 parentRot = CalcWorldPositionFromLocal(sprite->GetParent());
	Vector2 cur;

	Vector2 localPos = sprite->GetLocalPosition();

	cur = parentRot + localPos;
	return cur;
}

void Sprite::DeleteAllSprites()
{
	if (s_sprites == nullptr)
	{
		return;
	}
	for (size_t i = 0; s_sprites != nullptr && i < s_sprites->size(); i=i)
	{
		Sprite* sprite = s_sprites->at(i);
		if (sprite != nullptr)
		{
			delete sprite;
		}
	}
	if (s_sprites != nullptr)
	{
		s_sprites->clear();
		delete s_sprites;
		s_sprites = nullptr;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------
//Private Methods
void Sprite::RecalcPositionAndRotationFromParent()
{
	Vector2 parentPos = CalcWorldPositionFromLocal(m_parent);
	Vector2 myPos = CalcWorldPositionFromLocal(this);
	m_position = myPos;
	m_localPosition = m_position - parentPos;

	for (size_t i = 0; i < m_children.size(); i++)
	{
		if (m_children.at(i) != nullptr)
		{
			m_children[i]->RecalcPositionAndRotationFromParent();
		}
	}
}