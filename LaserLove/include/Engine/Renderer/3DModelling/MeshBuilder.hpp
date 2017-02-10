#pragma once
#include "Engine/Renderer/3DModelling/Mesh.hpp"
#include "Engine/Renderer/RendererEnums.hpp"
#include "Engine/Math/AABB2.hpp"
#include "Engine/Core/IBinaryWriter.hpp"
#include "Engine/Core/IBinaryReader.hpp"
#include "Engine/Console/Command.hpp"
#include "Engine/Console/RegisterCommandHelper.hpp"
#include "Engine/Math/skin_weight_t.hpp"

#ifndef MESHBUILDER_HPP
#define MESHBUILDER_HPP

struct Vertex_PCT;
struct Vertex_PCTT;
struct Vertex_PCTT_Skeleton;

enum eMeshDataFlag
{
	eMeshData_Position = 0,
	eMeshData_Color,
	eMeshData_TexCoord,
	eMeshData_Tangent,
	eMeshData_Bitangent,
	eMeshData_Normal,
	eMeshData_Bone_Weights_and_Indices,
	eMeshData_COUNT
};

struct Master_Vertex
{
	static const unsigned int MAX_UV = 100;
	Master_Vertex();
	~Master_Vertex();
	Vector3 position;
	Rgba color;
	Vector2 texCoord[MAX_UV]; //Could potentially have more than one UV
	Vector3 tangent;
	Vector3 bitangent;
	Vector3 normal; 
	skin_weight_t bone_weights_and_indicies;
	unsigned int CountOfTexCoords;
};
struct plane_data_t
{
	Vector3 initial_position;
	Vector3 right;
	Vector3 up;
};

typedef Vector3(*patch_func_cb)(void const*, float x, float y);

static Vector3 PlaneFunc(void const *user_arg, float x, float y);

class MeshBuilder;

extern MeshBuilder* g_builder;

void SaveMesh(Command& com);
void LoadMesh(Command& com);

struct IOBinLoadBoolHolder
{
	IOBinLoadBoolHolder();
	bool loadedFromBinFile = false;
	bool loadedOrSavePosition = true;
	bool loadedOrSaveColor = true;
	bool loadedOrSaveTexCoord = true;
	bool loadedOrSaveTangent = true;
	bool loadedOrSaveBitangent = true;
	bool loadedOrSaveNormal = true;
	bool loadedOrSaveBoneWeightAndIndices = true;
};

class MeshBuilder
{
private:
	static const std::string meshBuilder_posStr;
	static const std::string meshBuilder_colStr;
	static const std::string meshBuilder_texStr;
	static const std::string meshBuilder_tanStr;
	static const std::string meshBuilder_bitStr;
	static const std::string meshBuilder_norStr;
	static const std::string meshBuilder_boneWAIStr;

private:
	//variables
	Master_Vertex m_stamp;
	std::vector<Master_Vertex> m_vertices;
	std::vector<unsigned int> m_indices;
	bool m_buildTrianglesAsIfConstructingQuads;
	bool m_CurrentlyBuilding;
	bool m_useIndexBuffer;
	IOBinLoadBoolHolder m_IOBinInfo;
	PrimitiveType m_drawMode;

	//Versioning.
	//1: Initial Version
	static const uint32_t FILE_VERSION = 2;
	//Increased file version to 2 so as to support boneweights, bone indices, and multiple texcoords

public:
	//--------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------
	//Constructors
	MeshBuilder();
	~MeshBuilder();

