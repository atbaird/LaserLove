#include "Engine/2D/SpriteResource.hpp"
#include "Engine/Math/ExtendedMath.hpp"
#include "Engine/Renderer/3DModelling/MeshBuilder.hpp"

float SpriteResource::s_importSize = 720.f; //height
float SpriteResource::s_screenSize = (1080.f); //height of window in units
Vector2 SpriteResource::s_screenResolution = Vector2(1080.f, 720.f); //actual size of screen.

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//Constructors
SpriteResource::SpriteResource(Texture* texture, const AABB2& spriteCoords, const size_t& spriteID, const Vector2& virtualSpaceSize)
	: m_spriteResourceVirtualSize(virtualSpaceSize),
	m_textureCoords(spriteCoords),
	m_spriteID(spriteID),
	m_spriteSheet(texture),
	m_material(nullptr)
{
	m_material = new Material();
}

SpriteResource::~SpriteResource()
{
	if (m_material != nullptr)
	{
		delete m_material;
		m_material = nullptr;
	}
}


//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//Setters
void SpriteResource::SetMaterial(Material* mat)
{
	m_material = nullptr;
	m_material = mat;
}

void SpriteResource::SetSpriteSheet(Texture* spriteSheet)
{
	m_spriteSheet = nullptr;
	m_spriteSheet = spriteSheet;
}

void SpriteResource::SetVirtualSpaceSize(const Vector2& size)
{
	m_spriteResourceVirtualSize = size;
}

void SpriteResource::SetSpriteCoords(const AABB2& spriteCoords)
{
	m_textureCoords = spriteCoords;
}

void SpriteResource::SetSpriteResourceID(const size_t& spriteID)
{
	m_spriteID = spriteID;
}


//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//Getters
const Vector2 SpriteResource::ConvertVirtualCoordsToScreenCords(const Vector2& virtVal, const Vector2& texelSize, const Vector2& scale)
{

	const float heightOverWidth = GetInverseAspectRatio();
	const float conversion = (s_importSize / s_screenSize);
	const Vector2 fullConversion = Vector2((float)s_screenResolution.x, (float)s_screenResolution.y) * conversion;
	const Vector2 pix = Vector2(texelSize.x, texelSize.y) * (s_importSize / s_screenSize);
	const Vector2 mixConv = Vector2(pix.x / fullConversion.x, pix.y / fullConversion.y);
	const Vector2 trueSize = Vector2(virtVal.x * mixConv.x * scale.x / heightOverWidth, virtVal.y * mixConv.y * scale.y) * conversion;
	return trueSize;
}

void SpriteResource::GetRelativePositionsWithRotation(std::vector<Vector2>& arrayToPutIn, const Vector2& spriteCenter, const float& rotation, const Vector2& scale) const
{
	//Idea is this could be used for rendering sprites correctly; potentially used for collision?
	//Rotation code made in reference to: http://answers.unity3d.com/questions/661383/whats-the-most-efficient-way-to-rotate-a-vector2-o.html
	GetRelativePositionsWithoutRotation(arrayToPutIn, scale);
	const Vector2 cossin = Vector2(CosDegrees(rotation), SinDegrees(rotation));

	for (size_t i = 0; i < arrayToPutIn.size(); i++)
	{
		Vector2 original = arrayToPutIn.at(i);
		arrayToPutIn[i] = Vector2((original.x * cossin.x) - (original.y * cossin.y), (cossin.y * original.x) + (cossin.x * original.y)) + spriteCenter;
	}
}

void SpriteResource::GetRelativePositionsWithoutRotation(std::vector<Vector2>& arrayToPutIn, const Vector2& scale) const
{
	Vector2 texel = Vector2::vec2_ones;
	if (m_spriteSheet != nullptr)
	{
		texel = Vector2((float)m_spriteSheet->m_texelSize.x, (float)m_spriteSheet->m_texelSize.y);
	}
	//The base method for getting the correct corner positions.
	const Vector2 trueSize = ConvertVirtualCoordsToScreenCords(m_spriteResourceVirtualSize, texel, scale);

	const Vector2 RelUp = Vector2(0.f, (trueSize.y) / 2.f);
	const Vector2 RelRight = Vector2((trueSize.x) / 2.f, 0.f);
	const Vector2 center = Vector2(0.f, 0.f);

	MeshBuilder builder;
	builder.BuildQuad(Vector3(0.f, 1.f, 0.f), Vector3(1.f, 0.f, 0.f), trueSize.x, trueSize.y);
	std::vector<Master_Vertex> builderVerts;
	builder.GetVertices(builderVerts);

	for (size_t i = 0; i < builderVerts.size(); i++)
	{
		arrayToPutIn.push_back(Vector2(builderVerts.at(i).position.x, builderVerts.at(i).position.y));
	}

	//std::vector<Vector2> verts;
	//verts.push_back(center - RelRight - RelUp);
	//verts.push_back(center + RelRight - RelUp);
	//verts.push_back(center + RelRight + RelUp);
	//verts.push_back(center - RelRight + RelUp);

}

