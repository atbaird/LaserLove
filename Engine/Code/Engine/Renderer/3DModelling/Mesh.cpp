#include "Engine/Renderer/3DModelling/Mesh.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Math/Vertices/Vertex_PCT.hpp"
#include "Engine/Math/Vertices/Vertex_PCTT.hpp"
#include "Engine/Math/Vertices/Vertex_PCTT_Skeleton.hpp"
#include "Engine/Math/Vertices/Vertex_UI.hpp"
#include "Engine/2D/SpriteVert.hpp"
#include "Engine/2D/Particle2D.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include <typeinfo>

//---------------------------------------------------------------------
//---------------------------------------------------------------------
//Constructors
Mesh::Mesh(const unsigned int& sampleID, const PrimitiveType& drawMode, const DataType& iboDataType, const VBOusage& drawUsage, const unsigned int& vaoID)
	: IBO_ID(0),
	VBO_ID(0),
	VBOType(VBO_Vertex_PCT),
	m_vaoID(vaoID),
	samplerID(sampleID),
	m_drawMode(drawMode),
	m_iboDataType(iboDataType),
	m_drawUsage(drawUsage),
	m_skeleton(nullptr),
	m_debugSkeleton(false)
{
	PregenerateVBOAndIBO();
}

template<typename T>
Mesh::Mesh(const unsigned int& sampleID, const unsigned int& vaoID, const std::vector<Vertex_PCT>* model, const std::vector<T>* ibos, PrimitiveType drawMode, const DataType& iboDataType, const VBOusage& drawUsage)
	: IBO_ID(0),
	VBO_ID(0),
	VBOType(VBO_Vertex_PCT),
	m_vaoID(vaoID),
	samplerID(sampleID),
	m_drawMode(drawMode),
	m_iboDataType(iboDataType),
	m_drawUsage(drawUsage),
	m_skeleton(nullptr),
	m_debugSkeleton(false)
{
	if (model == nullptr || ibos == nullptr)
	{
		return;
	}
	numOfVBOObjects = model->size();
	numOfIBOObjects = ibos->size();
	PregenerateVBOAndIBO();
	g_Renderer->GenerateOrUpdateVBO_ID(&model, VBO_ID, drawUsage);
	g_Renderer->GenerateOrUpdateVBO_ID(&ibos, IBO_ID, drawUsage);
}
Mesh::~Mesh()
{
	g_Renderer->DeleteVBO_ID(VBO_ID);
	g_Renderer->DeleteVBO_ID(IBO_ID);
	g_Renderer->DeleteVAO(m_vaoID);
}


//---------------------------------------------------------------------
//---------------------------------------------------------------------
//Setters
void Mesh::CreateVAOID()
{
	unsigned int meh = m_vaoID;
	g_Renderer->GenerateVAO(meh);
	m_vaoID = meh;
}
void Mesh::SetSamplerID(const unsigned int& sampleID)
{ 
	samplerID = sampleID; 
}
void Mesh::SetSkeleton(Skeleton* skele)
{
	m_skeleton = nullptr;
	m_skeleton = skele;
}

void Mesh::PregenerateVBOAndIBO()
{
	g_Renderer->GenerateBuffer(VBO_ID);
	g_Renderer->GenerateBuffer(IBO_ID);
	m_iboDataType = UNSIGNED_INTEGER_;
}

void Mesh::SetDebugSkeleton(const bool& debug)
{
	m_debugSkeleton = debug;
}
void Mesh::SetDrawMode(const PrimitiveType& drawMode)
{
	m_drawMode = drawMode;
}
void Mesh::setVBOAndIBO(const std::vector<Vertex_PCT>& model, const std::vector<unsigned int>& ibos)
{
	setIBO(ibos);
	setVBO(model);
}

void Mesh::setIBO(const std::vector<unsigned int>& ibos)
{
	g_Renderer->GenerateOrUpdateVBO_ID((void *)ibos.data(), ibos.size(), sizeof(ibos[0]), IBO_ID, m_drawUsage);
	numOfIBOObjects = ibos.size();
	m_iboDataType = UNSIGNED_INTEGER_;
}
void Mesh::setVBO(const std::vector<Vertex_PCT>& model)
{
	VBOType = VBO_Vertex_PCT;
	g_Renderer->GenerateOrUpdateVBO_ID((void *)model.data(), model.size(), sizeof(model[0]), VBO_ID, m_drawUsage);
	numOfVBOObjects = model.size();
}

void Mesh::setVBO(const std::vector<Vertex_PCTT>& model)
{
	VBOType = VBO_Vertex_PCTT;
	g_Renderer->GenerateOrUpdateVBO_ID((void *)model.data(), model.size(), sizeof(model[0]), VBO_ID, m_drawUsage);
	numOfVBOObjects = model.size();
}
void Mesh::setVBO(const std::vector<Vertex_PCTT_Skeleton>& model)
{
	VBOType = VBO_Vertex_PCTT_Skeleton;
	g_Renderer->GenerateOrUpdateVBO_ID((void *)model.data(), model.size(), sizeof(model[0]), VBO_ID, m_drawUsage);
	numOfVBOObjects = model.size();
}

void Mesh::setVBO(const std::vector<SpriteVert>& model)
{
	VBOType = VBO_SpriteVert;
	g_Renderer->GenerateOrUpdateVBO_ID((void *)model.data(), model.size(), sizeof(model[0]), VBO_ID, m_drawUsage);
	numOfVBOObjects = model.size();
}

