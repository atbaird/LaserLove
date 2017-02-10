#pragma once
#include "Engine/Renderer/RendererEnums.hpp"
#include "Engine/Renderer/3DModelling/Material.hpp"
#include <vector>
#include "Engine/Renderer/3DModelling/Skeleton.hpp"

#ifndef MESH_HPP
#define MESH_HPP

struct Vertex_PCT;
struct Vertex_PCTT;
struct Vertex_PCTT_Skeleton;
struct SpriteVert;
class Particle2D;

enum VBO_TYPE
{
	INVALID_VBO_TYPE = -1,
	VBO_Vertex_PCT,
	VBO_Vertex_PCTT,
	VBO_Vertex_PCTT_Skeleton,
	VBO_SpriteVert,
	VBO_Particle2D,
	NUM_OF_VBO_TYPES
};

class Mesh
{
private:
	bool m_debugSkeleton;
	unsigned int samplerID;
	unsigned int VBO_ID;
	unsigned int IBO_ID;
	unsigned int m_vaoID;
	unsigned int numOfVBOObjects;
	unsigned int numOfIBOObjects;
	Skeleton* m_skeleton;
	PrimitiveType m_drawMode;
	DataType m_iboDataType;
	VBOusage m_drawUsage;
	VBO_TYPE VBOType;
public:
	//Constructors
	Mesh(const unsigned int& sampleID = 0, const PrimitiveType& drawMode = PRIMITIVE_TRIANGLES,
		const DataType& iboDataType = INVALID_DATA_TYPE, const VBOusage& drawUsage = STATIC_DRAW,
		const unsigned int& vaoID = 0);
	template<typename T>
	Mesh(const unsigned int& sampleID, const unsigned int& vaoID, const std::vector<Vertex_PCT>* model,
		const std::vector<T>* ibos, PrimitiveType drawMode, const DataType& iboDataType, const VBOusage& drawUsage);
	~Mesh();

	//Setters
	void SetDebugSkeleton(const bool& debug);
	void setDrawMode(const PrimitiveType& drawMode);
	void setVBOAndIBO(const std::vector<Vertex_PCT>& model, const std::vector<unsigned int>& ibos);
	void setIBO(const std::vector<unsigned int>& ibos);
	void setVBO(const std::vector<Vertex_PCT>& model);
	void setVBO(const std::vector<Vertex_PCTT>& model);
	void setVBO(const std::vector<Vertex_PCTT_Skeleton>& model);
	void setVBO(const std::vector<SpriteVert>& model);
	void setVBO(const std::vector<Particle2D>& model);
	void setVBOWithNoInternalAlterations(const std::vector<Particle2D>& model) const;
	void setIBOWithNoInternalAlterations(const std::vector<Particle2D>& model) const;
	void setVAOID(const unsigned int& id) { m_vaoID = id; };
	void CreateVAOID();
	void SetSamplerID(const unsigned int& sampleID);
	void SetSkeleton(Skeleton* skele);
	void PregenerateVBOAndIBO();

	//Getters
	const unsigned int getVBOID() const { return VBO_ID; };
	const unsigned int getIBOID() const { return IBO_ID; };
	const unsigned int getVAOID() const { return m_vaoID; };
	const unsigned int getNumOfVBOs() const { return numOfVBOObjects; };
	const unsigned int getNumOfIBOs() const { return numOfIBOObjects; };
	const unsigned int getSamplerID() const { return samplerID; };
	const PrimitiveType getDrawMode() const { return m_drawMode; };
	const DataType getIBODataType() const { return m_iboDataType; };
	const VBOusage getDrawUsage() const { return m_drawUsage; };

	//Operators
	void DrawWith(const Material* inMaterial) const;
	void BindProgramToMesh(const unsigned int& progID, const Vertex_PCT&) const;
	void BindProgramToMesh(const unsigned int& progID, const Vertex_PCTT&) const;
	void BindProgramToMesh(const unsigned int& progID, const Vertex_PCTT_Skeleton&) const;
	void BindProgramToMesh(const unsigned int& progID, const SpriteVert&) const;
	void BindProgramToMesh(const unsigned int& progID, const Particle2D&) const;

	static Mesh* GeneratePlain(const Vector3& startPoint = Vector3(0.1f, -0.5f, -0.5f),
		const Vector3& rightDir = Vector3(0.f, -1.f, 0.f), const Vector3& upDir = Vector3(0.f, 0.f, 1.f),
		const float& rightScale = 1.f, const float& upScale = 1.f,
		const Rgba& flatColor = Rgba(1.f, 1.f, 1.f, 1.f), const Vector2& startPointTexCoord = Vector2(0.f, 0.f),
		const Vector2& endPointTexCoord = Vector2(1.f, 1.f));

};
#endif