	//--------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------
	//Stamp specific
	void AddTriangleIndicesIf3Points();
	const std::vector<Master_Vertex> GetVertices() const;
	const std::vector<unsigned int> GetIndices() const;
	const std::string GetMaterialID() const;
	const unsigned int GetDataMask() const;
	void AddPointsAndIndices(const MeshBuilder* other);
	void Start(const PrimitiveType& toDrawAs = PRIMITIVE_TRIANGLES, const bool& useIndexBuffer = false);
	void End();
	const int GetCurrentIndex() const;
	void SetBuildTrianglesForQuads(const bool& buildTrianglesForQuads);
	void AddVertex(const Vector3& position);
	void SetPosition(const Vector3& position);
	void SetColor(const Rgba& color);
	void SetTexCoord0(const Vector2& texCoord);
	const bool SetTexCoords(const Vector2*& texCoords, const unsigned int& countOfCoords);
	void ResetTexCoords();
	void SetTangent(const Vector3& tangent);
	void SetBitangent(const Vector3& bitangent);
	void SetNormal(const Vector3& normal);

	//boneWeights
	void SetBoneWeightsAndIndices(const Vector4& boneW, const UnsignedIntVector4& boneI);
	void SetBoneWeight(const Vector4& boneW);
	void SetBoneIndices(const UnsignedIntVector4& boneI);
	void NormalizeBoneWeights();
	void SetBoneWeightsAndIndicesDefault();
	void SetUVCount(const unsigned int& count);
	void AddIndice(const unsigned int& indice);

	void ClearMeshBuilder();

	//--------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------
	//Getters
	const bool is_skinned() const;

	//Copying properties
	void CopyOnToMeshVertexPCT(Mesh* copyTo);
	void CopyOnToMeshVertexPCTT(Mesh* copyTo);
	void CopyOnToMeshVertexPCTTSkeleton(Mesh* copyTo);
	void CopyOnToMeshSpriteVert(Mesh* copyTo);


	//--------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------
	//Read and Write to Bin file specific
	void SetSavePosition(const bool& posSave);
	void SetSaveColor(const bool& colSave);
	void SetSaveTexCoord(const bool& texSave);
	void SetSaveTangent(const bool& tanSave);
	void SetSaveBitangent(const bool& bitSave);
	void SetSaveNormal(const bool& norSave);
	void SetSaveBoneWeightsAndIndices(const bool& boneWAISave);

	void WriteToFile(const char* filename);
private:
	void WriteToStream(IBinaryWriter& writer);
	void WriteDataMask(IBinaryWriter& writer, uint32_t mask);
	void WriteVertices(IBinaryWriter& writer, const std::vector<Master_Vertex>& m_vertices, const unsigned int& data_mask);
	void WriteIndices(IBinaryWriter& writer, const std::vector<unsigned int>& m_indices);
public:
	//void WriteDrawInstructions(IBinaryWriter& writer, draw_instructions);

	void ReadFromFile(const char* filename);
private:
	void ReadFromStream(IBinaryReader& reader);
	const unsigned int ReadDataMask(IBinaryReader& reader);
	void ReadVertices(IBinaryReader& reader, const unsigned int& data_mask, const unsigned int& numVerts, const unsigned int& version);
	void ReadIndices(IBinaryReader& reader, const unsigned int& numInd);

public:
	//--------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------
	//Simple Build copyable Mesh
	void BuildCube(const Vector3& size, const Rgba& solidColor = Rgba::s_White);
	void BuildInvertedCube(const Vector3& size, const Rgba& solidColor = Rgba::s_White);
	void BuildSkyBox(const Vector3& size, const Rgba& solidColor = Rgba::s_White,
		const AABB2& WestTexCoords = AABB2(0.75f, 1.f / 3.f, 1.0f, 2.f / 3.f), const AABB2& EastTexCoords = AABB2(0.25f, 1.f / 3.f, 0.5f, 2.f / 3.f),
		const AABB2& NorthTexCoords = AABB2(0.f, 1.f / 3.f, 0.25f, 2.f / 3.f), const AABB2& SouthTexCoords = AABB2(0.5f, 1.f / 3.f, 0.75f, 2.f / 3.f),
		const AABB2& TopTexCoords = AABB2(0.25f, 0.f, 0.5f, 1.f / 3.f), const AABB2& BottomTexCoords = AABB2(0.25f, 2.f / 3.f, 0.5f, 1.f));