void Mesh::setVBO(const std::vector<Particle2D>& model)
{
	VBOType = VBO_Particle2D;
	g_Renderer->GenerateOrUpdateVBO_ID((void *)model.data(), model.size(), sizeof(model[0]), VBO_ID, m_drawUsage);
	numOfVBOObjects = model.size();
}
void Mesh::setVBOWithNoInternalAlterations(const std::vector<Particle2D>& model) const
{
	g_Renderer->UpdateVBO_IDOnly((void *)model.data(), numOfVBOObjects, sizeof(model[0]), VBO_ID, m_drawUsage);
}

void Mesh::setIBOWithNoInternalAlterations(const std::vector<Particle2D>& model) const
{
	std::vector<unsigned int> indices;
	switch(m_drawMode)
	{
	case PRIMITIVE_POINTS:
		for (size_t i = 0; i < model.size(); i++)
		{
			indices.push_back(i);
		}
		break;
	default:
		break;
	}
	g_Renderer->UpdateVBO_IDOnly((void *)indices.data(), numOfIBOObjects, sizeof(indices[0]), IBO_ID, m_drawUsage);
}


//---------------------------------------------------------------------
//---------------------------------------------------------------------
//Operators
void Mesh::DrawWith(const Material* inMaterial) const
{
	ShaderProgram* prog = inMaterial->GetProgram();
	if (prog == nullptr)
	{
		return;
	}
	unsigned int progID = prog->GetShaderProgramID();

	if (m_skeleton != nullptr)
	{
		prog->SetSkeleton("gMatrices", m_skeleton, m_debugSkeleton);
	}

	switch (VBOType)
	{
	case VBO_Vertex_PCT:
		BindProgramToMesh(progID, Vertex_PCT::s_EmptyVert);
		break;
	case VBO_Vertex_PCTT:
		BindProgramToMesh(progID, Vertex_PCTT::s_EmptyVert);
		break;
	case VBO_Vertex_PCTT_Skeleton:
		BindProgramToMesh(progID, Vertex_PCTT_Skeleton::s_EmptyVert);
		break;
	case VBO_SpriteVert:
		BindProgramToMesh(progID, SpriteVert::s_EmptyVert);//SpriteVert::s_EmptyVert);
		break;
	case VBO_Particle2D:
		BindProgramToMesh(progID, Particle2D::s_empty);//SpriteVert::s_EmptyVert);
		break;
	default:
		ASSERT_OR_DIE(false, "YOU DIED!");
		return;
	}
	inMaterial->ActiveTextures(samplerID);

	g_Renderer->RunShaderProgram(m_vaoID, progID, numOfIBOObjects, m_drawMode, m_iboDataType);

	inMaterial->DeactivateTextures();
}

void Mesh::BindProgramToMesh(const unsigned int& progID, const Vertex_PCT&) const
{
	g_Renderer->BindMeshToVAO(m_vaoID , VBO_ID, IBO_ID, progID, Vertex_PCT::s_EmptyVert);

}
void Mesh::BindProgramToMesh(const unsigned int& progID, const Vertex_PCTT&) const
{
	g_Renderer->BindMeshToVAO(m_vaoID, VBO_ID, IBO_ID, progID, Vertex_PCTT::s_EmptyVert);
}
void Mesh::BindProgramToMesh(const unsigned int& progID, const Vertex_PCTT_Skeleton&) const
{
	g_Renderer->BindMeshToVAO(m_vaoID, VBO_ID, IBO_ID, progID, Vertex_PCTT_Skeleton::s_EmptyVert);
}
void Mesh::BindProgramToMesh(const unsigned int& progID, const SpriteVert&) const
{
	g_Renderer->BindMeshToVAO(m_vaoID, VBO_ID, IBO_ID, progID, SpriteVert::s_EmptyVert);
}

void Mesh::BindProgramToMesh(const unsigned int& progID, const Particle2D&) const
{
	g_Renderer->BindMeshToVAO(m_vaoID, VBO_ID, IBO_ID, progID, Particle2D::s_empty);
}

Mesh* Mesh::GeneratePlain(const Vector3& startPoint, const Vector3& rightDir, const Vector3& upDir, const float& rightScale, const float& upScale, const Rgba& flatColor,
	const Vector2& startPointTexCoord, const Vector2& endPointTexCoord)
{
	Mesh* newMesh = new Mesh();
	Vector3 trueRight = rightDir * rightScale;
	Vector3 trueUp = upDir * upScale;
	trueRight.normalize();
	trueUp.normalize();

	std::vector<Vertex_PCTT> verts;
	std::vector<unsigned int> indices;
	verts.push_back(Vertex_PCTT(Vector3(-1.f, 1.f, 0.f) + startPoint, flatColor, Vector2(startPointTexCoord.x, startPointTexCoord.y), 
		trueRight, trueUp));
	verts.push_back(Vertex_PCTT(Vector3(-1.f, -1.f, 0.f) + startPoint, flatColor, Vector2(endPointTexCoord.x, startPointTexCoord.y),
		trueRight, trueUp));
	verts.push_back(Vertex_PCTT(Vector3(1.f, -1.f, 0.f) + startPoint, flatColor, Vector2(endPointTexCoord.x, endPointTexCoord.y), 
		trueRight, trueUp));
	verts.push_back(Vertex_PCTT(Vector3(1.f, 1.f, 0.f) + startPoint, flatColor, Vector2(startPointTexCoord.x, endPointTexCoord.y), 
		trueRight, trueUp));

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);

	newMesh->setVBO(verts);
	newMesh->setIBO(indices);

	return newMesh;
}