void SpriteResource::GetVertices(std::vector<Vertex_PCTT>& arrayToPlaceIn, Vector2& spriteCenter, const Vector2& scale, const Rgba& tint) const
{
	std::vector<Vector2> vertPos;
	GetRelativePositionsWithoutRotation(vertPos, scale);

	//std::vector<Vertex_PCTT> vertices;

	Vertex_PCTT bl = Vertex_PCTT(Vector3(vertPos.at(0) + spriteCenter), tint, Vector2(m_textureCoords.mins.x, m_textureCoords.maxs.y));
	Vertex_PCTT br = Vertex_PCTT(Vector3(vertPos.at(1) + spriteCenter), tint, Vector2(m_textureCoords.maxs.x, m_textureCoords.maxs.y));
	Vertex_PCTT tr = Vertex_PCTT(Vector3(vertPos.at(2) + spriteCenter), tint, Vector2(m_textureCoords.mins.x, m_textureCoords.mins.y));
	Vertex_PCTT tl = Vertex_PCTT(Vector3(vertPos.at(3) + spriteCenter), tint, Vector2(m_textureCoords.maxs.x, m_textureCoords.mins.y));

	arrayToPlaceIn.push_back(bl);
	arrayToPlaceIn.push_back(br);
	arrayToPlaceIn.push_back(tr);
	arrayToPlaceIn.push_back(tl);
	//return vertices;
}

void SpriteResource::GetSpriteVertices(std::vector<SpriteVert>& arrayToPlaceIn, const Vector2& spriteCenter, const float& rotation, const Vector2& scale,
	const Rgba& tint) const
{
	std::vector<Vector2> vertPos;
	GetRelativePositionsWithRotation(vertPos, spriteCenter, rotation, scale);

	SpriteVert bl = SpriteVert(vertPos.at(0), tint, Vector2(m_textureCoords.mins.x, m_textureCoords.maxs.y));
	SpriteVert br = SpriteVert(vertPos.at(1), tint, Vector2(m_textureCoords.maxs.x, m_textureCoords.maxs.y));
	SpriteVert tr = SpriteVert(vertPos.at(2), tint, Vector2(m_textureCoords.mins.x, m_textureCoords.mins.y));
	SpriteVert tl = SpriteVert(vertPos.at(3), tint, Vector2(m_textureCoords.maxs.x, m_textureCoords.mins.y));

	arrayToPlaceIn.push_back(bl);
	arrayToPlaceIn.push_back(br);
	arrayToPlaceIn.push_back(tr);
	arrayToPlaceIn.push_back(tl);
	//return vertices;
}

void SpriteResource::GetTriangularIndices(std::vector<unsigned int>& indices) const
{

	MeshBuilder builder;
	builder.BuildQuad();

	builder.GetIndices(indices);
}

Material* SpriteResource::GetEditableMaterial() const
{
	return m_material;
}

const Material* SpriteResource::GetMaterial() const
{
	return m_material;
}

Texture* SpriteResource::GetSpriteSheet() const
{
	return m_spriteSheet;
}

const Vector2 SpriteResource::GetVirtualSpaceSize() const
{
	return m_spriteResourceVirtualSize;
}

const Vector2 SpriteResource::GetSpriteStartTextureCoordsPercentages() const
{
	return m_textureCoords.mins;
}

const Vector2 SpriteResource::GetSpriteEndTextureCoordsPercentages() const
{
	return m_textureCoords.maxs;
}

const IntVector2 SpriteResource::GetSpriteStartTextureCoords() const
{
	if (m_spriteSheet == nullptr)
	{
		return IntVector2::ivec2_zeros;
	}
	IntVector2 val = IntVector2((int)(m_textureCoords.mins.x * (float)m_spriteSheet->m_texelSize.x),
		(int)(m_textureCoords.mins.y * (float)m_spriteSheet->m_texelSize.y));
	return val;
}

const IntVector2 SpriteResource::GetSpriteEndTextureCoords() const
{
	if (m_spriteSheet == nullptr)
	{
		return IntVector2::ivec2_zeros;
	}
	IntVector2 val = IntVector2((int)(m_textureCoords.maxs.x * (float)m_spriteSheet->m_texelSize.x),
		(int)(m_textureCoords.maxs.y * (float)m_spriteSheet->m_texelSize.y));
	return val;
}

const AABB2 SpriteResource::GetSpriteCoords() const
{
	return m_textureCoords;
}

const size_t SpriteResource::GetSpriteResourceID() const
{
	return m_spriteID;
}



//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//static methods
void SpriteResource::SetScreenResolution(const Vector2& screenResolution)
{
	s_screenResolution = screenResolution;
}

void SpriteResource::SetImportSize(const float& importSize)
{
	s_importSize = importSize;
}

void SpriteResource::SetScreenSize(const float& screenSize)
{
	s_screenSize = screenSize;
}

const Vector2 SpriteResource::GetScreenResolution()
{
	return s_screenResolution;
}

const float SpriteResource::GetImportSize()
{
	return s_importSize;
}

const float SpriteResource::GetScreenSize()
{
	return s_screenSize;
}

const float SpriteResource::GetInverseAspectRatio()
{
	const Vector2 screenres = GetScreenResolution();
	return screenres.y / screenres.x;
}

const float SpriteResource::GetAspectRatio()
{
	const Vector2 screenres = GetScreenResolution();
	return screenres.x / screenres.y;
}