	void BuildSphere(const float& radius = 1.f, const Rgba& solidColor = Rgba::s_White);
	void BuildCircle(const float& radius = 1.f, const Rgba& solidColor = Rgba::s_White, const Vector3& up = Vector3(0.f, 1.f, 0.f), const Vector3& right = Vector3(1.f, 0.f, 0.f));
	void BuildPlane(const Vector3& Up = Vector3(0.f, 1.f, 0.f), const Vector3& Right = Vector3(1.f, 0.f, 0.f), const float& length = 1.f,
		const float& width = 1.f, const int& numXDivisions = 1, const int& numYDivisions = 1, const Rgba& solidColor = Rgba(1.f, 1.f, 1.f, 1.f));


	static void MeshBuildPatch(MeshBuilder *mb,
		float start_x, float end_x, uint32_t x_sections,
		float start_y, float end_y, uint32_t y_sections,
		patch_func_cb patch_func,
		void *user_data);


	//--------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------
	//Simple Mesh Builds
	//plain
	static Mesh* FastBuildSinglePlain(const Vector3& Up = Vector3(0.f, 1.f, 0.f), const Vector3& Right = Vector3(1.f,0.f,0.f), const float& length = 1.f,
		const float& width = 1.f, const Rgba& solidColor = Rgba(1.f,1.f,1.f,1.f));

	//sphere
	static Mesh* FastBuildSingleSphere(const float& xyRadius, const float& yzRadius, const Rgba& solidColor = Rgba(1.f, 1.f, 1.f, 1.f));
	static Mesh* FastBuildSingleSphere(const float& radius = 1.f, const Rgba& solidColor = Rgba(1.f, 1.f, 1.f, 1.f));

	//cube
	static Mesh* FastBuildSingleCube(const Vector3& lengthWidthHeight = Vector3(1.f,1.f,1.f), const Rgba& solidColor = Rgba(1.f, 1.f, 1.f, 1.f));
	static Mesh* FastBuildSingleInvertedCube(const Vector3& lengthWidthHeight, const Rgba& solidColor = Rgba(1.f, 1.f, 1.f, 1.f));
	static Mesh* FastBuildSingleSkyBox(const Vector3& lengthWidthHeight, const Rgba& solidColor = Rgba(1.f, 1.f, 1.f, 1.f),
		const AABB2& WestTexCoords = AABB2(0.75f, 1.f / 3.f, 1.0f, 2.f / 3.f), const AABB2& EastTexCoords = AABB2(0.25f, 1.f / 3.f, 0.5f, 2.f / 3.f),
		const AABB2& NorthTexCoords = AABB2(0.f, 1.f / 3.f, 0.25f, 2.f / 3.f), const AABB2& SouthTexCoords = AABB2(0.5f, 1.f / 3.f, 0.75f, 2.f / 3.f),
		const AABB2& TopTexCoords = AABB2(0.25f, 0.f, 0.5f, 1.f / 3.f), const AABB2& BottomTexCoords = AABB2(0.25f, 2.f / 3.f, 0.5f, 1.f));


private:
	//--------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------
	//Fast Single Cube Specific
	//Simple Mesh Builds
	void addQuadIndices(unsigned int var1, unsigned int var2, unsigned int var3, unsigned int var4);
	static void addSixIndices(std::vector<unsigned int>& triangularIndices, unsigned int var1, unsigned int var2, unsigned int var3, unsigned int var4, unsigned int var5, unsigned int var6);
	static void setSixIndices(std::vector<unsigned int>& triangularIndices, const unsigned int& startIndex, unsigned int var1, unsigned int var2, unsigned int var3, unsigned int var4, unsigned int var5, unsigned int var6);
public:
	static const std::vector<unsigned int> GetCubeTriangularIndices();
private:
	static void getBottomSideQuadTang(std::vector<Vertex_PCTT>& quads, const Vector3& bottomLeftCorner, const Vector3& size, const Rgba& tint, const AABB2& texCoords);
	static void getTopSideQuadTang(std::vector<Vertex_PCTT>& quads, const Vector3& bottomLeftCorner, const Vector3& size, const Rgba& tint, const AABB2& texCoords);
	static void getNorthSideQuadTang(std::vector<Vertex_PCTT>& quads, const Vector3& bottomLeftCorner, const Vector3& size, const Rgba& tint, const AABB2& texCoords);
	static void getSouthSideQuadTang(std::vector<Vertex_PCTT>& quads, const Vector3& bottomLeftCorner, const Vector3& size, const Rgba& tint, const AABB2& texCoords);
	static void getWestSideQuadTang(std::vector<Vertex_PCTT>& quads, const Vector3& bottomLeftCorner, const Vector3& size, const Rgba& tint, const AABB2& texCoords);
	static void getEastSideQuadTang(std::vector<Vertex_PCTT>& quads, const Vector3& bottomLeftCorner, const Vector3& size, const Rgba& tint, const AABB2& texCoords);

	static void getInvertedBottomSideQuadTang(std::vector<Vertex_PCTT>& quads, const Vector3& bottomLeftCorner, const Vector3& size, const Rgba& tint, const AABB2& texCoords);
	static void getInvertedTopSideQuadTang(std::vector<Vertex_PCTT>& quads, const Vector3& bottomLeftCorner, const Vector3& size, const Rgba& tint, const AABB2& texCoords);
	static void getInvertedNorthSideQuadTang(std::vector<Vertex_PCTT>& quads, const Vector3& bottomLeftCorner, const Vector3& size, const Rgba& tint, const AABB2& texCoords);
	static void getInvertedSouthSideQuadTang(std::vector<Vertex_PCTT>& quads, const Vector3& bottomLeftCorner, const Vector3& size, const Rgba& tint, const AABB2& texCoords);
	static void getInvertedWestSideQuadTang(std::vector<Vertex_PCTT>& quads, const Vector3& bottomLeftCorner, const Vector3& size, const Rgba& tint, const AABB2& texCoords);
	static void getInvertedEastSideQuadTang(std::vector<Vertex_PCTT>& quads, const Vector3& bottomLeftCorner, const Vector3& size, const Rgba& tint, const AABB2& texCoords);


	//--------------------------------------------------------------------------------------------------------------------
	//Simple Build copyable Mesh
	void getBottomSideQuadTangMasterVertex(std::vector<Master_Vertex>& quads, const Vector3& size, const AABB2& texCoords);
	void getTopSideQuadTangMasterVertex(std::vector<Master_Vertex>& quads, const Vector3& size, const AABB2& texCoords);
	void getNorthSideQuadTangMasterVertex(std::vector<Master_Vertex>& quads, const Vector3& size, const AABB2& texCoords);
	void getSouthSideQuadTangMasterVertex(std::vector<Master_Vertex>& quads, const Vector3& size, const AABB2& texCoords);
	void getWestSideQuadTangMasterVertex(std::vector<Master_Vertex>& quads, const Vector3& size, const AABB2& texCoords);
	void getEastSideQuadTangMasterVertex(std::vector<Master_Vertex>& quads, const Vector3& size, const AABB2& texCoords);

	void getInvertedBottomSideQuadTangMasterVertex(std::vector<Master_Vertex>& quads, const Vector3& size, const AABB2& texCoords);
	void getInvertedTopSideQuadTangMasterVertex(std::vector<Master_Vertex>& quads, const Vector3& size, const AABB2& texCoords);
	void getInvertedNorthSideQuadTangMasterVertex(std::vector<Master_Vertex>& quads, const Vector3& size, const AABB2& texCoords);
	void getInvertedSouthSideQuadTangMasterVertex(std::vector<Master_Vertex>& quads, const Vector3& size, const AABB2& texCoords);
	void getInvertedWestSideQuadTangMasterVertex(std::vector<Master_Vertex>& quads, const Vector3& size, const AABB2& texCoords);
	void getInvertedEastSideQuadTangMasterVertex(std::vector<Master_Vertex>& quads, const Vector3& size, const AABB2& texCoords);
};
#endif