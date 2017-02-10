#include "Engine/Renderer/3DModelling/MeshBuilder.hpp"
#include "Engine/Math/ExtendedMath.hpp"
#include "Engine/Core/FileBinaryWriter.hpp"
#include "Engine/Core/FileBinaryReader.hpp"
#include "Engine/Math/Vertices/Vertex_PCT.hpp"
#include "Engine/Math/Vertices/Vertex_PCTT.hpp"
#include "Engine/Math/Vertices/Vertex_PCTT_Skeleton.hpp"
#include "Engine/Math/Vertices/Vertex_UI.hpp"
#include "Engine/2D/SpriteVert.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

MeshBuilder* g_builder = nullptr;

void SaveMesh(Command& com)
{
	std::string fileLoc = com.get_string_by_index(0);
	if (fileLoc == "" || g_builder == nullptr)
	{
		return;
	}
	g_builder->WriteToFile(fileLoc.c_str());
}
void LoadMesh(Command& com)
{
	std::string fileLoc = com.get_string_by_index(0);
	if (fileLoc == "" || g_builder == nullptr)
	{
		return;
	}
	g_builder->ReadFromFile(fileLoc.c_str());
}

//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------
//constructors

Master_Vertex::Master_Vertex()
	: position(0.f, 0.f, 0.f),
	color(0.f, 0.f, 0.f, 1.f),
	tangent(0.f, 0.f, 0.f),
	bitangent(0.f, 0.f, 0.f),
	normal(0.f, 0.f, 0.f),
	CountOfTexCoords(0)
{

	bone_weights_and_indicies.bone_weights = Vector4(1.f, 0.f, 0.f, 0.f);
	bone_weights_and_indicies.bone_indices = UnsignedIntVector4(0, 0, 0, 0);
	for (size_t i = 0; i < MAX_UV; i++)
	{
		texCoord[i] = Vector2(0.f, 0.f);
	}
}
Master_Vertex::~Master_Vertex()
{

}

MeshBuilder::MeshBuilder()
	: m_buildTrianglesAsIfConstructingQuads(false),
	m_useIndexBuffer(false),
	m_CurrentlyBuilding(false),
	m_drawMode(PRIMITIVE_TRIANGLES)
{
}
MeshBuilder::~MeshBuilder()
{
}


IOBinLoadBoolHolder::IOBinLoadBoolHolder()
	:loadedFromBinFile(false),
	loadedOrSavePosition(true),
	loadedOrSaveColor(true),
	loadedOrSaveTexCoord(true),
	loadedOrSaveTangent(true),
	loadedOrSaveBitangent(true),
	loadedOrSaveNormal(true)
{

}

Vector3 PlaneFunc(void const *user_arg, float x, float y)
{
	plane_data_t const *plane = (plane_data_t const*)user_arg;
	Vector3 position = plane->initial_position
		+ (plane->right * x)
		+ (plane->up * y);

	return position;
}

//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------
//Stamp specific


void MeshBuilder::AddTriangleIndicesIf3Points()
{
	if (m_vertices.size() % 3 == 0)
	{
		m_indices.push_back(m_vertices.size() - 3);
		m_indices.push_back(m_vertices.size() - 2);
		m_indices.push_back(m_vertices.size() - 1);
	}
}
void MeshBuilder::GetVertices(std::vector<Master_Vertex>& verts) const
{ 
	verts.insert(verts.end(), m_vertices.begin(), m_vertices.end());
}

void MeshBuilder::GetVertices(std::vector<Vertex_PCT>& verts) const
{
	verts.clear();
	for (size_t i = 0; i < m_vertices.size(); i++)
	{
		Master_Vertex vert = m_vertices[i];
		Vertex_PCT outVert;
		outVert.m_pos = vert.position;
		outVert.m_color = vert.color;
		outVert.m_texCoords = vert.texCoord[0];
		verts.push_back(outVert);
	}
}

void MeshBuilder::GetVertices(std::vector<SpriteVert>& verts) const
{
	verts.clear();
	for (size_t i = 0; i < m_vertices.size(); i++)
	{
		Master_Vertex vert = m_vertices[i];
		SpriteVert outVert;
		outVert.m_pos = vert.position.GetXY();
		outVert.m_color = vert.color;
		outVert.m_texCoords = vert.texCoord[0];
		verts.push_back(outVert);
	}
}

void MeshBuilder::GetPositionDataOnly(std::vector<Vector3>& verts) const
{
	verts.resize(m_vertices.size());
	for (size_t i = 0; i < m_vertices.size(); i++)
	{
		verts[i] = m_vertices.at(i).position;
	}
}

void MeshBuilder::GetFirstTexCoordDataOnly(std::vector < Vector2>& texCoords) const
{
	texCoords.resize(m_vertices.size());
	for (size_t i = 0; i < m_vertices.size(); i++)
	{
		texCoords[i] = m_vertices.at(i).texCoord[0];
	}
}

void MeshBuilder::GetIndices(std::vector<unsigned int>& indices) const
{
	if (indices.size() == 0)
	{
		for (size_t i = 0; i < m_indices.size(); i++)
		{
			indices.push_back(m_indices.at(i));
		}
		//indices.insert(indices.begin(), m_indices.begin(), m_indices.end());
	}
	else
	{
		indices.insert(indices.end(), m_indices.begin(), m_indices.end());
	}
}

int MeshBuilder::GetNumberOfVertices() const
{
	return m_vertices.size();
}

void MeshBuilder::AddPointsAndIndices(const MeshBuilder* other)
{

	std::vector<Master_Vertex> vertices;
	other->GetVertices(vertices);
	std::vector<unsigned int> indices;
	other->GetIndices(indices);
	size_t startIndex = m_vertices.size();

	m_vertices.insert(m_vertices.end(), vertices.begin(), vertices.end());

	for (size_t i = 0; i < indices.size(); i++)
	{
		unsigned int indice = indices.at(i) + startIndex;
		m_indices.push_back(indice);
	}
}
void MeshBuilder::Start(const PrimitiveType& toDrawAs, const bool& useIndexBuffer)
{
	m_useIndexBuffer = useIndexBuffer;
	m_drawMode = toDrawAs;
	m_CurrentlyBuilding = true;
}
void MeshBuilder::End()
{
	m_CurrentlyBuilding = false;
}
const int MeshBuilder::GetCurrentIndex() const
{ 
	return m_vertices.size(); 
}
void MeshBuilder::SetBuildTrianglesForQuads(const bool& buildTrianglesForQuads)
{
	m_buildTrianglesAsIfConstructingQuads = buildTrianglesForQuads;
}
void MeshBuilder::AddVertex(const Vector3& position)
{
	if (m_CurrentlyBuilding == true)
	{
		Master_Vertex vert = m_stamp;
		vert.position = position;
		m_vertices.push_back(vert);
	}
}
void MeshBuilder::SetPosition(const Vector3& position)
{
	m_stamp.position = position;
}
void MeshBuilder::SetColor(const Rgba& color)
{
	m_stamp.color = color;
}
void MeshBuilder::SetTexCoord0(const Vector2& texCoord)
{
	if (m_stamp.CountOfTexCoords <= 0)
	{
		m_stamp.CountOfTexCoords = 1;
	}
	m_stamp.texCoord[0] = texCoord;
}
const bool MeshBuilder::SetTexCoords(const Vector2*& texCoords, const unsigned int& countOfCoords)
{
	unsigned int count = countOfCoords;
	if (countOfCoords > Master_Vertex::MAX_UV)
	{
		count = Master_Vertex::MAX_UV;
	}

	for (unsigned int i = 0; i < count; i++)
	{
		m_stamp.texCoord[i] = texCoords[i];
	}
	if (m_stamp.CountOfTexCoords < count)
	{
		m_stamp.CountOfTexCoords = count;
	}

	if (countOfCoords > Master_Vertex::MAX_UV)
	{
		//Did not copy all
		return false;
	}

	//did copy all
	return true;
}
void MeshBuilder::ResetTexCoords()
{
	for (unsigned int i = 0; i < Master_Vertex::MAX_UV; i++)
	{
		m_stamp.texCoord[i] = Vector2(0.f, 0.f);
	}
	m_stamp.CountOfTexCoords = 0;
}
void MeshBuilder::SetTangent(const Vector3& tangent)
{
	m_stamp.tangent = tangent;
}
void MeshBuilder::SetBitangent(const Vector3& bitangent)
{
	m_stamp.bitangent = bitangent;
}
void MeshBuilder::SetNormal(const Vector3& normal)
{
	m_stamp.normal = normal;
}
void MeshBuilder::AddIndice(const unsigned int& indice)
{
	m_indices.push_back(indice);
}

//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------
//boneWeights
void MeshBuilder::SetBoneWeightsAndIndices(const Vector4& boneW, const UnsignedIntVector4& boneI)
{
	SetBoneWeight(boneW);
	SetBoneIndices(boneI);
}
void MeshBuilder::SetBoneWeight(const Vector4& boneW)
{
	m_stamp.bone_weights_and_indicies.bone_weights = boneW;
}
void MeshBuilder::SetBoneIndices(const UnsignedIntVector4& boneI)
{
	m_stamp.bone_weights_and_indicies.bone_indices = boneI;
}
void MeshBuilder::NormalizeBoneWeights()
{
	m_stamp.bone_weights_and_indicies.bone_weights.normalize();
}
void MeshBuilder::SetBoneWeightsAndIndicesDefault()
{
	SetBoneWeight(Vector4(1.f, 0.f, 0.f, 0.f));
	SetBoneIndices(UnsignedIntVector4(0, 0, 0, 0));
}
void MeshBuilder::SetUVCount(const unsigned int& count)
{
	if (count > Master_Vertex::MAX_UV)
	{
		m_stamp.CountOfTexCoords = Master_Vertex::MAX_UV;
	}
	else
	{
		m_stamp.CountOfTexCoords = count;
	}
}

void MeshBuilder::ClearMeshBuilder()
{

	m_vertices.clear();
	m_indices.clear();
	//m_vertices.empty();
	//m_indices.empty();
}

//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------
//Getters
const bool MeshBuilder::IsSkinned() const
{
	return false;
}

PrimitiveType MeshBuilder::GetDrawMode() const
{
	return m_drawMode;
}

const std::string MeshBuilder::GetMaterialID() const
{
	return "";
}
const unsigned int MeshBuilder::GetDataMask() const
{
	unsigned int mask = 0;

	if (m_IOBinInfo.loadedOrSavePosition)
	{
		mask += (1 << eMeshData_Position);
	}
	if (m_IOBinInfo.loadedOrSaveColor)
	{
		mask += (1 << eMeshData_Color);
	}
	if (m_IOBinInfo.loadedOrSaveTexCoord)
	{
		mask += (1 << eMeshData_TexCoord);
	}
	if (m_IOBinInfo.loadedOrSaveTangent)
	{
		mask += (1 << eMeshData_Tangent);
	}
	if (m_IOBinInfo.loadedOrSaveBitangent)
	{
		mask += (1 << eMeshData_Bitangent);
	}
	if (m_IOBinInfo.loadedOrSaveNormal)
	{
		mask += (1 << eMeshData_Normal);
	}
	if (m_IOBinInfo.loadedOrSaveBoneWeightAndIndices)
	{
		mask += (1 << eMeshData_Bone_Weights_and_Indices);
	}
	return mask;
}




//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------
//Copying properties

const std::string MeshBuilder::meshBuilder_posStr = "position";
const std::string MeshBuilder::meshBuilder_colStr = "color";
const std::string MeshBuilder::meshBuilder_texStr = "texcoord";
const std::string MeshBuilder::meshBuilder_tanStr = "tangent";
const std::string MeshBuilder::meshBuilder_bitStr = "bitangent";
const std::string MeshBuilder::meshBuilder_norStr = "normal";
const std::string MeshBuilder::meshBuilder_boneWAIStr = "boneWAI";

void MeshBuilder::CopyOnToMeshVertexPCT(Mesh* copyTo)
{
	if (copyTo == nullptr)
	{
		ERROR_AND_DIE("You tried to copy stuff onto a null Mesh!");
		return;
	}
	std::vector<Vertex_PCT> verticesToPlug;
	//m_vertices
	for (size_t index = 0; index < m_vertices.size(); index++)
	{
		Master_Vertex vertex = m_vertices.at(index);
		verticesToPlug.push_back(Vertex_PCT(vertex.position, vertex.color, vertex.texCoord[0]));
	}
	copyTo->setVBO(verticesToPlug);
	copyTo->setIBO(m_indices);
	copyTo->SetDrawMode(m_drawMode);
	if (copyTo->getVAOID() == 0)
	{
		copyTo->CreateVAOID();
	}
}

void MeshBuilder::CopyOnToMeshVertexPCTT(Mesh* copyTo)
{
	if (copyTo == nullptr)
	{
		ERROR_AND_DIE("You tried to copy stuff onto a null Mesh!");
		return;
	}
	std::vector<Vertex_PCTT> verticesToPlug;
	//m_vertices
	for (size_t index = 0; index < m_vertices.size(); index++)
	{
		Master_Vertex vertex = m_vertices.at(index);
		verticesToPlug.push_back(Vertex_PCTT(vertex.position, vertex.color, vertex.texCoord[0], vertex.tangent, vertex.bitangent, vertex.normal));
	}
	copyTo->setVBO(verticesToPlug);
	copyTo->setIBO(m_indices);
	copyTo->SetDrawMode(m_drawMode);
	if (copyTo->getVAOID() == 0)
	{
		copyTo->CreateVAOID();
	}
}

void MeshBuilder::CopyOnToMeshVertexPCTTSkeleton(Mesh* copyTo)
{
	if (copyTo == nullptr)
	{
		ERROR_AND_DIE("You tried to copy stuff onto a null Mesh!");
		return;
	}
	std::vector<Vertex_PCTT_Skeleton> verticesToPlug;
	for (size_t index = 0; index < m_vertices.size(); index++)
	{
		Master_Vertex vertex = m_vertices.at(index);
		verticesToPlug.push_back(Vertex_PCTT_Skeleton(vertex.position, vertex.color, vertex.texCoord[0], 
			vertex.tangent, vertex.bitangent, vertex.normal,
			vertex.bone_weights_and_indicies));
	}
	copyTo->setVBO(verticesToPlug);
	copyTo->setIBO(m_indices);
	copyTo->SetDrawMode(m_drawMode);
	if (copyTo->getVAOID() == 0)
	{
		copyTo->CreateVAOID();
	}
}


void MeshBuilder::CopyOnToMeshVertexUI(Mesh* copyTo, std::vector<Vertex_UI>& model)
{
	if (copyTo == nullptr)
	{
		ERROR_AND_DIE("You tried to copy stuff onto a null Mesh!");
		return;
	}
	model.clear();

}

void MeshBuilder::CopyOnToMeshSpriteVert(Mesh* copyTo, std::vector<SpriteVert>& verticesToPlug, bool useCustomTexCoords, const AABB2& customTexCoords)
{
	if (copyTo == nullptr)
	{
		ERROR_AND_DIE("You tried to copy stuff onto a null Mesh!");
		return;
	}
	verticesToPlug.clear();
	Vector2 texCoordsBL = m_vertices.at(0).texCoord[0]; //bl
	Vector2 texCoordsTR = m_vertices.at(m_vertices.size() - 1).texCoord[0]; //tr
	Vector2 diff = texCoordsTR - texCoordsBL;
	Vector2 copyDiff = Vector2(abs(customTexCoords.maxs.x - customTexCoords.mins.x), abs(customTexCoords.maxs.y - customTexCoords.mins.y)); //Danger zone?
	Vector2 intendedMins = customTexCoords.mins;

	for (size_t index = 0; index < m_vertices.size(); index++)
	{
		Master_Vertex vertex = m_vertices.at(index);
		Vector2 texCoords = vertex.texCoord[0];

		if (useCustomTexCoords)
		{
			texCoords.x = (((texCoords.x - texCoordsBL.x) / diff.x) * copyDiff.x) + intendedMins.x;
			texCoords.y = (((texCoords.y - texCoordsBL.y) / diff.y) * copyDiff.y) + intendedMins.y;
		}

		verticesToPlug.push_back(SpriteVert(vertex.position.GetXY(), vertex.color, texCoords));
	}
	copyTo->setVBO(verticesToPlug);
	copyTo->setIBO(m_indices);
	copyTo->SetDrawMode(m_drawMode);
	if (copyTo->getVAOID() == 0)
	{
		copyTo->CreateVAOID();
	}
}


//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//Read from or Write to bin file specific

void MeshBuilder::SetSavePosition(const bool& posSave)
{
	m_IOBinInfo.loadedOrSavePosition = posSave;
}
void MeshBuilder::SetSaveColor(const bool& colSave)
{
	m_IOBinInfo.loadedOrSaveColor = colSave;
}
void MeshBuilder::SetSaveTexCoord(const bool& texSave)
{
	m_IOBinInfo.loadedOrSaveTexCoord = texSave;
}
void MeshBuilder::SetSaveTangent(const bool& tanSave)
{
	m_IOBinInfo.loadedOrSaveTangent = tanSave;
}
void MeshBuilder::SetSaveBitangent(const bool& bitSave)
{
	m_IOBinInfo.loadedOrSaveBitangent = bitSave;
}
void MeshBuilder::SetSaveNormal(const bool& norSave)
{
	m_IOBinInfo.loadedOrSaveNormal = norSave;
}
void MeshBuilder::SetSaveBoneWeightsAndIndices(const bool& boneWSave)
{
	m_IOBinInfo.loadedOrSaveBoneWeightAndIndices = boneWSave;
}


void MeshBuilder::WriteToFile(const char* filename)
{
	FileBinaryWriter writer;
	writer.open(filename);
	//Data I want to Write


	WriteToStream(writer);
	writer.close();
}
void MeshBuilder::WriteToStream(IBinaryWriter &writer)
{
	// FILE VERSION
	// material id
	// vertex data mask(ie. position, tangent, normal, etc...)
	// vertices
	// indices
	// draw_instructions

	writer.Write<unsigned int>(FILE_VERSION);
	writer.WriteString(GetMaterialID().c_str());

	unsigned int vertex_count = m_vertices.size();
	unsigned int index_count = m_indices.size();
	//unsigned int draw_instruction_count = draw_instructions.size();

	writer.Write<unsigned int>(vertex_count);
	writer.Write<unsigned int>(index_count);
	//writer.Write<unsigned int>(draw_instruction_count);

	unsigned int data_mask = GetDataMask();
	WriteDataMask(writer, data_mask);
	WriteVertices(writer, m_vertices, data_mask);
	WriteIndices(writer, m_indices);
	//WriteDrawInstructions(writer, draw_instructions);
}
void MeshBuilder::WriteDataMask(IBinaryWriter& writer, uint32_t mask)
{
	writer.Write<unsigned int>(eMeshData_COUNT);

	if ((mask & (1 << eMeshData_Position)) != 0)
	{
		writer.WriteString(meshBuilder_posStr.c_str());
	}
	if ((mask & (1 << eMeshData_Color)) != 0)
	{
		writer.WriteString(meshBuilder_colStr.c_str());
	}
	if ((mask & (1 << eMeshData_TexCoord)) != 0)
	{
		writer.WriteString(meshBuilder_texStr.c_str());
	}
	if ((mask & (1 << eMeshData_Tangent)) != 0)
	{
		writer.WriteString(meshBuilder_tanStr.c_str());
	}
	if ((mask & (1 << eMeshData_Bitangent)) != 0)
	{
		writer.WriteString(meshBuilder_bitStr.c_str());
	}
	if ((mask & (1 << eMeshData_Normal)) != 0)
	{
		writer.WriteString(meshBuilder_norStr.c_str());
	}
	if ((mask & (1 << eMeshData_Bone_Weights_and_Indices)) != 0)
	{
		writer.WriteString(meshBuilder_boneWAIStr.c_str());
	}
}
void MeshBuilder::WriteVertices(IBinaryWriter& writer, const std::vector<Master_Vertex>& vertices, const unsigned int& data_mask)
{

	for (size_t i = 0; i < vertices.size(); i++)
	{
		if ((data_mask & (1 << eMeshData_Position)) != 0)
		{
			Vector3 pos = vertices.at(i).position;
			writer.Write<float>(pos.x);
			writer.Write<float>(pos.y);
			writer.Write<float>(pos.z);
		}
		if ((data_mask & (1 << eMeshData_Color)) != 0)
		{
			Rgba color = vertices.at(i).color;
			writer.Write<float>(color.Red);
			writer.Write<float>(color.Green);
			writer.Write<float>(color.Blue);
			writer.Write<float>(color.Alpha);
		}
		if ((data_mask & (1 << eMeshData_TexCoord)) != 0)
		{
			writer.Write<unsigned int>(vertices.at(i).CountOfTexCoords);
			for (size_t j = 0; j < vertices.at(i).CountOfTexCoords; j++)
			{
				Vector2 texcoord = vertices.at(i).texCoord[j];
				writer.Write<float>(texcoord.x);
				writer.Write<float>(texcoord.y);
			}
		}
		if ((data_mask & (1 << eMeshData_Tangent)) != 0)
		{
			Vector3 tangent = vertices.at(i).tangent;
			writer.Write<float>(tangent.x);
			writer.Write<float>(tangent.y);
			writer.Write<float>(tangent.z);
		}
		if ((data_mask & (1 << eMeshData_Bitangent)) != 0)
		{
			Vector3 bitangent = vertices.at(i).bitangent;
			writer.Write<float>(bitangent.x);
			writer.Write<float>(bitangent.y);
			writer.Write<float>(bitangent.z);
		}
		if ((data_mask & (1 << eMeshData_Normal)) != 0)
		{
			Vector3 normal = vertices.at(i).normal;
			writer.Write<float>(normal.x);
			writer.Write<float>(normal.y);
			writer.Write<float>(normal.z);
		}
		if ((data_mask & (1 << eMeshData_Bone_Weights_and_Indices)) != 0)
		{
			skin_weight_t boneWeight = vertices.at(i).bone_weights_and_indicies;
			writer.Write<float>(boneWeight.bone_weights.x);
			writer.Write<float>(boneWeight.bone_weights.y);
			writer.Write<float>(boneWeight.bone_weights.z);
			writer.Write<float>(boneWeight.bone_weights.w);
			writer.Write<unsigned int>(boneWeight.bone_indices.x);
			writer.Write<unsigned int>(boneWeight.bone_indices.y);
			writer.Write<unsigned int>(boneWeight.bone_indices.z);
			writer.Write<unsigned int>(boneWeight.bone_indices.w);
		}
	}
}
void MeshBuilder::WriteIndices(IBinaryWriter& writer, const std::vector<unsigned int>& indices)
{
	for (size_t i = 0; i < indices.size(); i++)
	{
		writer.Write<unsigned int>(indices[i]);
	}
}
void MeshBuilder::ReadFromFile(const char* filename)
{
	FileBinaryReader reader;
	reader.open(filename);
	ReadFromStream(reader);
	reader.close();
}

void MeshBuilder::ReadFromStream(IBinaryReader& reader)
{
	unsigned int version = 0;
	char* matID = "";
	unsigned int sizeofUint = sizeof(unsigned int);
	reader.Read<unsigned int>(version, sizeofUint);
	reader.ReadString(matID);

	unsigned int vertex_count = 0;
	unsigned int index_count = 0;

	reader.Read<unsigned int>(vertex_count, sizeofUint);
	reader.Read<unsigned int>(index_count, sizeofUint);

	unsigned int data_mask = ReadDataMask(reader);
	ReadVertices(reader, data_mask, vertex_count, version);
	ReadIndices(reader, index_count);

	m_IOBinInfo.loadedFromBinFile = true;
}
const unsigned int MeshBuilder::ReadDataMask(IBinaryReader& reader)
{
	uint32_t mask = 0;

	size_t size = 0;
	unsigned int uIntSize = sizeof(unsigned int);
	reader.Read<unsigned int>(size, uIntSize);

	m_IOBinInfo = IOBinLoadBoolHolder();



	for (size_t i = 0; i < size; i++)
	{
		char* str = new char[64];
		for (size_t j = 0; j < 64; j++)
		{
			str[j] = '\0';
		}
		reader.ReadString(str);
		if (strcmp(str, meshBuilder_posStr.c_str()) == 0)
		{
			mask |= (1 << eMeshData_Position);
			m_IOBinInfo.loadedOrSavePosition = true;
		}
		else if (strcmp(str, meshBuilder_colStr.c_str()) == 0)
		{
			mask |= (1 << eMeshData_Color);
			m_IOBinInfo.loadedOrSaveColor = true;
		}
		else if (strcmp(str, meshBuilder_texStr.c_str()) == 0)
		{
			mask |= (1 << eMeshData_TexCoord);
			m_IOBinInfo.loadedOrSaveTexCoord = true;
		}
		else if (strcmp(str, meshBuilder_tanStr.c_str()) == 0)
		{
			mask |= (1 << eMeshData_Tangent);
			m_IOBinInfo.loadedOrSaveTangent = true;
		}
		else if (strcmp(str, meshBuilder_bitStr.c_str()) == 0)
		{
			mask |= (1 << eMeshData_Bitangent);
			m_IOBinInfo.loadedOrSaveBitangent = true;
		}
		else if (strcmp(str, meshBuilder_norStr.c_str()) == 0)
		{
			mask |= (1 << eMeshData_Normal);
			m_IOBinInfo.loadedOrSaveNormal = true;
		}
		else if (strcmp(str, meshBuilder_boneWAIStr.c_str()) == 0)
		{
			mask |= (1 << eMeshData_Bone_Weights_and_Indices);
			m_IOBinInfo.loadedOrSaveBoneWeightAndIndices = true;
		}
		delete str;
	}
	return mask;
}
void MeshBuilder::ReadVertices(IBinaryReader& reader, const unsigned int& data_mask, const unsigned int& numVerts, const unsigned int& version)
{
	m_vertices.clear();

	unsigned int sizeOfFloat = sizeof(float);
	unsigned int sizeOfUInt = sizeof(unsigned int);
	for (size_t i = 0; i < numVerts; i++)
	{
		Master_Vertex vert;
		if ((data_mask & (1 << eMeshData_Position)) != 0)
		{
			reader.Read<float>(vert.position.x, sizeOfFloat);
			reader.Read<float>(vert.position.y, sizeOfFloat);
			reader.Read<float>(vert.position.z, sizeOfFloat);
		}
		if ((data_mask & (1 << eMeshData_Color)) != 0)
		{
			reader.Read<float>(vert.color.Red, sizeOfFloat);
			reader.Read<float>(vert.color.Green, sizeOfFloat);
			reader.Read<float>(vert.color.Blue, sizeOfFloat);
			reader.Read<float>(vert.color.Alpha, sizeOfFloat);
		}
		if ((data_mask & (1 << eMeshData_TexCoord)) != 0)
		{
			if (version <= 1)
			{
				reader.Read<float>(vert.texCoord[0].x, sizeOfFloat);
				reader.Read<float>(vert.texCoord[0].y, sizeOfFloat);
			}
			else
			{
				reader.Read<unsigned int>(vert.CountOfTexCoords, sizeOfUInt);
				for (unsigned int j = 0; j < vert.CountOfTexCoords; j++)
				{
					reader.Read<float>(vert.texCoord[j].x, sizeOfFloat);
					reader.Read<float>(vert.texCoord[j].y, sizeOfFloat);
				}
			}
		}
		if ((data_mask & (1 << eMeshData_Tangent)) != 0)
		{
			reader.Read<float>(vert.tangent.x, sizeOfFloat);
			reader.Read<float>(vert.tangent.y, sizeOfFloat);
			reader.Read<float>(vert.tangent.z, sizeOfFloat);
		}
		if ((data_mask & (1 << eMeshData_Bitangent)) != 0)
		{
			reader.Read<float>(vert.bitangent.x, sizeOfFloat);
			reader.Read<float>(vert.bitangent.y, sizeOfFloat);
			reader.Read<float>(vert.bitangent.z, sizeOfFloat);
		}
		if ((data_mask & (1 << eMeshData_Normal)) != 0)
		{
			reader.Read<float>(vert.normal.x, sizeOfFloat);
			reader.Read<float>(vert.normal.y, sizeOfFloat);
			reader.Read<float>(vert.normal.z, sizeOfFloat);
		}
		if ((data_mask & (1 << eMeshData_Bone_Weights_and_Indices)) != 0)
		{
			reader.Read<float>(vert.bone_weights_and_indicies.bone_weights.x, sizeOfFloat);
			reader.Read<float>(vert.bone_weights_and_indicies.bone_weights.y, sizeOfFloat);
			reader.Read<float>(vert.bone_weights_and_indicies.bone_weights.z, sizeOfFloat);
			reader.Read<float>(vert.bone_weights_and_indicies.bone_weights.w, sizeOfFloat);
			reader.Read<unsigned int>(vert.bone_weights_and_indicies.bone_indices.x, sizeOfUInt);
			reader.Read<unsigned int>(vert.bone_weights_and_indicies.bone_indices.y, sizeOfUInt);
			reader.Read<unsigned int>(vert.bone_weights_and_indicies.bone_indices.z, sizeOfUInt);
			reader.Read<unsigned int>(vert.bone_weights_and_indicies.bone_indices.w, sizeOfUInt);
		}
		m_vertices.push_back(vert);
	}
}
void MeshBuilder::ReadIndices(IBinaryReader& reader, const unsigned int& numInd)
{
	m_indices.clear();
	unsigned int sizeOfUInt = sizeof(unsigned int);
	for (size_t i = 0; i < numInd; i++)
	{
		unsigned int index;
		reader.Read<unsigned int>(index, sizeOfUInt);
		m_indices.push_back(index);
	}
}

//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//Simple Build copyable Mesh
void MeshBuilder::BuildCube(const Vector3& size, const Rgba& solidColor)
{
	m_vertices.clear();
	m_indices.clear();
	m_stamp.color = solidColor;
	m_stamp.position = Vector3(-size.x / 2.f, -size.y / 2.f, -size.z / 2.f);
	AABB2 allCoords = AABB2(Vector2(0.f, 0.f), Vector2(1.f, 1.f));

	getBottomSideQuadTangMasterVertex(m_vertices, size, allCoords);
	getTopSideQuadTangMasterVertex(m_vertices, size, allCoords);
	getNorthSideQuadTangMasterVertex(m_vertices, size, allCoords);
	getSouthSideQuadTangMasterVertex(m_vertices, size, allCoords);
	getWestSideQuadTangMasterVertex(m_vertices, size, allCoords);
	getEastSideQuadTangMasterVertex(m_vertices, size, allCoords);

	m_indices = GetCubeTriangularIndices();
}
void MeshBuilder::BuildInvertedCube(const Vector3& size, const Rgba& solidColor)
{
	m_vertices.clear();
	m_indices.clear();
	m_stamp.color = solidColor;
	m_stamp.position = Vector3(-size.x / 2.f, -size.y / 2.f, -size.z / 2.f);
	AABB2 allCoords = AABB2(Vector2(0.f, 0.f), Vector2(1.f, 1.f));

	getInvertedBottomSideQuadTangMasterVertex(m_vertices, size, allCoords);
	getInvertedTopSideQuadTangMasterVertex(m_vertices, size, allCoords);
	getInvertedNorthSideQuadTangMasterVertex(m_vertices, size, allCoords);
	getInvertedSouthSideQuadTangMasterVertex(m_vertices, size, allCoords);
	getInvertedWestSideQuadTangMasterVertex(m_vertices, size, allCoords);
	getInvertedEastSideQuadTangMasterVertex(m_vertices, size, allCoords);

	m_indices = GetCubeTriangularIndices();
}
void MeshBuilder::BuildSkyBox(const Vector3& size, const Rgba& solidColor,
	const AABB2& WestTexCoords, const AABB2& EastTexCoords,
	const AABB2& NorthTexCoords, const AABB2& SouthTexCoords,
	const AABB2& TopTexCoords, const AABB2& BottomTexCoords)
{
	m_vertices.clear();
	m_indices.clear();
	m_stamp.color = solidColor;
	m_stamp.position = Vector3(-size.x / 2.f, -size.y / 2.f, -size.z / 2.f);
	getInvertedBottomSideQuadTangMasterVertex(m_vertices, size, BottomTexCoords);
	getInvertedTopSideQuadTangMasterVertex(m_vertices, size, TopTexCoords);
	getInvertedNorthSideQuadTangMasterVertex(m_vertices, size, NorthTexCoords);
	getInvertedSouthSideQuadTangMasterVertex(m_vertices, size, SouthTexCoords);
	getInvertedWestSideQuadTangMasterVertex(m_vertices, size, WestTexCoords);
	getInvertedEastSideQuadTangMasterVertex(m_vertices, size, EastTexCoords);

	m_indices = GetCubeTriangularIndices();
}

void MeshBuilder::BuildSphere(const float& radius, const Rgba& solidColor)
{
	m_vertices.clear();
	m_indices.clear();
	m_stamp.color = solidColor;
	int numRings = 100;
	int numSectors = 100;
	int currentRow = 0;
	int nextRow = 0;
	int nextSector;

	const float R = 1.0f / (float)(numRings);
	const float S = 1.0f / (float)(numSectors);

	for (int r = 0; r <= numRings; ++r)
	{
		const float internalR = (float)PI * r * R;
		const float z = sin(-(float)PI / 2 + internalR);
		const float sinR = sin(internalR);
		for (int s = 0; s <= numSectors + 1; ++s)
		{
			const float internalS = 2 * (float)PI * s * S;
			const float tanX = cos(internalS);
			const float tanY = sin(internalS);
			const float tanZ = 0.f;
			const float y = tanX * sinR;
			const float x = sin(internalS) * sinR; //const float var = tan(x);
			const Vector3 spherePos = Vector3(x * radius, y * radius, -z * radius);
			Vector3 normal = spherePos;
			normal.normalize();
			const Vector3 tangent = Vector3(-tanX, tanY, tanZ);
			const Vector3 biTangent = normal.crossProduct(tangent);
			//const float bitangent = z / x;
			//const float tangent = tan(2 * (float)PI * s * S) * tan((float)PI * r * R);

			Master_Vertex current = m_stamp;
			current.position = spherePos;
			current.texCoord[0] = Vector2(r * R, s * S);
			current.tangent = tangent;
			current.bitangent = biTangent;
			current.normal = normal;
			current.bone_weights_and_indicies = skin_weight_t(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0, 0, 0, 0));

			m_vertices.push_back(current);
			currentRow = r * numSectors;
			nextRow = (r + 1) * numSectors;
			nextSector = (s + 1) % numSectors;

			if (r < numRings - 1)
			{
				m_indices.push_back((uint16_t)(nextRow + nextSector));
				m_indices.push_back((uint16_t)(nextRow + s));
				m_indices.push_back((uint16_t)(currentRow + s));

				m_indices.push_back((uint16_t)(currentRow + nextSector));
				m_indices.push_back((uint16_t)(nextRow + nextSector));
				m_indices.push_back((uint16_t)(currentRow + s));
			}
		}
	}
}
void MeshBuilder::BuildCircle(const float& radius, const Rgba& solidColor, const Vector3& up, const Vector3& right)
{
	m_vertices.clear();
	m_indices.clear();
	m_stamp.color = solidColor;
	int numSectors = 100;
	int nextSector;
	const float S = 1.0f / (float)(numSectors);
	for (int s = 0; s <= numSectors + 1; ++s)
	{
		const float internalS = 2 * (float)PI * s * S;
		const float tanX = cos(internalS);
		const float tanY = sin(internalS);
		const float tanZ = 0.f;
		const float y = tanX;
		const float x = sin(internalS); //const float var = tan(x);
		const Vector3 spherePos = (right * x * radius) + (up * y * radius);
		Vector3 normal = spherePos;
		normal.normalize();
		const Vector3 tangent = Vector3(-tanX, tanY, tanZ);
		const Vector3 biTangent = normal.crossProduct(tangent);
		//const float bitangent = z / x;
		//const float tangent = tan(2 * (float)PI * s * S) * tan((float)PI * r * R);

		Master_Vertex current = m_stamp;
		current.position = spherePos;
		current.texCoord[0] = Vector2(tanX, tanY);
		current.tangent = tangent;
		current.bitangent = biTangent;
		current.normal = normal;
		current.bone_weights_and_indicies = skin_weight_t(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0, 0, 0, 0));

		m_vertices.push_back(current);
		nextSector = (s + 1) % numSectors;

		if (s < numSectors - 1)
		{
			//Presently Indices do not hook up correctly.
			m_indices.push_back((uint16_t)(nextSector));
			m_indices.push_back((uint16_t)(s));
			m_indices.push_back((uint16_t)(s));

			m_indices.push_back((uint16_t)(nextSector));
			m_indices.push_back((uint16_t)(nextSector));
			m_indices.push_back((uint16_t)(s));
		}
	}
}

void MeshBuilder::BuildQuad(const Vector3& Up, const Vector3& Right, const float& length, const float& height, const int& numXDivisions, 
	const int& numYDivisions, const Rgba& solidColor, const Vector3& translation)
{
	m_vertices.clear();
	m_indices.clear();

	float halfHeight = height / 2.f;
	float halfLength = length / 2.f;
	plane_data_t plane;
	plane.initial_position = (Up * (-halfHeight)) + (Right * (-halfLength)) + translation;
	plane.right = Right;
	plane.up = Up;
	this->SetColor(solidColor);

	MeshBuildPatch(this, 0.f, length, (uint32_t)numXDivisions, 0.f, height, (uint32_t)numYDivisions, PlaneFunc, &plane);
}

//BuildPatch
void  MeshBuilder::MeshBuildPatch(MeshBuilder *mb,
	float start_x, float end_x, uint32_t x_sections,
	float start_y, float end_y, uint32_t y_sections,
	patch_func_cb patch_func,
	void *user_data)
{
	if (x_sections <= 0 || y_sections <= 0)
	{
		return;
	}

	mb->Start(/*true /*uses index buffer*/);

	// So, let's add out our vertices.
	uint32_t x_vertex_count = x_sections + 1;
	uint32_t y_vertex_count = y_sections + 1;

	float const x_range = end_x - start_x;
	float const y_range = end_y - start_y;
	float const x_step = x_range / (float)x_sections;
	float const y_step = y_range / (float)y_sections;

	// Texture goes over the whole patch
	// BONUS - you can provite uv ranges as well.
	float const u_step = 1.0f / (float)x_sections;
	float const v_step = 1.0f / (float)y_sections;

	uint32_t start_vertex_index = mb->GetCurrentIndex();

	// Okay, now, let's add all our vertices
	float x, y;
	float u, v;

	y = start_y;
	v = 0.0f;
	float const delta = .01f; // artitrarily small value, can go smaller

	for (uint32_t iy = 0; iy < y_vertex_count; ++iy) {
		x = start_x;
		u = 0.0f;

		for (uint32_t ix = 0; ix < x_vertex_count; ++ix) {
			Vector3 tangent = patch_func(user_data, x + delta, y)
				- patch_func(user_data, x - delta, y);
			Vector3 bitangent = patch_func(user_data, x, y + delta)
				- patch_func(user_data, x, y - delta);

			tangent.normalize();
			bitangent.normalize();
			Vector3 normal = tangent.crossProduct(bitangent);
			bitangent = normal.crossProduct(tangent);
			mb->SetTangent(tangent);
			mb->SetBitangent(bitangent);
			mb->SetNormal(normal);
			mb->SetTexCoord0(Vector2(u, v));
			mb->SetBoneWeightsAndIndices(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0, 0, 0, 0));

			Vector3 position = patch_func(user_data, x, y);
			mb->AddVertex(position);

			x += x_step;
			u += u_step;
		}

		y += y_step;
		v += v_step;
	}

	// Next, add all the indices for this patch
	for (uint32_t iy = 0; iy < y_sections; ++iy) {
		for (uint32_t ix = 0; ix < x_sections; ++ix) {

			uint32_t bl_idx = start_vertex_index
				+ (iy * x_vertex_count) + ix;
			uint32_t br_idx = bl_idx + 1;
			uint32_t tl_idx = bl_idx + x_vertex_count;
			uint32_t tr_idx = tl_idx + 1;

			// How many indices is this actually adding
			// under the hood, it we're working with triangles?
			mb->addQuadIndices(bl_idx, br_idx, tl_idx, tr_idx);
		}
	}

	mb->End();
}



//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//Simple Mesh Builds

//!-----
//Plain
Mesh* MeshBuilder::FastBuildSinglePlain(const Vector3& Up, const Vector3& Right, const float& length, const float& width, const Rgba& solidColor)
{
	Vector3 trueUp = Up;
	Vector3 trueRight = Right;
	trueUp.normalize();
	trueRight.normalize();
	Vector3 normal = trueRight.crossProduct(trueUp);
	Vector3 bottomLeft = (trueUp * (width * -0.5f)) + (trueRight * (length * -0.5f));
	Vector3 topLeft = bottomLeft + (trueUp * width);
	Vector3 bottomRight = bottomLeft + (trueRight * length);
	Vector3 topRight = bottomLeft + (trueRight * length) + (trueUp * width);

	std::vector<Vertex_PCTT> vertices;
	vertices.push_back(Vertex_PCTT(bottomLeft, solidColor, Vector2(0.f, 1.f), trueRight, trueUp, normal)); //BL
	vertices.push_back(Vertex_PCTT(bottomRight, solidColor, Vector2(1.f, 1.f), trueRight, trueUp, normal)); //BR
	vertices.push_back(Vertex_PCTT(topRight, solidColor, Vector2(1.f, 0.f), trueRight, trueUp, normal)); //TR
	vertices.push_back(Vertex_PCTT(topLeft, solidColor, Vector2(0.f, 0.f), trueRight, trueUp, normal)); //TL

	std::vector<unsigned int> ibos;
	ibos.push_back(0);
	ibos.push_back(1);
	ibos.push_back(2);
	ibos.push_back(2);
	ibos.push_back(3);
	ibos.push_back(0);

	Mesh* plain = new Mesh();
	plain->setVBO(vertices);
	plain->setIBO(ibos);
	plain->CreateVAOID();
	return plain;
}

//!-----
//Sphere
Mesh* MeshBuilder::FastBuildSingleSphere(const float& xyRadius, const float& yzRadius, const Rgba& solidColor)
{
	// Add all vertexes to the mesh for VAO, add all indices to the mesh for IBO
	std::vector<Vertex_PCTT> sphereVerts;
	std::vector<unsigned int> sphereIndices;
	int numRings = 100;
	int numSectors = 100;
	int currentRow = 0;
	int nextRow = 0;
	int nextSector;

	const float R = 1.0f / (float)(numRings);
	const float S = 1.0f / (float)(numSectors);

	for (int r = 0; r <= numRings; ++r)
	{
		const float internalR = (float)PI * r * R;
		const float z = sin(-(float)PI / 2 + internalR);
		const float sinR = sin(internalR);
		for (int s = 0; s <= numSectors + 1; ++s)
		{
			const float internalS = 2 * (float)PI * s * S;
			const float tanX = cos(internalS);
			const float tanY = sin(internalS);
			const float tanZ = 0.f;
			const float y = (tanX * sinR);
			const float x = (tanY * sinR); //const float var = tan(x);
			const Vector3 spherePos = Vector3(x * xyRadius, y * xyRadius * yzRadius, -z * yzRadius);
			Vector3 normal = spherePos;
			normal.normalize();
			const Vector3 tangent = Vector3(-tanX, tanY, tanZ);
			const Vector3 biTangent = normal.crossProduct(tangent);
			//const float bitangent = z / x;
			//const float tangent = tan(2 * (float)PI * s * S) * tan((float)PI * r * R);

			sphereVerts.push_back(Vertex_PCTT(spherePos, solidColor, Vector2(r * R, s * S), tangent, biTangent, normal));
			currentRow = r * numSectors;
			nextRow = (r + 1) * numSectors;
			nextSector = (s + 1) % numSectors;

			if (r < numRings - 1)
			{
				sphereIndices.push_back((uint16_t)(nextRow + nextSector));
				sphereIndices.push_back((uint16_t)(nextRow + s));
				sphereIndices.push_back((uint16_t)(currentRow + s));

				sphereIndices.push_back((uint16_t)(currentRow + nextSector));
				sphereIndices.push_back((uint16_t)(nextRow + nextSector));
				sphereIndices.push_back((uint16_t)(currentRow + s));
			}
		}
	}

	Mesh* sphere = new Mesh();
	sphere->setVBO(sphereVerts);
	sphere->setIBO(sphereIndices);
	sphere->CreateVAOID();
	return sphere;
}
Mesh* MeshBuilder::FastBuildSingleSphere(const float& radius, const Rgba& solidColor)
{
	// Add all vertexes to the mesh for VAO, add all indices to the mesh for IBO
	std::vector<Vertex_PCTT> sphereVerts;
	std::vector<unsigned int> sphereIndices;
	int numRings = 100;
	int numSectors = 100;
	int currentRow = 0;
	int nextRow = 0;
	int nextSector;

	const float R = 1.0f / (float)(numRings);
	const float S = 1.0f / (float)(numSectors);

	for (int r = 0; r <= numRings; ++r)
	{
		const float internalR = (float)PI * r * R;
		const float z = sin(-(float)PI / 2 + internalR);
		const float sinR = sin(internalR);
		for (int s = 0; s <= numSectors + 1; ++s)
		{
			const float internalS = 2 * (float)PI * s * S;
			const float tanX = cos(internalS);
			const float tanY = sin(internalS);
			const float tanZ = 0.f;
			const float y = tanX * sinR;
			const float x = sin(internalS) * sinR; //const float var = tan(x);
			const Vector3 spherePos = Vector3(x * radius, y * radius, -z * radius);
			Vector3 normal = spherePos;
			normal.normalize();
			const Vector3 tangent = Vector3(-tanX, tanY, tanZ);
			const Vector3 biTangent = normal.crossProduct(tangent);
			//const float bitangent = z / x;
			//const float tangent = tan(2 * (float)PI * s * S) * tan((float)PI * r * R);

			sphereVerts.push_back(Vertex_PCTT(spherePos, solidColor, Vector2(r * R, s * S), tangent, biTangent, normal));
			currentRow = r * numSectors;
			nextRow = (r + 1) * numSectors;
			nextSector = (s + 1) % numSectors;

			if (r < numRings - 1)
			{
				sphereIndices.push_back((uint16_t)(nextRow + nextSector));
				sphereIndices.push_back((uint16_t)(nextRow + s));
				sphereIndices.push_back((uint16_t)(currentRow + s));

				sphereIndices.push_back((uint16_t)(currentRow + nextSector));
				sphereIndices.push_back((uint16_t)(nextRow + nextSector));
				sphereIndices.push_back((uint16_t)(currentRow + s));
			}
		}
	}

	Mesh* sphere = new Mesh();
	sphere->setVBO(sphereVerts);
	sphere->setIBO(sphereIndices);
	sphere->CreateVAOID();
	return sphere;
}

//!-----
//Fast Single Cube Specific
Mesh* MeshBuilder::FastBuildSingleCube(const Vector3& lengthWidthHeight, const Rgba& solidColor)
{
	std::vector<Vertex_PCTT> sides;
	const AABB2 texCoordsForAllSides = AABB2(Vector2(), Vector2(1.f, 1.f));
	Vector3 bottomLeftCorner = Vector3(0.f, 0.f, 0.f) - (lengthWidthHeight / 2.f);
	getBottomSideQuadTang(sides, bottomLeftCorner, lengthWidthHeight, solidColor, texCoordsForAllSides);
	getTopSideQuadTang(sides, bottomLeftCorner, lengthWidthHeight, solidColor, texCoordsForAllSides);
	getNorthSideQuadTang(sides, bottomLeftCorner, lengthWidthHeight, solidColor, texCoordsForAllSides);
	getSouthSideQuadTang(sides, bottomLeftCorner, lengthWidthHeight, solidColor, texCoordsForAllSides);
	getWestSideQuadTang(sides, bottomLeftCorner, lengthWidthHeight, solidColor, texCoordsForAllSides);
	getEastSideQuadTang(sides, bottomLeftCorner, lengthWidthHeight, solidColor, texCoordsForAllSides);
	const std::vector<unsigned int> indicies = GetCubeTriangularIndices();
	Mesh* cube = new Mesh();
	cube->setVBO(sides);
	cube->setIBO(indicies);
	cube->CreateVAOID();
	return cube;
}
Mesh* MeshBuilder::FastBuildSingleInvertedCube(const Vector3& lengthWidthHeight, const Rgba& solidColor)
{
	std::vector<Vertex_PCTT> sides;
	const AABB2 texCoordsForAllSides = AABB2(Vector2(), Vector2(1.f, 1.f));
	Vector3 bottomLeftCorner = Vector3(0.f, 0.f, 0.f) - (lengthWidthHeight / 2.f);
	getInvertedBottomSideQuadTang(sides, bottomLeftCorner, lengthWidthHeight, solidColor, texCoordsForAllSides);
	getInvertedTopSideQuadTang(sides, bottomLeftCorner, lengthWidthHeight, solidColor, texCoordsForAllSides);
	getInvertedNorthSideQuadTang(sides, bottomLeftCorner, lengthWidthHeight, solidColor, texCoordsForAllSides);
	getInvertedSouthSideQuadTang(sides, bottomLeftCorner, lengthWidthHeight, solidColor, texCoordsForAllSides);
	getInvertedWestSideQuadTang(sides, bottomLeftCorner, lengthWidthHeight, solidColor, texCoordsForAllSides);
	getInvertedEastSideQuadTang(sides, bottomLeftCorner, lengthWidthHeight, solidColor, texCoordsForAllSides);
	const std::vector<unsigned int> indicies = GetCubeTriangularIndices();
	Mesh* cube = new Mesh();
	cube->setVBO(sides);
	cube->setIBO(indicies);
	cube->CreateVAOID();
	return cube;
}
Mesh* MeshBuilder::FastBuildSingleSkyBox(const Vector3& lengthWidthHeight, const Rgba& solidColor,
	const AABB2& WestTexCoords, const AABB2& EastTexCoords,
	const AABB2& NorthTexCoords, const AABB2& SouthTexCoords,
	const AABB2& TopTexCoords, const AABB2& BottomTexCoords)
{
	std::vector<Vertex_PCTT> sides;
	Vector3 bottomLeftCorner = Vector3(0.f, 0.f, 0.f) - (lengthWidthHeight / 2.f);
	getInvertedBottomSideQuadTang(sides, bottomLeftCorner, lengthWidthHeight, solidColor, BottomTexCoords);
	getInvertedTopSideQuadTang(sides, bottomLeftCorner, lengthWidthHeight, solidColor, TopTexCoords);
	getInvertedNorthSideQuadTang(sides, bottomLeftCorner, lengthWidthHeight, solidColor, NorthTexCoords);
	getInvertedSouthSideQuadTang(sides, bottomLeftCorner, lengthWidthHeight, solidColor, SouthTexCoords);
	getInvertedWestSideQuadTang(sides, bottomLeftCorner, lengthWidthHeight, solidColor, WestTexCoords);
	getInvertedEastSideQuadTang(sides, bottomLeftCorner, lengthWidthHeight, solidColor, EastTexCoords);
	const std::vector<unsigned int> indicies = GetCubeTriangularIndices();
	Mesh* skyBox = new Mesh();
	skyBox->setVBO(sides);
	skyBox->setIBO(indicies);
	skyBox->CreateVAOID();
	return skyBox;
}

//--------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------
//Cube Specific
//!----
//Simple Mesh Build

void MeshBuilder::addQuadIndices(unsigned int var1, unsigned int var2, unsigned int var3, unsigned int var4)
{
	//	addSixIndices(m_indices, var0, var1, var2, var2, var1, var3);
	addSixIndices(m_indices, var1, var2, var3, var3, var2, var4);
}
void MeshBuilder::addSixIndices(std::vector<unsigned int>& triangularIndices, unsigned int var1, unsigned int var2, unsigned int var3, unsigned int var4, unsigned int var5, unsigned int var6)
{
	triangularIndices.push_back(var1); //0
	triangularIndices.push_back(var2); //1
	triangularIndices.push_back(var3); //2
	triangularIndices.push_back(var4); //1
	triangularIndices.push_back(var5); //3
	triangularIndices.push_back(var6); //2
}

void MeshBuilder::setSixIndices(std::vector<unsigned int>& triangularIndices, const unsigned int& startIndex, unsigned int var1, unsigned int var2, unsigned int var3, unsigned int var4, unsigned int var5, unsigned int var6)
{
	triangularIndices[startIndex] = var1; //0
	triangularIndices[startIndex + 1] = var2; //1
	triangularIndices[startIndex + 2] = var3; //2
	triangularIndices[startIndex + 3] = var4; //1
	triangularIndices[startIndex + 4] = var5; //3
	triangularIndices[startIndex + 5] = var6; //2
}

const std::vector<unsigned int> MeshBuilder::GetCubeTriangularIndices()
{
	//0-3 bottom, 4-7 top, 8-11 north, 12-15 south, 16-19 west, 20-23 east; mult 24 by 2 = 48
	std::vector<unsigned int> triangularIndices;
	triangularIndices.resize(36);

	setSixIndices(triangularIndices, 0, 0, 1, 2, 0, 2, 3); //starts at 0, bottom
	setSixIndices(triangularIndices, 6, 4, 5, 6, 4, 6, 7); //starts at 4, top
	setSixIndices(triangularIndices, 12, 8, 9, 10, 8, 10, 11); //starts at 8, north
	setSixIndices(triangularIndices, 18, 12, 13, 14, 12, 14, 15); //starts at 12, south
	setSixIndices(triangularIndices, 24, 16, 17, 18, 16, 18, 19); //starts at 16, west
	setSixIndices(triangularIndices, 30, 20, 21, 22, 20, 22, 23); //starts at 20, east

	return triangularIndices;
}

void MeshBuilder::getBottomSideQuadTang(std::vector<Vertex_PCTT>& quads, const Vector3& bottomLeftCorner, const Vector3& size, const Rgba& tint, const AABB2& texCoords)
{
	float maxX = bottomLeftCorner.x + size.x;
	float maxY = bottomLeftCorner.y + size.y;
	Vector3 normal1 = Vector3(bottomLeftCorner.x, bottomLeftCorner.y, bottomLeftCorner.z);
	Vector3 normal2 = Vector3(bottomLeftCorner.x, maxY, bottomLeftCorner.z);
	Vector3 normal3 = Vector3(maxX, maxY, bottomLeftCorner.z);
	Vector3 normal4 = Vector3(maxX, bottomLeftCorner.y, bottomLeftCorner.z);
	//texCoords = mins and maxs
	normal1.normalize();
	normal2.normalize();
	normal3.normalize();
	normal4.normalize();
	Vector3 leftToRight = Vector3(maxX, 0.f, 0.f);
	Vector3 topToBottom = Vector3(0.f, maxY, 0.f);
	leftToRight.normalize();
	topToBottom.normalize();
	normal1 = leftToRight.crossProduct(topToBottom);
	normal2 = leftToRight.crossProduct(topToBottom);
	normal3 = leftToRight.crossProduct(topToBottom);
	normal4 = leftToRight.crossProduct(topToBottom);
	Vector3 tangent1 = leftToRight;
	Vector3 tangent2 = leftToRight;
	Vector3 tangent3 = leftToRight;
	Vector3 tangent4 = leftToRight;
	Vector3 bitangent1 = topToBottom; //normal1.crossProduct(tangent1);
	Vector3 bitangent2 = topToBottom; //normal2.crossProduct(tangent2);
	Vector3 bitangent3 = topToBottom; //normal3.crossProduct(tangent3);
	Vector3 bitangent4 = topToBottom; //normal4.crossProduct(tangent4);

	quads.push_back(Vertex_PCTT(Vector3(bottomLeftCorner.x, bottomLeftCorner.y, bottomLeftCorner.z), tint, texCoords.mins, tangent1, bitangent1, normal1));
	quads.push_back(Vertex_PCTT(Vector3(bottomLeftCorner.x, maxY, bottomLeftCorner.z), tint, Vector2(texCoords.mins.x, texCoords.maxs.y), tangent2, bitangent2, normal2));
	quads.push_back(Vertex_PCTT(Vector3(maxX, maxY, bottomLeftCorner.z), tint, texCoords.maxs, tangent3, bitangent3, normal3));
	quads.push_back(Vertex_PCTT(Vector3(maxX, bottomLeftCorner.y, bottomLeftCorner.z), tint, Vector2(texCoords.maxs.x, texCoords.mins.y), tangent4, bitangent4, normal4));
}
void MeshBuilder::getTopSideQuadTang(std::vector<Vertex_PCTT>& quads, const Vector3& bottomLeftCorner, const Vector3& size, const Rgba& tint, const AABB2& texCoords)
{
	float zVal = bottomLeftCorner.z + size.z;
	float maxX = bottomLeftCorner.x + size.x;
	float maxY = bottomLeftCorner.y + size.y;
	Vector3 normal1 = Vector3(bottomLeftCorner.x, bottomLeftCorner.y, zVal);
	Vector3 normal2 = Vector3(maxX, bottomLeftCorner.y, zVal);
	Vector3 normal3 = Vector3(maxX, maxY, zVal);
	Vector3 normal4 = Vector3(bottomLeftCorner.x, maxY, zVal);
	normal1.normalize();
	normal2.normalize();
	normal3.normalize();
	normal4.normalize();
	Vector3 leftToRight = Vector3(-maxX, 0.f, 0.f);
	Vector3 topToBottom = Vector3(0.f, -maxY, 0.f);
	leftToRight.normalize();
	topToBottom.normalize();
	normal1 = leftToRight.crossProduct(topToBottom);
	normal2 = leftToRight.crossProduct(topToBottom);
	normal3 = leftToRight.crossProduct(topToBottom);
	normal4 = leftToRight.crossProduct(topToBottom);
	Vector3 tangent1 = leftToRight;
	Vector3 tangent2 = leftToRight;
	Vector3 tangent3 = leftToRight;
	Vector3 tangent4 = leftToRight;
	Vector3 bitangent1 = topToBottom;//normal1.crossProduct(tangent1);
	Vector3 bitangent2 = topToBottom;//normal2.crossProduct(tangent2);
	Vector3 bitangent3 = topToBottom;//normal3.crossProduct(tangent3);
	Vector3 bitangent4 = topToBottom;//normal4.crossProduct(tangent4);
	quads.push_back(Vertex_PCTT(Vector3(bottomLeftCorner.x, bottomLeftCorner.y, zVal), tint, texCoords.mins, tangent1, bitangent1, normal1));
	quads.push_back(Vertex_PCTT(Vector3(maxX, bottomLeftCorner.y, zVal), tint, Vector2(texCoords.mins.x, texCoords.maxs.y), tangent2, bitangent2, normal2));
	quads.push_back(Vertex_PCTT(Vector3(maxX, maxY, zVal), tint, texCoords.maxs, tangent3, bitangent3, normal3));
	quads.push_back(Vertex_PCTT(Vector3(bottomLeftCorner.x, maxY, zVal), tint, Vector2(texCoords.maxs.x, texCoords.mins.y), tangent4, bitangent4, normal4));
}
void MeshBuilder::getNorthSideQuadTang(std::vector<Vertex_PCTT>& quads, const Vector3& bottomLeftCorner, const Vector3& size, const Rgba& tint, const AABB2& texCoords)
{
	float yVal = bottomLeftCorner.y + size.y;
	float maxZ = bottomLeftCorner.z + size.z;
	float maxX = bottomLeftCorner.x + size.x;
	Vector3 normal1 = Vector3(bottomLeftCorner.x, yVal, bottomLeftCorner.z);
	Vector3 normal2 = Vector3(bottomLeftCorner.x, yVal, maxZ);
	Vector3 normal3 = Vector3(maxX, yVal, maxZ);
	Vector3 normal4 = Vector3(maxX, yVal, bottomLeftCorner.z);
	normal1.normalize();
	normal2.normalize();
	normal3.normalize();
	normal4.normalize();
	Vector3 leftToRight = Vector3(-maxX, 0.f, 0.f);
	Vector3 topToBottom = Vector3(0.f, 0.f, maxZ);
	leftToRight.normalize();
	topToBottom.normalize();

	normal1 = leftToRight.crossProduct(topToBottom);
	normal2 = leftToRight.crossProduct(topToBottom);
	normal3 = leftToRight.crossProduct(topToBottom);
	normal4 = leftToRight.crossProduct(topToBottom);
	Vector3 tangent1 = leftToRight;
	Vector3 tangent2 = leftToRight;
	Vector3 tangent3 = leftToRight;
	Vector3 tangent4 = leftToRight;
	Vector3 bitangent1 = topToBottom;//normal1.crossProduct(tangent1);
	Vector3 bitangent2 = topToBottom;//normal2.crossProduct(tangent2);
	Vector3 bitangent3 = topToBottom;//normal3.crossProduct(tangent3);
	Vector3 bitangent4 = topToBottom;//normal4.crossProduct(tangent4);
	quads.push_back(Vertex_PCTT(Vector3(bottomLeftCorner.x, yVal, bottomLeftCorner.z), tint, texCoords.maxs, tangent1, bitangent1, normal1));
	quads.push_back(Vertex_PCTT(Vector3(bottomLeftCorner.x, yVal, maxZ), tint, Vector2(texCoords.maxs.x, texCoords.mins.y), tangent2, bitangent2, normal2));
	quads.push_back(Vertex_PCTT(Vector3(maxX, yVal, maxZ), tint, texCoords.mins, tangent3, bitangent3, normal3));
	quads.push_back(Vertex_PCTT(Vector3(maxX, yVal, bottomLeftCorner.z), tint, Vector2(texCoords.mins.x, texCoords.maxs.y), tangent4, bitangent4, normal4));
}
void MeshBuilder::getSouthSideQuadTang(std::vector<Vertex_PCTT>& quads, const Vector3& bottomLeftCorner, const Vector3& size, const Rgba& tint, const AABB2& texCoords)
{
	float maxX = bottomLeftCorner.x + size.x;
	float maxZ = bottomLeftCorner.z + size.z;
	Vector3 normal1 = Vector3(maxX, bottomLeftCorner.y, bottomLeftCorner.z);
	Vector3 normal2 = Vector3(maxX, bottomLeftCorner.y, maxZ);
	Vector3 normal3 = Vector3(bottomLeftCorner.x, bottomLeftCorner.y, maxZ);
	Vector3 normal4 = Vector3(bottomLeftCorner.x, bottomLeftCorner.y, bottomLeftCorner.z);
	normal1.normalize();
	normal2.normalize();
	normal3.normalize();
	normal4.normalize();
	Vector3 leftToRight = Vector3(maxX, 0.f, 0.f);
	Vector3 topToBottom = Vector3(0.f, 0.f, maxZ);
	leftToRight.normalize();
	topToBottom.normalize();
	normal1 = leftToRight.crossProduct(topToBottom);
	normal2 = leftToRight.crossProduct(topToBottom);
	normal3 = leftToRight.crossProduct(topToBottom);
	normal4 = leftToRight.crossProduct(topToBottom);
	Vector3 tangent1 = leftToRight;
	Vector3 tangent2 = leftToRight;
	Vector3 tangent3 = leftToRight;
	Vector3 tangent4 = leftToRight;
	Vector3 bitangent1 = topToBottom;//normal1.crossProduct(tangent1);
	Vector3 bitangent2 = topToBottom;//normal2.crossProduct(tangent2);
	Vector3 bitangent3 = topToBottom;//normal3.crossProduct(tangent3);
	Vector3 bitangent4 = topToBottom;//normal4.crossProduct(tangent4);
	quads.push_back(Vertex_PCTT(Vector3(maxX, bottomLeftCorner.y, bottomLeftCorner.z), tint, texCoords.maxs, tangent1, bitangent1, normal1));
	quads.push_back(Vertex_PCTT(Vector3(maxX, bottomLeftCorner.y, maxZ), tint, Vector2(texCoords.maxs.x, texCoords.mins.y), tangent2, bitangent2, normal2));
	quads.push_back(Vertex_PCTT(Vector3(bottomLeftCorner.x, bottomLeftCorner.y, maxZ), tint, texCoords.mins, tangent3, bitangent3, normal3));
	quads.push_back(Vertex_PCTT(Vector3(bottomLeftCorner.x, bottomLeftCorner.y, bottomLeftCorner.z), tint, Vector2(texCoords.mins.x, texCoords.maxs.y), tangent4, bitangent4, normal4));
}
void MeshBuilder::getWestSideQuadTang(std::vector<Vertex_PCTT>& quads, const Vector3& bottomLeftCorner, const Vector3& size, const Rgba& tint, const AABB2& texCoords)
{
	float maxY = bottomLeftCorner.y + size.y;
	float maxZ = bottomLeftCorner.z + size.z;
	Vector3 normal1 = Vector3(bottomLeftCorner.x, bottomLeftCorner.y, bottomLeftCorner.z);
	Vector3 normal2 = Vector3(bottomLeftCorner.x, bottomLeftCorner.y, maxZ);
	Vector3 normal3 = Vector3(bottomLeftCorner.x, maxY, maxZ);
	Vector3 normal4 = Vector3(bottomLeftCorner.x, maxY, bottomLeftCorner.z);
	normal1.normalize();
	normal2.normalize();
	normal3.normalize();
	normal4.normalize();
	Vector3 leftToRight = Vector3(0.f, -maxY, 0.f);
	Vector3 topToBottom = Vector3(0.f, 0.f, maxZ);
	leftToRight.normalize();
	topToBottom.normalize();
	normal1 = leftToRight.crossProduct(topToBottom);
	normal2 = leftToRight.crossProduct(topToBottom);
	normal3 = leftToRight.crossProduct(topToBottom);
	normal4 = leftToRight.crossProduct(topToBottom);
	Vector3 tangent1 = leftToRight;
	Vector3 tangent2 = leftToRight;
	Vector3 tangent3 = leftToRight;
	Vector3 tangent4 = leftToRight;
	Vector3 bitangent1 = topToBottom;//normal1.crossProduct(tangent1);
	Vector3 bitangent2 = topToBottom;//normal2.crossProduct(tangent2);
	Vector3 bitangent3 = topToBottom;//normal3.crossProduct(tangent3);
	Vector3 bitangent4 = topToBottom;//normal4.crossProduct(tangent4);
	quads.push_back(Vertex_PCTT(Vector3(bottomLeftCorner.x, bottomLeftCorner.y, bottomLeftCorner.z), tint, texCoords.maxs, tangent1, bitangent1, normal1));
	quads.push_back(Vertex_PCTT(Vector3(bottomLeftCorner.x, bottomLeftCorner.y, maxZ), tint, Vector2(texCoords.maxs.x, texCoords.mins.y), tangent2, bitangent2, normal2));
	quads.push_back(Vertex_PCTT(Vector3(bottomLeftCorner.x, maxY, maxZ), tint, texCoords.mins, tangent3, bitangent3, normal3));
	quads.push_back(Vertex_PCTT(Vector3(bottomLeftCorner.x, maxY, bottomLeftCorner.z), tint, Vector2(texCoords.mins.x, texCoords.maxs.y), tangent4, bitangent4, normal4));
}
void MeshBuilder::getEastSideQuadTang(std::vector<Vertex_PCTT>& quads, const Vector3& bottomLeftCorner, const Vector3& size, const Rgba& tint, const AABB2& texCoords)
{
	float xVal = bottomLeftCorner.x + size.x;
	float maxY = bottomLeftCorner.y + size.y;
	float maxZ = bottomLeftCorner.z + size.z;
	Vector3 normal1 = Vector3(xVal, maxY, bottomLeftCorner.z);
	Vector3 normal2 = Vector3(xVal, maxY, maxZ);
	Vector3 normal3 = Vector3(xVal, bottomLeftCorner.y, maxZ);
	Vector3 normal4 = Vector3(xVal, bottomLeftCorner.y, bottomLeftCorner.z);
	normal1.normalize();
	normal2.normalize();
	normal3.normalize();
	normal4.normalize();
	Vector3 leftToRight = Vector3(0.f, maxY, 0.f);
	Vector3 topToBottom = Vector3(0.f, 0.f, maxZ);
	leftToRight.normalize();
	topToBottom.normalize();
	normal1 = leftToRight.crossProduct(topToBottom);
	normal2 = leftToRight.crossProduct(topToBottom);
	normal3 = leftToRight.crossProduct(topToBottom);
	normal4 = leftToRight.crossProduct(topToBottom);
	Vector3 tangent1 = leftToRight;
	Vector3 tangent2 = leftToRight;
	Vector3 tangent3 = leftToRight;
	Vector3 tangent4 = leftToRight;
	Vector3 bitangent1 = topToBottom;//normal1.crossProduct(tangent1);
	Vector3 bitangent2 = topToBottom;//normal2.crossProduct(tangent2);
	Vector3 bitangent3 = topToBottom;//normal3.crossProduct(tangent3);
	Vector3 bitangent4 = topToBottom;//normal4.crossProduct(tangent4);
	quads.push_back(Vertex_PCTT(Vector3(xVal, maxY, bottomLeftCorner.z), tint, texCoords.maxs, tangent1, bitangent1, normal1));
	quads.push_back(Vertex_PCTT(Vector3(xVal, maxY, maxZ), tint, Vector2(texCoords.maxs.x, texCoords.mins.y), tangent2, bitangent2, normal2));
	quads.push_back(Vertex_PCTT(Vector3(xVal, bottomLeftCorner.y, maxZ), tint, texCoords.mins, tangent3, bitangent3, normal3));
	quads.push_back(Vertex_PCTT(Vector3(xVal, bottomLeftCorner.y, bottomLeftCorner.z), tint, Vector2(texCoords.mins.x, texCoords.maxs.y), tangent4, bitangent4, normal4));
}

void MeshBuilder::getInvertedBottomSideQuadTang(std::vector<Vertex_PCTT>& quads, const Vector3& bottomLeftCorner, const Vector3& size, const Rgba& tint, const AABB2& texCoords)
{
	float maxX = bottomLeftCorner.x + size.x;
	float maxY = bottomLeftCorner.y + size.y;
	Vector3 normal1 = Vector3(bottomLeftCorner.x, bottomLeftCorner.y, bottomLeftCorner.z);
	Vector3 normal2 = Vector3(bottomLeftCorner.x, maxY, bottomLeftCorner.z);
	Vector3 normal3 = Vector3(maxX, maxY, bottomLeftCorner.z);
	Vector3 normal4 = Vector3(maxX, bottomLeftCorner.y, bottomLeftCorner.z);
	normal1.normalize();
	normal2.normalize();
	normal3.normalize();
	normal4.normalize();
	Vector3 leftToRight = Vector3(-maxX, 0.f, 0.f);
	Vector3 topToBottom = Vector3(0.f, -maxY, 0.f);
	leftToRight.normalize();
	topToBottom.normalize();
	normal1 = leftToRight.crossProduct(topToBottom);
	normal2 = leftToRight.crossProduct(topToBottom);
	normal3 = leftToRight.crossProduct(topToBottom);
	normal4 = leftToRight.crossProduct(topToBottom);
	Vector3 tangent1 = leftToRight;
	Vector3 tangent2 = leftToRight;
	Vector3 tangent3 = leftToRight;
	Vector3 tangent4 = leftToRight;
	Vector3 bitangent1 = topToBottom;//normal1.crossProduct(tangent1);
	Vector3 bitangent2 = topToBottom;//normal2.crossProduct(tangent2);
	Vector3 bitangent3 = topToBottom;//normal3.crossProduct(tangent3);
	Vector3 bitangent4 = topToBottom;//normal4.crossProduct(tangent4);

	quads.push_back(Vertex_PCTT(Vector3(maxX, bottomLeftCorner.y, bottomLeftCorner.z), tint, Vector2(texCoords.maxs.x, texCoords.mins.y), tangent4, bitangent4, normal4));
	quads.push_back(Vertex_PCTT(Vector3(maxX, maxY, bottomLeftCorner.z), tint, texCoords.maxs, tangent3, bitangent3, normal3));
	quads.push_back(Vertex_PCTT(Vector3(bottomLeftCorner.x, maxY, bottomLeftCorner.z), tint, Vector2(texCoords.mins.x, texCoords.maxs.y), tangent2, bitangent2, normal2));
	quads.push_back(Vertex_PCTT(Vector3(bottomLeftCorner.x, bottomLeftCorner.y, bottomLeftCorner.z), tint, texCoords.mins, tangent1, bitangent1, normal1));
}
void MeshBuilder::getInvertedTopSideQuadTang(std::vector<Vertex_PCTT>& quads, const Vector3& bottomLeftCorner, const Vector3& size, const Rgba& tint, const AABB2& texCoords)
{
	float zVal = bottomLeftCorner.z + size.z;
	float maxX = bottomLeftCorner.x + size.x;
	float maxY = bottomLeftCorner.y + size.y;
	Vector3 normal1 = Vector3(bottomLeftCorner.x, bottomLeftCorner.y, zVal);
	Vector3 normal2 = Vector3(maxX, bottomLeftCorner.y, zVal);
	Vector3 normal3 = Vector3(maxX, maxY, zVal);
	Vector3 normal4 = Vector3(bottomLeftCorner.x, maxY, zVal);
	normal1.normalize();
	normal2.normalize();
	normal3.normalize();
	normal4.normalize();
	Vector3 leftToRight = Vector3(maxX, 0.f, 0.f);
	Vector3 topToBottom = Vector3(0.f, maxY, 0.f);
	leftToRight.normalize();
	topToBottom.normalize();
	normal1 = leftToRight.crossProduct(topToBottom);
	normal2 = leftToRight.crossProduct(topToBottom);
	normal3 = leftToRight.crossProduct(topToBottom);
	normal4 = leftToRight.crossProduct(topToBottom);
	Vector3 tangent1 = leftToRight;
	Vector3 tangent2 = leftToRight;
	Vector3 tangent3 = leftToRight;
	Vector3 tangent4 = leftToRight;
	Vector3 bitangent1 = topToBottom;//normal1.crossProduct(tangent1);
	Vector3 bitangent2 = topToBottom;//normal2.crossProduct(tangent2);
	Vector3 bitangent3 = topToBottom;//normal3.crossProduct(tangent3);
	Vector3 bitangent4 = topToBottom;//normal4.crossProduct(tangent4);

	quads.push_back(Vertex_PCTT(Vector3(bottomLeftCorner.x, maxY, zVal), tint, Vector2(texCoords.maxs.x, texCoords.mins.y), tangent4, bitangent4, normal4));
	quads.push_back(Vertex_PCTT(Vector3(maxX, maxY, zVal), tint, texCoords.maxs, tangent3, bitangent3, normal3));
	quads.push_back(Vertex_PCTT(Vector3(maxX, bottomLeftCorner.y, zVal), tint, Vector2(texCoords.mins.x, texCoords.maxs.y), tangent2, bitangent2, normal2));
	quads.push_back(Vertex_PCTT(Vector3(bottomLeftCorner.x, bottomLeftCorner.y, zVal), tint, texCoords.mins, tangent1, bitangent1, normal1));
}
void MeshBuilder::getInvertedNorthSideQuadTang(std::vector<Vertex_PCTT>& quads, const Vector3& bottomLeftCorner, const Vector3& size, const Rgba& tint, const AABB2& texCoords)
{
	float yVal = bottomLeftCorner.y + size.y;
	float maxZ = bottomLeftCorner.z + size.z;
	float maxX = bottomLeftCorner.x + size.x;
	Vector3 normal1 = Vector3(bottomLeftCorner.x, yVal, bottomLeftCorner.z);
	Vector3 normal2 = Vector3(bottomLeftCorner.x, yVal, maxZ);
	Vector3 normal3 = Vector3(maxX, yVal, maxZ);
	Vector3 normal4 = Vector3(maxX, yVal, bottomLeftCorner.z);
	normal1.normalize();
	normal2.normalize();
	normal3.normalize();
	normal4.normalize();
	Vector3 leftToRight = Vector3(maxX, 0.f, 0.f);
	Vector3 topToBottom = Vector3(0.f, 0.f, maxZ);
	leftToRight.normalize();
	topToBottom.normalize();
	normal1 = leftToRight.crossProduct(topToBottom);
	normal2 = leftToRight.crossProduct(topToBottom);
	normal3 = leftToRight.crossProduct(topToBottom);
	normal4 = leftToRight.crossProduct(topToBottom);
	Vector3 tangent1 = leftToRight;
	Vector3 tangent2 = leftToRight;
	Vector3 tangent3 = leftToRight;
	Vector3 tangent4 = leftToRight;
	Vector3 bitangent1 = topToBottom;
	Vector3 bitangent2 = topToBottom;
	Vector3 bitangent3 = topToBottom;
	Vector3 bitangent4 = topToBottom;
	quads.push_back(Vertex_PCTT(Vector3(maxX, yVal, bottomLeftCorner.z), tint, Vector2(texCoords.mins.x, texCoords.maxs.y), tangent4, bitangent4, normal4));
	quads.push_back(Vertex_PCTT(Vector3(maxX, yVal, maxZ), tint, texCoords.mins, tangent3, bitangent3, normal3));
	quads.push_back(Vertex_PCTT(Vector3(bottomLeftCorner.x, yVal, maxZ), tint, Vector2(texCoords.maxs.x, texCoords.mins.y), tangent2, bitangent2,normal2));
	quads.push_back(Vertex_PCTT(Vector3(bottomLeftCorner.x, yVal, bottomLeftCorner.z), tint, texCoords.maxs, tangent1, bitangent1,normal1));
}
void MeshBuilder::getInvertedSouthSideQuadTang(std::vector<Vertex_PCTT>& quads, const Vector3& bottomLeftCorner, const Vector3& size, const Rgba& tint, const AABB2& texCoords)
{
	float maxX = bottomLeftCorner.x + size.x;
	float maxZ = bottomLeftCorner.z + size.z;
	Vector3 normal1 = Vector3(maxX, bottomLeftCorner.y, bottomLeftCorner.z);
	Vector3 normal2 = Vector3(maxX, bottomLeftCorner.y, maxZ);
	Vector3 normal3 = Vector3(bottomLeftCorner.x, bottomLeftCorner.y, maxZ);
	Vector3 normal4 = Vector3(bottomLeftCorner.x, bottomLeftCorner.y, bottomLeftCorner.z);
	normal1.normalize();
	normal2.normalize();
	normal3.normalize();
	normal4.normalize();
	Vector3 leftToRight = Vector3(-maxX, 0.f, 0.f);
	Vector3 topToBottom = Vector3(0.f, 0.f, maxZ);
	leftToRight.normalize();
	topToBottom.normalize();
	normal1 = leftToRight.crossProduct(topToBottom);
	normal2 = leftToRight.crossProduct(topToBottom);
	normal3 = leftToRight.crossProduct(topToBottom);
	normal4 = leftToRight.crossProduct(topToBottom);
	Vector3 tangent1 = leftToRight;
	Vector3 tangent2 = leftToRight;
	Vector3 tangent3 = leftToRight;
	Vector3 tangent4 = leftToRight;
	Vector3 bitangent1 = topToBottom;
	Vector3 bitangent2 = topToBottom;
	Vector3 bitangent3 = topToBottom;
	Vector3 bitangent4 = topToBottom;
	quads.push_back(Vertex_PCTT(Vector3(bottomLeftCorner.x, bottomLeftCorner.y, bottomLeftCorner.z), tint, Vector2(texCoords.mins.x, texCoords.maxs.y), tangent4, bitangent4,normal4));
	quads.push_back(Vertex_PCTT(Vector3(bottomLeftCorner.x, bottomLeftCorner.y, maxZ), tint, texCoords.mins, tangent3, bitangent3,normal3));
	quads.push_back(Vertex_PCTT(Vector3(maxX, bottomLeftCorner.y, maxZ), tint, Vector2(texCoords.maxs.x, texCoords.mins.y), tangent2, bitangent2,normal2));
	quads.push_back(Vertex_PCTT(Vector3(maxX, bottomLeftCorner.y, bottomLeftCorner.z), tint, texCoords.maxs, tangent1, bitangent1,normal1));
}
void MeshBuilder::getInvertedWestSideQuadTang(std::vector<Vertex_PCTT>& quads, const Vector3& bottomLeftCorner, const Vector3& size, const Rgba& tint, const AABB2& texCoords)
{
	float maxY = bottomLeftCorner.y + size.y;
	float maxZ = bottomLeftCorner.z + size.z;
	Vector3 normal1 = Vector3(bottomLeftCorner.x, bottomLeftCorner.y, bottomLeftCorner.z);
	Vector3 normal2 = Vector3(bottomLeftCorner.x, bottomLeftCorner.y, maxZ);
	Vector3 normal3 = Vector3(bottomLeftCorner.x, maxY, maxZ);
	Vector3 normal4 = Vector3(bottomLeftCorner.x, maxY, bottomLeftCorner.z);
	normal1.normalize();
	normal2.normalize();
	normal3.normalize();
	normal4.normalize();
	Vector3 leftToRight = Vector3(0.f, maxY, 0.f);
	Vector3 topToBottom = Vector3(0.f, 0.f, maxZ);
	leftToRight.normalize();
	topToBottom.normalize();
	normal1 = leftToRight.crossProduct(topToBottom);
	normal2 = leftToRight.crossProduct(topToBottom);
	normal3 = leftToRight.crossProduct(topToBottom);
	normal4 = leftToRight.crossProduct(topToBottom);
	Vector3 tangent1 = leftToRight;
	Vector3 tangent2 = leftToRight;
	Vector3 tangent3 = leftToRight;
	Vector3 tangent4 = leftToRight;
	Vector3 bitangent1 = topToBottom;
	Vector3 bitangent2 = topToBottom;
	Vector3 bitangent3 = topToBottom;
	Vector3 bitangent4 = topToBottom;
	quads.push_back(Vertex_PCTT(Vector3(bottomLeftCorner.x, maxY, bottomLeftCorner.z), tint, Vector2(texCoords.mins.x, texCoords.maxs.y), tangent4, bitangent4,normal4));
	quads.push_back(Vertex_PCTT(Vector3(bottomLeftCorner.x, maxY, maxZ), tint, texCoords.mins, tangent3, bitangent3,normal3));
	quads.push_back(Vertex_PCTT(Vector3(bottomLeftCorner.x, bottomLeftCorner.y, maxZ), tint, Vector2(texCoords.maxs.x, texCoords.mins.y), tangent2, bitangent2,normal2));
	quads.push_back(Vertex_PCTT(Vector3(bottomLeftCorner.x, bottomLeftCorner.y, bottomLeftCorner.z), tint, texCoords.maxs, tangent1, bitangent1,normal1));
}
void MeshBuilder::getInvertedEastSideQuadTang(std::vector<Vertex_PCTT>& quads, const Vector3& bottomLeftCorner, const Vector3& size, const Rgba& tint, const AABB2& texCoords)
{
	float xVal = bottomLeftCorner.x + size.x;
	float maxY = bottomLeftCorner.y + size.y;
	float maxZ = bottomLeftCorner.z + size.z;
	Vector3 normal1 = Vector3(xVal, maxY, bottomLeftCorner.z);
	Vector3 normal2 = Vector3(xVal, maxY, maxZ);
	Vector3 normal3 = Vector3(xVal, bottomLeftCorner.y, maxZ);
	Vector3 normal4 = Vector3(xVal, bottomLeftCorner.y, bottomLeftCorner.z);
	normal1.normalize();
	normal2.normalize();
	normal3.normalize();
	normal4.normalize();
	Vector3 leftToRight = Vector3(0.f, -maxY, 0.f);
	Vector3 topToBottom = Vector3(0.f, 0.f, maxZ);
	leftToRight.normalize();
	topToBottom.normalize();
	normal1 = leftToRight.crossProduct(topToBottom);
	normal2 = leftToRight.crossProduct(topToBottom);
	normal3 = leftToRight.crossProduct(topToBottom);
	normal4 = leftToRight.crossProduct(topToBottom);
	Vector3 tangent1 = leftToRight;
	Vector3 tangent2 = leftToRight;
	Vector3 tangent3 = leftToRight;
	Vector3 tangent4 = leftToRight;
	Vector3 bitangent1 = topToBottom;
	Vector3 bitangent2 = topToBottom;
	Vector3 bitangent3 = topToBottom;
	Vector3 bitangent4 = topToBottom;
	quads.push_back(Vertex_PCTT(Vector3(xVal, bottomLeftCorner.y, bottomLeftCorner.z), tint, Vector2(texCoords.mins.x, texCoords.maxs.y), tangent4, bitangent4,normal4));
	quads.push_back(Vertex_PCTT(Vector3(xVal, bottomLeftCorner.y, maxZ), tint, texCoords.mins, tangent3, bitangent3,normal3));
	quads.push_back(Vertex_PCTT(Vector3(xVal, maxY, maxZ), tint, Vector2(texCoords.maxs.x, texCoords.mins.y), tangent2, bitangent2,normal2));
	quads.push_back(Vertex_PCTT(Vector3(xVal, maxY, bottomLeftCorner.z), tint, texCoords.maxs, tangent1, bitangent1,normal1));
}

//!----
//Simple Build copyable Mesh


void MeshBuilder::getBottomSideQuadTangMasterVertex(std::vector<Master_Vertex>& quads, const Vector3& size, const AABB2& texCoords)
{

	Master_Vertex SWBottom = m_stamp;
	Master_Vertex NWBottom = m_stamp;
	Master_Vertex NEBottom = m_stamp;
	Master_Vertex SEBottom = m_stamp;

	NWBottom.position = Vector3(SWBottom.position.x, SWBottom.position.y + size.y, SWBottom.position.z);
	NEBottom.position = Vector3(SWBottom.position.x + size.x, SWBottom.position.y + size.y, SWBottom.position.z);
	SEBottom.position = Vector3(SWBottom.position.x + size.x, SWBottom.position.y, SWBottom.position.z);

	Vector3 normal1 = SWBottom.position;
	Vector3 normal2 = NWBottom.position;
	Vector3 normal3 = NEBottom.position;
	Vector3 normal4 = SEBottom.position;
	//texCoords = mins and maxs
	normal1.normalize();
	normal2.normalize();
	normal3.normalize();
	normal4.normalize();
	Vector3 leftToRight = Vector3(m_stamp.position.x + size.x, 0.f, 0.f);
	Vector3 topToBottom = Vector3(0.f, m_stamp.position.y + size.y, 0.f);

	//Vector3 leftToRight = Vector3(maxX, 0.f, 0.f);
	//Vector3 topToBottom = Vector3(0.f, maxY, 0.f);
	leftToRight.normalize();
	topToBottom.normalize();
	Vector3 tangent1 = leftToRight;
	Vector3 tangent2 = leftToRight;
	Vector3 tangent3 = leftToRight;
	Vector3 tangent4 = leftToRight;
	Vector3 bitangent1 = topToBottom; //normal1.crossProduct(tangent1);
	Vector3 bitangent2 = topToBottom; //normal2.crossProduct(tangent2);
	Vector3 bitangent3 = topToBottom; //normal3.crossProduct(tangent3);
	Vector3 bitangent4 = topToBottom; //normal4.crossProduct(tangent4);
	normal1 = leftToRight.crossProduct(topToBottom);
	normal2 = leftToRight.crossProduct(topToBottom);
	normal3 = leftToRight.crossProduct(topToBottom);
	normal4 = leftToRight.crossProduct(topToBottom);

	SWBottom.normal = normal1;
	NWBottom.normal = normal2;
	NEBottom.normal = normal3;
	SEBottom.normal = normal4;

	SWBottom.tangent = tangent1;
	NWBottom.tangent = tangent2;
	NEBottom.tangent = tangent3;
	SEBottom.tangent = tangent4;
	
	SWBottom.bitangent = bitangent1;
	NWBottom.bitangent = bitangent2;
	NEBottom.bitangent = bitangent3;
	SEBottom.bitangent = bitangent4;

	/*

	quads.push_back(Vertex_PCTT(Vector3(bottomLeftCorner.x, bottomLeftCorner.y, bottomLeftCorner.z), tint, texCoords.mins, tangent1, bitangent1));
	quads.push_back(Vertex_PCTT(Vector3(bottomLeftCorner.x, maxY, bottomLeftCorner.z), tint, Vector2(texCoords.mins.x, texCoords.maxs.y), tangent2, bitangent2));
	quads.push_back(Vertex_PCTT(Vector3(maxX, maxY, bottomLeftCorner.z), tint, texCoords.maxs, tangent3, bitangent3));
	quads.push_back(Vertex_PCTT(Vector3(maxX, bottomLeftCorner.y, bottomLeftCorner.z), tint, Vector2(texCoords.maxs.x, texCoords.mins.y), tangent4, bitangent4));
	*/

	SWBottom.texCoord[0] = texCoords.mins;
	NWBottom.texCoord[0] = Vector2(texCoords.mins.x, texCoords.maxs.y);
	NEBottom.texCoord[0] = texCoords.maxs;
	SEBottom.texCoord[0] = Vector2(texCoords.maxs.x, texCoords.mins.y);

	SWBottom.bone_weights_and_indicies = skin_weight_t(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0, 0, 0, 0));
	NWBottom.bone_weights_and_indicies = skin_weight_t(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0, 0, 0, 0));
	NEBottom.bone_weights_and_indicies = skin_weight_t(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0, 0, 0, 0));
	SEBottom.bone_weights_and_indicies = skin_weight_t(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0, 0, 0, 0));

	quads.push_back(SWBottom);
	quads.push_back(NWBottom);
	quads.push_back(NEBottom);
	quads.push_back(SEBottom);
}
void MeshBuilder::getTopSideQuadTangMasterVertex(std::vector<Master_Vertex>& quads, const Vector3& size, const AABB2& texCoords)
{

	Master_Vertex SWTop = m_stamp;
	Master_Vertex NWTop = m_stamp;
	Master_Vertex NETop = m_stamp;
	Master_Vertex SETop = m_stamp;

	SWTop.position = Vector3(SWTop.position.x, SWTop.position.y, SWTop.position.z + size.z);
	NWTop.position = Vector3(SWTop.position.x, SWTop.position.y + size.y, SWTop.position.z);
	NETop.position = Vector3(SWTop.position.x + size.x, SWTop.position.y + size.y, SWTop.position.z);
	SETop.position = Vector3(SWTop.position.x + size.x, SWTop.position.y, SWTop.position.z);


	Vector3 normal1 = SWTop.position;
	Vector3 normal2 = NWTop.position;
	Vector3 normal3 = NETop.position;
	Vector3 normal4 = SETop.position;
	normal1.normalize();
	normal2.normalize();
	normal3.normalize();
	normal4.normalize();
	/*
	Vector3 leftToRight = Vector3(-maxX, 0.f, 0.f);
	Vector3 topToBottom = Vector3(0.f, -maxY, 0.f);
	leftToRight.normalize();
	topToBottom.normalize();
	*/
	Vector3 leftToRight = Vector3(m_stamp.position.x, 0.f, 0.f);
	Vector3 topToBottom = Vector3(0.f, m_stamp.position.y, 0.f);
	leftToRight.normalize();
	topToBottom.normalize();
	Vector3 tangent1 = leftToRight;
	Vector3 tangent2 = leftToRight;
	Vector3 tangent3 = leftToRight;
	Vector3 tangent4 = leftToRight;
	Vector3 bitangent1 = topToBottom;//normal1.crossProduct(tangent1);
	Vector3 bitangent2 = topToBottom;//normal2.crossProduct(tangent2);
	Vector3 bitangent3 = topToBottom;//normal3.crossProduct(tangent3);
	Vector3 bitangent4 = topToBottom;//normal4.crossProduct(tangent4);
	normal1 = leftToRight.crossProduct(topToBottom);
	normal2 = leftToRight.crossProduct(topToBottom);
	normal3 = leftToRight.crossProduct(topToBottom);
	normal4 = leftToRight.crossProduct(topToBottom);

	SWTop.normal = normal1;
	NWTop.normal = normal2;
	NETop.normal = normal3;
	SETop.normal = normal4;

	SWTop.tangent = tangent1;
	NWTop.tangent = tangent2;
	NETop.tangent = tangent3;
	SETop.tangent = tangent4;

	SWTop.bitangent = bitangent1;
	NWTop.bitangent = bitangent2;
	NETop.bitangent = bitangent3;
	SETop.bitangent = bitangent4;

	/*

	quads.push_back(Vertex_PCTT(Vector3(bottomLeftCorner.x, bottomLeftCorner.y, zVal), tint, texCoords.mins, tangent1, bitangent1));
	quads.push_back(Vertex_PCTT(Vector3(maxX, bottomLeftCorner.y, zVal), tint, Vector2(texCoords.mins.x, texCoords.maxs.y), tangent2, bitangent2));
	quads.push_back(Vertex_PCTT(Vector3(maxX, maxY, zVal), tint, texCoords.maxs, tangent3, bitangent3));
	quads.push_back(Vertex_PCTT(Vector3(bottomLeftCorner.x, maxY, zVal), tint, Vector2(texCoords.maxs.x, texCoords.mins.y), tangent4, bitangent4));
	*/

	SWTop.texCoord[0] = texCoords.mins;
	NWTop.texCoord[0] = Vector2(texCoords.mins.x, texCoords.maxs.y);
	NETop.texCoord[0] = texCoords.maxs;
	SETop.texCoord[0] = Vector2(texCoords.maxs.x, texCoords.mins.y);

	SWTop.bone_weights_and_indicies = skin_weight_t(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0, 0, 0, 0));
	NWTop.bone_weights_and_indicies = skin_weight_t(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0, 0, 0, 0));
	NETop.bone_weights_and_indicies = skin_weight_t(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0, 0, 0, 0));
	SETop.bone_weights_and_indicies = skin_weight_t(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0, 0, 0, 0));

	quads.push_back(SETop);
	quads.push_back(NETop);
	quads.push_back(NWTop);
	quads.push_back(SWTop);
}
void MeshBuilder::getNorthSideQuadTangMasterVertex(std::vector<Master_Vertex>& quads, const Vector3& size, const AABB2& texCoords)
{
	Master_Vertex NWBottom = m_stamp;
	Master_Vertex NWTop = m_stamp;
	Master_Vertex NETop = m_stamp;
	Master_Vertex NEBottom = m_stamp;

	NWBottom.position = Vector3(NWBottom.position.x, NWBottom.position.y + size.y, NWBottom.position.z);
	NWTop.position = Vector3(NWBottom.position.x, NWBottom.position.y, NWBottom.position.z + size.z);
	NETop.position = Vector3(NWBottom.position.x + size.x, NWBottom.position.y, NWBottom.position.z + size.z);
	NEBottom.position = Vector3(NWBottom.position.x + size.x, NWBottom.position.y, NWBottom.position.z);

	Vector3 normal1 = NWBottom.position;
	Vector3 normal2 = NWTop.position;
	Vector3 normal3 = NETop.position;
	Vector3 normal4 = NEBottom.position;
	
	normal1.normalize();
	normal2.normalize();
	normal3.normalize();
	normal4.normalize();

	/*

	Vector3 leftToRight = Vector3(-maxX, 0.f, 0.f);
	Vector3 topToBottom = Vector3(0.f, 0.f, maxZ);
	leftToRight.normalize();
	topToBottom.normalize();
	*/

	Vector3 leftToRight = Vector3(m_stamp.position.x, 0.f, 0.f);
	Vector3 topToBottom = Vector3(0.f, 0.f, m_stamp.position.z + size.z);
	leftToRight.normalize();
	topToBottom.normalize();

	Vector3 tangent1 = leftToRight;
	Vector3 tangent2 = leftToRight;
	Vector3 tangent3 = leftToRight;
	Vector3 tangent4 = leftToRight;

	Vector3 bitangent1 = topToBottom;//normal1.crossProduct(tangent1);
	Vector3 bitangent2 = topToBottom;//normal2.crossProduct(tangent2);
	Vector3 bitangent3 = topToBottom;//normal3.crossProduct(tangent3);
	Vector3 bitangent4 = topToBottom;//normal4.crossProduct(tangent4);
	normal1 = leftToRight.crossProduct(topToBottom);
	normal2 = leftToRight.crossProduct(topToBottom);
	normal3 = leftToRight.crossProduct(topToBottom);
	normal4 = leftToRight.crossProduct(topToBottom);

	NWBottom.normal = normal1;
	NWTop.normal = normal2;
	NETop.normal = normal3;
	NEBottom.normal = normal4;

	NWBottom.tangent = tangent1;
	NWTop.tangent = tangent2;
	NETop.tangent = tangent3;
	NEBottom.tangent = tangent4;

	NWBottom.bitangent = bitangent1;
	NWTop.bitangent = bitangent2;
	NETop.bitangent = bitangent3;
	NEBottom.bitangent = bitangent4;

	/*

	quads.push_back(Vertex_PCTT(Vector3(bottomLeftCorner.x, yVal, bottomLeftCorner.z), tint, texCoords.maxs, tangent1, bitangent1));
	quads.push_back(Vertex_PCTT(Vector3(bottomLeftCorner.x, yVal, maxZ), tint, Vector2(texCoords.maxs.x, texCoords.mins.y), tangent2, bitangent2));
	quads.push_back(Vertex_PCTT(Vector3(maxX, yVal, maxZ), tint, texCoords.mins, tangent3, bitangent3));
	quads.push_back(Vertex_PCTT(Vector3(maxX, yVal, bottomLeftCorner.z), tint, Vector2(texCoords.mins.x, texCoords.maxs.y), tangent4, bitangent4));
	*/

	NWBottom.texCoord[0] = texCoords.maxs;
	NWTop.texCoord[0] = Vector2(texCoords.maxs.x, texCoords.mins.y);
	NETop.texCoord[0] = texCoords.mins;
	NEBottom.texCoord[0] = Vector2(texCoords.mins.x, texCoords.maxs.y);

	NWBottom.bone_weights_and_indicies = skin_weight_t(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0, 0, 0, 0));
	NWTop.bone_weights_and_indicies = skin_weight_t(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0, 0, 0, 0));
	NETop.bone_weights_and_indicies = skin_weight_t(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0, 0, 0, 0));
	NEBottom.bone_weights_and_indicies = skin_weight_t(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0, 0, 0, 0));

	quads.push_back(NWBottom);
	quads.push_back(NWTop);
	quads.push_back(NETop);
	quads.push_back(NEBottom);
}
void MeshBuilder::getSouthSideQuadTangMasterVertex(std::vector<Master_Vertex>& quads, const Vector3& size, const AABB2& texCoords)
{
	Master_Vertex SWBottom = m_stamp;
	Master_Vertex SWTop = m_stamp;
	Master_Vertex SETop = m_stamp;
	Master_Vertex SEBottom = m_stamp;

	SWBottom.position = Vector3(SWBottom.position.x, SWBottom.position.y, SWBottom.position.z);
	SWTop.position = Vector3(SWBottom.position.x, SWBottom.position.y, SWBottom.position.z + size.z);
	SETop.position = Vector3(SWBottom.position.x + size.x, SWBottom.position.y, SWBottom.position.z + size.z);
	SEBottom.position = Vector3(SWBottom.position.x + size.x, SWBottom.position.y, SWBottom.position.z);

	Vector3 normal1 = SWBottom.position;
	Vector3 normal2 = SWTop.position;
	Vector3 normal3 = SETop.position;
	Vector3 normal4 = SEBottom.position;

	normal1.normalize();
	normal2.normalize();
	normal3.normalize();
	normal4.normalize();

	/*

	Vector3 leftToRight = Vector3(maxX, 0.f, 0.f);
	Vector3 topToBottom = Vector3(0.f, 0.f, maxZ);
	leftToRight.normalize();
	topToBottom.normalize();
	*/

	Vector3 leftToRight = Vector3(m_stamp.position.x + size.x, 0.f, 0.f);
	Vector3 topToBottom = Vector3(0.f, 0.f, m_stamp.position.z + size.z);
	leftToRight.normalize();
	topToBottom.normalize();

	Vector3 tangent1 = leftToRight;
	Vector3 tangent2 = leftToRight;
	Vector3 tangent3 = leftToRight;
	Vector3 tangent4 = leftToRight;

	Vector3 bitangent1 = topToBottom;//normal1.crossProduct(tangent1);
	Vector3 bitangent2 = topToBottom;//normal2.crossProduct(tangent2);
	Vector3 bitangent3 = topToBottom;//normal3.crossProduct(tangent3);
	Vector3 bitangent4 = topToBottom;//normal4.crossProduct(tangent4);
	normal1 = leftToRight.crossProduct(topToBottom);
	normal2 = leftToRight.crossProduct(topToBottom);
	normal3 = leftToRight.crossProduct(topToBottom);
	normal4 = leftToRight.crossProduct(topToBottom);

	SWBottom.normal = normal1;
	SWTop.normal = normal2;
	SETop.normal = normal3;
	SEBottom.normal = normal4;

	SWBottom.tangent = tangent1;
	SWTop.tangent = tangent2;
	SETop.tangent = tangent3;
	SEBottom.tangent = tangent4;

	SWBottom.bitangent = bitangent1;
	SWTop.bitangent = bitangent2;
	SETop.bitangent = bitangent3;
	SEBottom.bitangent = bitangent4;

	/*

	quads.push_back(Vertex_PCTT(Vector3(maxX, bottomLeftCorner.y, bottomLeftCorner.z), tint, texCoords.maxs, tangent1, bitangent1));
	quads.push_back(Vertex_PCTT(Vector3(maxX, bottomLeftCorner.y, maxZ), tint, Vector2(texCoords.maxs.x, texCoords.mins.y), tangent2, bitangent2));
	quads.push_back(Vertex_PCTT(Vector3(bottomLeftCorner.x, bottomLeftCorner.y, maxZ), tint, texCoords.mins, tangent3, bitangent3));
	quads.push_back(Vertex_PCTT(Vector3(bottomLeftCorner.x, bottomLeftCorner.y, bottomLeftCorner.z), tint, Vector2(texCoords.mins.x, texCoords.maxs.y), tangent4, bitangent4));
	*/

	SWBottom.texCoord[0] = texCoords.maxs;
	SWTop.texCoord[0] = Vector2(texCoords.maxs.x, texCoords.mins.y);
	SETop.texCoord[0] = texCoords.mins;
	SEBottom.texCoord[0] = Vector2(texCoords.mins.x, texCoords.maxs.y);

	SWBottom.bone_weights_and_indicies = skin_weight_t(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0, 0, 0, 0));
	SWTop.bone_weights_and_indicies = skin_weight_t(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0, 0, 0, 0));
	SETop.bone_weights_and_indicies = skin_weight_t(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0, 0, 0, 0));
	SEBottom.bone_weights_and_indicies = skin_weight_t(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0, 0, 0, 0));

	quads.push_back(SEBottom);
	quads.push_back(SETop);
	quads.push_back(SWTop);
	quads.push_back(SWBottom);
}
void MeshBuilder::getWestSideQuadTangMasterVertex(std::vector<Master_Vertex>& quads, const Vector3& size, const AABB2& texCoords)
{
	Master_Vertex SWBottom = m_stamp;
	Master_Vertex SWTop = m_stamp;
	Master_Vertex NWTop = m_stamp;
	Master_Vertex NWBottom = m_stamp;

	SWBottom.position = Vector3(SWBottom.position.x, SWBottom.position.y, SWBottom.position.z);
	SWTop.position = Vector3(SWBottom.position.x, SWBottom.position.y, SWBottom.position.z + size.z);
	NWTop.position = Vector3(SWBottom.position.x, SWBottom.position.y + size.y, SWBottom.position.z + size.z);
	NWBottom.position = Vector3(SWBottom.position.x, SWBottom.position.y + size.y, SWBottom.position.z);

	Vector3 normal1 = SWBottom.position;
	Vector3 normal2 = SWTop.position;
	Vector3 normal3 = NWTop.position;
	Vector3 normal4 = NWBottom.position;

	normal1.normalize();
	normal2.normalize();
	normal3.normalize();
	normal4.normalize();

	/*

	Vector3 leftToRight = Vector3(0.f, -maxY, 0.f);
	Vector3 topToBottom = Vector3(0.f, 0.f, maxZ);
	leftToRight.normalize();
	topToBottom.normalize();
	*/

	Vector3 leftToRight = Vector3(0.f, m_stamp.position.y, 0.f);
	Vector3 topToBottom = Vector3(0.f, 0.f, m_stamp.position.z + size.z);
	leftToRight.normalize();
	topToBottom.normalize();

	Vector3 tangent1 = leftToRight;
	Vector3 tangent2 = leftToRight;
	Vector3 tangent3 = leftToRight;
	Vector3 tangent4 = leftToRight;

	Vector3 bitangent1 = topToBottom;//normal1.crossProduct(tangent1);
	Vector3 bitangent2 = topToBottom;//normal2.crossProduct(tangent2);
	Vector3 bitangent3 = topToBottom;//normal3.crossProduct(tangent3);
	Vector3 bitangent4 = topToBottom;//normal4.crossProduct(tangent4);
	normal1 = leftToRight.crossProduct(topToBottom);
	normal2 = leftToRight.crossProduct(topToBottom);
	normal3 = leftToRight.crossProduct(topToBottom);
	normal4 = leftToRight.crossProduct(topToBottom);

	SWBottom.normal = normal1;
	SWTop.normal = normal2;
	NWTop.normal = normal3;
	NWBottom.normal = normal4;

	SWBottom.tangent = tangent1;
	SWTop.tangent = tangent2;
	NWTop.tangent = tangent3;
	NWBottom.tangent = tangent4;

	SWBottom.bitangent = bitangent1;
	SWTop.bitangent = bitangent2;
	NWTop.bitangent = bitangent3;
	NWBottom.bitangent = bitangent4;

	SWBottom.texCoord[0] = texCoords.maxs;
	SWTop.texCoord[0] = Vector2(texCoords.maxs.x, texCoords.mins.y);
	NWTop.texCoord[0] = texCoords.mins;
	NWBottom.texCoord[0] = Vector2(texCoords.mins.x, texCoords.maxs.y);

	SWBottom.bone_weights_and_indicies = skin_weight_t(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0, 0, 0, 0));
	SWTop.bone_weights_and_indicies = skin_weight_t(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0, 0, 0, 0));
	NWTop.bone_weights_and_indicies = skin_weight_t(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0, 0, 0, 0));
	NWBottom.bone_weights_and_indicies = skin_weight_t(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0, 0, 0, 0));

	/*

	quads.push_back(Vertex_PCTT(Vector3(bottomLeftCorner.x, bottomLeftCorner.y, bottomLeftCorner.z), tint, texCoords.maxs, tangent1, bitangent1));
	quads.push_back(Vertex_PCTT(Vector3(bottomLeftCorner.x, bottomLeftCorner.y, maxZ), tint, Vector2(texCoords.maxs.x, texCoords.mins.y), tangent2, bitangent2));
	quads.push_back(Vertex_PCTT(Vector3(bottomLeftCorner.x, maxY, maxZ), tint, texCoords.mins, tangent3, bitangent3));
	quads.push_back(Vertex_PCTT(Vector3(bottomLeftCorner.x, maxY, bottomLeftCorner.z), tint, Vector2(texCoords.mins.x, texCoords.maxs.y), tangent4, bitangent4));
	*/

	quads.push_back(SWBottom);
	quads.push_back(SWTop);
	quads.push_back(NWTop);
	quads.push_back(NWBottom);
}
void MeshBuilder::getEastSideQuadTangMasterVertex(std::vector<Master_Vertex>& quads, const Vector3& size, const AABB2& texCoords)
{
	Master_Vertex SEBottom = m_stamp;
	Master_Vertex SETop = m_stamp;
	Master_Vertex NETop = m_stamp;
	Master_Vertex NEBottom = m_stamp;

	SEBottom.position = Vector3(SEBottom.position.x + size.x, SEBottom.position.y, SEBottom.position.z);
	SETop.position = Vector3(SEBottom.position.x, SEBottom.position.y, SEBottom.position.z + size.z);
	NETop.position = Vector3(SEBottom.position.x, SEBottom.position.y + size.y, SEBottom.position.z + size.z);
	NEBottom.position = Vector3(SEBottom.position.x, SEBottom.position.y + size.y, SEBottom.position.z);

	Vector3 normal1 = SEBottom.position;
	Vector3 normal2 = SETop.position;
	Vector3 normal3 = NETop.position;
	Vector3 normal4 = NEBottom.position;

	normal1.normalize();
	normal2.normalize();
	normal3.normalize();
	normal4.normalize();

	/*

	Vector3 leftToRight = Vector3(0.f, maxY, 0.f);
	Vector3 topToBottom = Vector3(0.f, 0.f, maxZ);
	leftToRight.normalize();
	topToBottom.normalize();
	*/

	Vector3 leftToRight = Vector3(0.f, m_stamp.position.y + size.y, 0.f);
	Vector3 topToBottom = Vector3(0.f, 0.f, m_stamp.position.z + size.z);
	leftToRight.normalize();
	topToBottom.normalize();

	Vector3 tangent1 = leftToRight;
	Vector3 tangent2 = leftToRight;
	Vector3 tangent3 = leftToRight;
	Vector3 tangent4 = leftToRight;

	Vector3 bitangent1 = topToBottom;//normal1.crossProduct(tangent1);
	Vector3 bitangent2 = topToBottom;//normal2.crossProduct(tangent2);
	Vector3 bitangent3 = topToBottom;//normal3.crossProduct(tangent3);
	Vector3 bitangent4 = topToBottom;//normal4.crossProduct(tangent4);
	normal1 = leftToRight.crossProduct(topToBottom);
	normal2 = leftToRight.crossProduct(topToBottom);
	normal3 = leftToRight.crossProduct(topToBottom);
	normal4 = leftToRight.crossProduct(topToBottom);

	SEBottom.normal = normal1;
	SETop.normal = normal2;
	NETop.normal = normal3;
	NEBottom.normal = normal4;

	SEBottom.tangent = tangent1;
	SETop.tangent = tangent2;
	NETop.tangent = tangent3;
	NEBottom.tangent = tangent4;

	SEBottom.bitangent = bitangent1;
	SETop.bitangent = bitangent2;
	NETop.bitangent = bitangent3;
	NEBottom.bitangent = bitangent4;

	/*

	quads.push_back(Vertex_PCTT(Vector3(xVal, maxY, bottomLeftCorner.z), tint, texCoords.maxs, tangent1, bitangent1));
	quads.push_back(Vertex_PCTT(Vector3(xVal, maxY, maxZ), tint, Vector2(texCoords.maxs.x, texCoords.mins.y), tangent2, bitangent2));
	quads.push_back(Vertex_PCTT(Vector3(xVal, bottomLeftCorner.y, maxZ), tint, texCoords.mins, tangent3, bitangent3));
	quads.push_back(Vertex_PCTT(Vector3(xVal, bottomLeftCorner.y, bottomLeftCorner.z), tint, Vector2(texCoords.mins.x, texCoords.maxs.y), tangent4, bitangent4));
	*/


	SEBottom.bone_weights_and_indicies = skin_weight_t(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0, 0, 0, 0));
	SETop.bone_weights_and_indicies = skin_weight_t(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0, 0, 0, 0));
	NETop.bone_weights_and_indicies = skin_weight_t(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0, 0, 0, 0));
	NEBottom.bone_weights_and_indicies = skin_weight_t(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0, 0, 0, 0));

	SEBottom.texCoord[0] = Vector2(texCoords.mins.x, texCoords.maxs.y);
	SETop.texCoord[0] = texCoords.mins;
	NETop.texCoord[0] = Vector2(texCoords.maxs.x, texCoords.mins.y);
	NEBottom.texCoord[0] = texCoords.maxs;

	quads.push_back(NEBottom);
	quads.push_back(NETop);
	quads.push_back(SETop);
	quads.push_back(SEBottom);
}

void MeshBuilder::getInvertedBottomSideQuadTangMasterVertex(std::vector<Master_Vertex>& quads, const Vector3& size, const AABB2& texCoords)
{

	Master_Vertex SWBottom = m_stamp;
	Master_Vertex NWBottom = m_stamp;
	Master_Vertex NEBottom = m_stamp;
	Master_Vertex SEBottom = m_stamp;

	NWBottom.position = Vector3(SWBottom.position.x, SWBottom.position.y + size.y, SWBottom.position.z);
	NEBottom.position = Vector3(SWBottom.position.x + size.x, SWBottom.position.y + size.y, SWBottom.position.z);
	SEBottom.position = Vector3(SWBottom.position.x + size.x, SWBottom.position.y, SWBottom.position.z);

	Vector3 normal1 = SWBottom.position;
	Vector3 normal2 = NWBottom.position;
	Vector3 normal3 = NEBottom.position;
	Vector3 normal4 = SEBottom.position;
	//texCoords = mins and maxs
	normal1.normalize();
	normal2.normalize();
	normal3.normalize();
	normal4.normalize();

	Vector3 leftToRight = Vector3(NEBottom.position.x, 0.f, 0.f);
	Vector3 topToBottom = Vector3(0.f, NEBottom.position.y, 0.f);
	leftToRight.normalize();
	topToBottom.normalize();
	Vector3 tangent1 = leftToRight;
	Vector3 tangent2 = leftToRight;
	Vector3 tangent3 = leftToRight;
	Vector3 tangent4 = leftToRight;
	Vector3 bitangent1 = topToBottom; //normal1.crossProduct(tangent1);
	Vector3 bitangent2 = topToBottom; //normal2.crossProduct(tangent2);
	Vector3 bitangent3 = topToBottom; //normal3.crossProduct(tangent3);
	Vector3 bitangent4 = topToBottom; //normal4.crossProduct(tangent4);
	normal1 = leftToRight.crossProduct(topToBottom);
	normal2 = leftToRight.crossProduct(topToBottom);
	normal3 = leftToRight.crossProduct(topToBottom);
	normal4 = leftToRight.crossProduct(topToBottom);

	SWBottom.normal = normal1;
	NWBottom.normal = normal2;
	NEBottom.normal = normal3;
	SEBottom.normal = normal4;

	SWBottom.tangent = tangent1;
	NWBottom.tangent = tangent2;
	NEBottom.tangent = tangent3;
	SEBottom.tangent = tangent4;

	SWBottom.bitangent = bitangent1;
	NWBottom.bitangent = bitangent2;
	NEBottom.bitangent = bitangent3;
	SEBottom.bitangent = bitangent4;

	/*

	quads.push_back(Vertex_PCTT(Vector3(bottomLeftCorner.x, bottomLeftCorner.y, bottomLeftCorner.z), tint, texCoords.mins, tangent1, bitangent1));
	quads.push_back(Vertex_PCTT(Vector3(bottomLeftCorner.x, maxY, bottomLeftCorner.z), tint, Vector2(texCoords.mins.x, texCoords.maxs.y), tangent2, bitangent2));
	quads.push_back(Vertex_PCTT(Vector3(maxX, maxY, bottomLeftCorner.z), tint, texCoords.maxs, tangent3, bitangent3));
	quads.push_back(Vertex_PCTT(Vector3(maxX, bottomLeftCorner.y, bottomLeftCorner.z), tint, Vector2(texCoords.maxs.x, texCoords.mins.y), tangent4, bitangent4));
	*/

	SWBottom.bone_weights_and_indicies = skin_weight_t(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0, 0, 0, 0));
	NWBottom.bone_weights_and_indicies = skin_weight_t(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0, 0, 0, 0));
	NEBottom.bone_weights_and_indicies = skin_weight_t(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0, 0, 0, 0));
	SEBottom.bone_weights_and_indicies = skin_weight_t(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0, 0, 0, 0));

	SWBottom.texCoord[0] = texCoords.mins;
	NWBottom.texCoord[0] = Vector2(texCoords.mins.x, texCoords.maxs.y);
	NEBottom.texCoord[0] = texCoords.maxs;
	SEBottom.texCoord[0] = Vector2(texCoords.maxs.x, texCoords.mins.y);

	quads.push_back(SEBottom);
	quads.push_back(NEBottom);
	quads.push_back(NWBottom);
	quads.push_back(SWBottom);
}
void MeshBuilder::getInvertedTopSideQuadTangMasterVertex(std::vector<Master_Vertex>& quads, const Vector3& size, const AABB2& texCoords)
{
	Master_Vertex SWTop = m_stamp;
	Master_Vertex NWTop = m_stamp;
	Master_Vertex NETop = m_stamp;
	Master_Vertex SETop = m_stamp;

	SWTop.position = Vector3(SWTop.position.x, SWTop.position.y, SWTop.position.z + size.z);
	NWTop.position = Vector3(SWTop.position.x, SWTop.position.y + size.y, SWTop.position.z);
	NETop.position = Vector3(SWTop.position.x + size.x, SWTop.position.y + size.y, SWTop.position.z);
	SETop.position = Vector3(SWTop.position.x + size.x, SWTop.position.y, SWTop.position.z);


	Vector3 normal1 = SWTop.position;
	Vector3 normal2 = NWTop.position;
	Vector3 normal3 = NETop.position;
	Vector3 normal4 = SETop.position;
	normal1.normalize();
	normal2.normalize();
	normal3.normalize();
	normal4.normalize();
	Vector3 leftToRight = Vector3(SWTop.position.x, 0.f, 0.f);
	Vector3 topToBottom = Vector3(0.f, SWTop.position.y, 0.f);
	leftToRight.normalize();
	topToBottom.normalize();
	Vector3 tangent1 = leftToRight;
	Vector3 tangent2 = leftToRight;
	Vector3 tangent3 = leftToRight;
	Vector3 tangent4 = leftToRight;
	Vector3 bitangent1 = topToBottom;//normal1.crossProduct(tangent1);
	Vector3 bitangent2 = topToBottom;//normal2.crossProduct(tangent2);
	Vector3 bitangent3 = topToBottom;//normal3.crossProduct(tangent3);
	Vector3 bitangent4 = topToBottom;//normal4.crossProduct(tangent4);
	normal1 = leftToRight.crossProduct(topToBottom);
	normal2 = leftToRight.crossProduct(topToBottom);
	normal3 = leftToRight.crossProduct(topToBottom);
	normal4 = leftToRight.crossProduct(topToBottom);

	SWTop.normal = normal1;
	NWTop.normal = normal2;
	NETop.normal = normal3;
	SETop.normal = normal4;

	SWTop.tangent = tangent1;
	NWTop.tangent = tangent2;
	NETop.tangent = tangent3;
	SETop.tangent = tangent4;

	SWTop.bitangent = bitangent1;
	NWTop.bitangent = bitangent2;
	NETop.bitangent = bitangent3;
	SETop.bitangent = bitangent4;

	/*

	quads.push_back(Vertex_PCTT(Vector3(bottomLeftCorner.x, bottomLeftCorner.y, zVal), tint, texCoords.mins, tangent1, bitangent1));
	quads.push_back(Vertex_PCTT(Vector3(maxX, bottomLeftCorner.y, zVal), tint, Vector2(texCoords.mins.x, texCoords.maxs.y), tangent2, bitangent2));
	quads.push_back(Vertex_PCTT(Vector3(maxX, maxY, zVal), tint, texCoords.maxs, tangent3, bitangent3));
	quads.push_back(Vertex_PCTT(Vector3(bottomLeftCorner.x, maxY, zVal), tint, Vector2(texCoords.maxs.x, texCoords.mins.y), tangent4, bitangent4));
	*/


	SWTop.bone_weights_and_indicies = skin_weight_t(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0, 0, 0, 0));
	NWTop.bone_weights_and_indicies = skin_weight_t(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0, 0, 0, 0));
	NETop.bone_weights_and_indicies = skin_weight_t(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0, 0, 0, 0));
	SETop.bone_weights_and_indicies = skin_weight_t(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0, 0, 0, 0));

	SWTop.texCoord[0] = Vector2(texCoords.maxs.x, texCoords.mins.y);
	NWTop.texCoord[0] = texCoords.maxs;
	NETop.texCoord[0] = Vector2(texCoords.mins.x, texCoords.maxs.y);
	SETop.texCoord[0] = texCoords.mins;

	quads.push_back(SWTop);
	quads.push_back(NWTop);
	quads.push_back(NETop);
	quads.push_back(SETop);
}
void MeshBuilder::getInvertedNorthSideQuadTangMasterVertex(std::vector<Master_Vertex>& quads, const Vector3& size, const AABB2& texCoords)
{
	Master_Vertex NWBottom = m_stamp;
	Master_Vertex NWTop = m_stamp;
	Master_Vertex NETop = m_stamp;
	Master_Vertex NEBottom = m_stamp;

	NWBottom.position = Vector3(NWBottom.position.x, NWBottom.position.y + size.y, NWBottom.position.z);
	NWTop.position = Vector3(NWBottom.position.x, NWBottom.position.y, NWBottom.position.z + size.z);
	NETop.position = Vector3(NWBottom.position.x + size.x, NWBottom.position.y, NWBottom.position.z + size.z);
	NEBottom.position = Vector3(NWBottom.position.x + size.x, NWBottom.position.y, NWBottom.position.z);

	Vector3 normal1 = NWBottom.position;
	Vector3 normal2 = NWTop.position;
	Vector3 normal3 = NETop.position;
	Vector3 normal4 = NEBottom.position;

	normal1.normalize();
	normal2.normalize();
	normal3.normalize();
	normal4.normalize();

	/*

	Vector3 leftToRight = Vector3(NWBottom.position.x, 0.f, 0.f);
	Vector3 topToBottom = Vector3(0.f, 0.f, NWTop.position.z);
	leftToRight.normalize();
	topToBottom.normalize();
	*/

	Vector3 leftToRight = Vector3(NWBottom.position.x, 0.f, 0.f);
	Vector3 topToBottom = Vector3(0.f, 0.f, NWTop.position.z);
	leftToRight.normalize();
	topToBottom.normalize();

	Vector3 tangent1 = leftToRight;
	Vector3 tangent2 = leftToRight;
	Vector3 tangent3 = leftToRight;
	Vector3 tangent4 = leftToRight;

	Vector3 bitangent1 = topToBottom;//normal1.crossProduct(tangent1);
	Vector3 bitangent2 = topToBottom;//normal2.crossProduct(tangent2);
	Vector3 bitangent3 = topToBottom;//normal3.crossProduct(tangent3);
	Vector3 bitangent4 = topToBottom;//normal4.crossProduct(tangent4);
	normal1 = leftToRight.crossProduct(topToBottom);
	normal2 = leftToRight.crossProduct(topToBottom);
	normal3 = leftToRight.crossProduct(topToBottom);
	normal4 = leftToRight.crossProduct(topToBottom);

	NWBottom.normal = normal1;
	NWTop.normal = normal2;
	NETop.normal = normal3;
	NEBottom.normal = normal4;

	NWBottom.tangent = tangent1;
	NWTop.tangent = tangent2;
	NETop.tangent = tangent3;
	NEBottom.tangent = tangent4;

	NWBottom.bitangent = bitangent1;
	NWTop.bitangent = bitangent2;
	NETop.bitangent = bitangent3;
	NEBottom.bitangent = bitangent4;

	/*

	quads.push_back(Vertex_PCTT(Vector3(bottomLeftCorner.x, yVal, bottomLeftCorner.z), tint, texCoords.maxs, tangent1, bitangent1));
	quads.push_back(Vertex_PCTT(Vector3(bottomLeftCorner.x, yVal, maxZ), tint, Vector2(texCoords.maxs.x, texCoords.mins.y), tangent2, bitangent2));
	quads.push_back(Vertex_PCTT(Vector3(maxX, yVal, maxZ), tint, texCoords.mins, tangent3, bitangent3));
	quads.push_back(Vertex_PCTT(Vector3(maxX, yVal, bottomLeftCorner.z), tint, Vector2(texCoords.mins.x, texCoords.maxs.y), tangent4, bitangent4));
	*/

	NWBottom.bone_weights_and_indicies = skin_weight_t(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0, 0, 0, 0));
	NWTop.bone_weights_and_indicies = skin_weight_t(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0, 0, 0, 0));
	NETop.bone_weights_and_indicies = skin_weight_t(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0, 0, 0, 0));
	NEBottom.bone_weights_and_indicies = skin_weight_t(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0, 0, 0, 0));


	NWBottom.texCoord[0] = Vector2(texCoords.mins.x, texCoords.maxs.y);
	NWTop.texCoord[0] = texCoords.mins;
	NETop.texCoord[0] = Vector2(texCoords.maxs.x, texCoords.mins.y);
	NEBottom.texCoord[0] = texCoords.maxs;

	quads.push_back(NEBottom);
	quads.push_back(NETop);
	quads.push_back(NWTop);
	quads.push_back(NWBottom);
}
void MeshBuilder::getInvertedSouthSideQuadTangMasterVertex(std::vector<Master_Vertex>& quads, const Vector3& size, const AABB2& texCoords)
{
	Master_Vertex SWBottom = m_stamp;
	Master_Vertex SWTop = m_stamp;
	Master_Vertex SETop = m_stamp;
	Master_Vertex SEBottom = m_stamp;

	SWBottom.position = Vector3(SWBottom.position.x, SWBottom.position.y, SWBottom.position.z);
	SWTop.position = Vector3(SWBottom.position.x, SWBottom.position.y, SWBottom.position.z + size.z);
	SETop.position = Vector3(SWBottom.position.x + size.x, SWBottom.position.y, SWBottom.position.z + size.z);
	SEBottom.position = Vector3(SWBottom.position.x + size.x, SWBottom.position.y, SWBottom.position.z);

	Vector3 normal1 = SWBottom.position;
	Vector3 normal2 = SWTop.position;
	Vector3 normal3 = SETop.position;
	Vector3 normal4 = SEBottom.position;

	normal1.normalize();
	normal2.normalize();
	normal3.normalize();
	normal4.normalize();

	Vector3 leftToRight = Vector3(SEBottom.position.x, 0.f, 0.f);
	Vector3 topToBottom = Vector3(0.f, 0.f, SWTop.position.z);
	leftToRight.normalize();
	topToBottom.normalize();

	Vector3 tangent1 = leftToRight;
	Vector3 tangent2 = leftToRight;
	Vector3 tangent3 = leftToRight;
	Vector3 tangent4 = leftToRight;

	Vector3 bitangent1 = topToBottom;//normal1.crossProduct(tangent1);
	Vector3 bitangent2 = topToBottom;//normal2.crossProduct(tangent2);
	Vector3 bitangent3 = topToBottom;//normal3.crossProduct(tangent3);
	Vector3 bitangent4 = topToBottom;//normal4.crossProduct(tangent4);
	normal1 = leftToRight.crossProduct(topToBottom);
	normal2 = leftToRight.crossProduct(topToBottom);
	normal3 = leftToRight.crossProduct(topToBottom);
	normal4 = leftToRight.crossProduct(topToBottom);

	SWBottom.normal = normal1;
	SWTop.normal = normal2;
	SETop.normal = normal3;
	SEBottom.normal = normal4;

	SWBottom.tangent = tangent1;
	SWTop.tangent = tangent2;
	SETop.tangent = tangent3;
	SEBottom.tangent = tangent4;

	SWBottom.bitangent = bitangent1;
	SWTop.bitangent = bitangent2;
	SETop.bitangent = bitangent3;
	SEBottom.bitangent = bitangent4;

	/*

	quads.push_back(Vertex_PCTT(Vector3(maxX, bottomLeftCorner.y, bottomLeftCorner.z), tint, texCoords.maxs, tangent1, bitangent1));
	quads.push_back(Vertex_PCTT(Vector3(maxX, bottomLeftCorner.y, maxZ), tint, Vector2(texCoords.maxs.x, texCoords.mins.y), tangent2, bitangent2));
	quads.push_back(Vertex_PCTT(Vector3(bottomLeftCorner.x, bottomLeftCorner.y, maxZ), tint, texCoords.mins, tangent3, bitangent3));
	quads.push_back(Vertex_PCTT(Vector3(bottomLeftCorner.x, bottomLeftCorner.y, bottomLeftCorner.z), tint, Vector2(texCoords.mins.x, texCoords.maxs.y), tangent4, bitangent4));
	*/

	SWBottom.texCoord[0] = Vector2(texCoords.mins.x, texCoords.maxs.y);
	SWTop.texCoord[0] = texCoords.mins;
	SETop.texCoord[0] = Vector2(texCoords.maxs.x, texCoords.mins.y);
	SEBottom.texCoord[0] = texCoords.maxs;


	SWBottom.bone_weights_and_indicies = skin_weight_t(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0, 0, 0, 0));
	SWTop.bone_weights_and_indicies = skin_weight_t(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0, 0, 0, 0));
	SETop.bone_weights_and_indicies = skin_weight_t(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0, 0, 0, 0));
	SEBottom.bone_weights_and_indicies = skin_weight_t(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0, 0, 0, 0));

	quads.push_back(SWBottom);
	quads.push_back(SWTop);
	quads.push_back(SETop);
	quads.push_back(SEBottom);
}
void MeshBuilder::getInvertedWestSideQuadTangMasterVertex(std::vector<Master_Vertex>& quads, const Vector3& size, const AABB2& texCoords)
{
	Master_Vertex SWBottom = m_stamp;
	Master_Vertex SWTop = m_stamp;
	Master_Vertex NWTop = m_stamp;
	Master_Vertex NWBottom = m_stamp;

	SWBottom.position = Vector3(SWBottom.position.x, SWBottom.position.y, SWBottom.position.z);
	SWTop.position = Vector3(SWBottom.position.x, SWBottom.position.y, SWBottom.position.z + size.z);
	NWTop.position = Vector3(SWBottom.position.x, SWBottom.position.y + size.y, SWBottom.position.z + size.z);
	NWBottom.position = Vector3(SWBottom.position.x, SWBottom.position.y + size.y, SWBottom.position.z);

	Vector3 normal1 = SWBottom.position;
	Vector3 normal2 = SWTop.position;
	Vector3 normal3 = NWTop.position;
	Vector3 normal4 = NWBottom.position;

	normal1.normalize();
	normal2.normalize();
	normal3.normalize();
	normal4.normalize();

	Vector3 leftToRight = Vector3(0.f, SWBottom.position.y, 0.f);
	Vector3 topToBottom = Vector3(0.f, 0.f, SWTop.position.z);
	leftToRight.normalize();
	topToBottom.normalize();

	Vector3 tangent1 = leftToRight;
	Vector3 tangent2 = leftToRight;
	Vector3 tangent3 = leftToRight;
	Vector3 tangent4 = leftToRight;

	Vector3 bitangent1 = topToBottom;//normal1.crossProduct(tangent1);
	Vector3 bitangent2 = topToBottom;//normal2.crossProduct(tangent2);
	Vector3 bitangent3 = topToBottom;//normal3.crossProduct(tangent3);
	Vector3 bitangent4 = topToBottom;//normal4.crossProduct(tangent4);

	SWBottom.tangent = tangent1;
	SWTop.tangent = tangent2;
	NWTop.tangent = tangent3;
	NWBottom.tangent = tangent4;

	SWBottom.bitangent = bitangent1;
	SWTop.bitangent = bitangent2;
	NWTop.bitangent = bitangent3;
	NWBottom.bitangent = bitangent4;

	SWBottom.texCoord[0] = Vector2(texCoords.mins.x, texCoords.maxs.y);
	SWTop.texCoord[0] = texCoords.mins;
	NWTop.texCoord[0] = Vector2(texCoords.maxs.x, texCoords.mins.y);
	NWBottom.texCoord[0] = texCoords.maxs;

	/*

	quads.push_back(Vertex_PCTT(Vector3(bottomLeftCorner.x, bottomLeftCorner.y, bottomLeftCorner.z), tint, texCoords.maxs, tangent1, bitangent1));
	quads.push_back(Vertex_PCTT(Vector3(bottomLeftCorner.x, bottomLeftCorner.y, maxZ), tint, Vector2(texCoords.maxs.x, texCoords.mins.y), tangent2, bitangent2));
	quads.push_back(Vertex_PCTT(Vector3(bottomLeftCorner.x, maxY, maxZ), tint, texCoords.mins, tangent3, bitangent3));
	quads.push_back(Vertex_PCTT(Vector3(bottomLeftCorner.x, maxY, bottomLeftCorner.z), tint, Vector2(texCoords.mins.x, texCoords.maxs.y), tangent4, bitangent4));
	*/

	NWBottom.bone_weights_and_indicies = skin_weight_t(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0, 0, 0, 0));
	NWTop.bone_weights_and_indicies = skin_weight_t(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0, 0, 0, 0));
	SWTop.bone_weights_and_indicies = skin_weight_t(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0, 0, 0, 0));
	SWBottom.bone_weights_and_indicies = skin_weight_t(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0, 0, 0, 0));

	quads.push_back(NWBottom);
	quads.push_back(NWTop);
	quads.push_back(SWTop);
	quads.push_back(SWBottom);
}
void MeshBuilder::getInvertedEastSideQuadTangMasterVertex(std::vector<Master_Vertex>& quads, const Vector3& size, const AABB2& texCoords)
{
	Master_Vertex SEBottom = m_stamp;
	Master_Vertex SETop = m_stamp;
	Master_Vertex NETop = m_stamp;
	Master_Vertex NEBottom = m_stamp;

	SEBottom.position = Vector3(SEBottom.position.x + size.x, SEBottom.position.y, SEBottom.position.z);
	SETop.position = Vector3(SEBottom.position.x, SEBottom.position.y, SEBottom.position.z + size.z);
	NETop.position = Vector3(SEBottom.position.x, SEBottom.position.y + size.y, SEBottom.position.z + size.z);
	NEBottom.position = Vector3(SEBottom.position.x, SEBottom.position.y + size.y, SEBottom.position.z);

	Vector3 normal1 = SEBottom.position;
	Vector3 normal2 = SETop.position;
	Vector3 normal3 = NETop.position;
	Vector3 normal4 = NEBottom.position;

	normal1.normalize();
	normal2.normalize();
	normal3.normalize();
	normal4.normalize();

	Vector3 leftToRight = Vector3(0.f, NEBottom.position.y, 0.f);
	Vector3 topToBottom = Vector3(0.f, 0.f, SETop.position.z);
	leftToRight.normalize();
	topToBottom.normalize();

	Vector3 tangent1 = leftToRight;
	Vector3 tangent2 = leftToRight;
	Vector3 tangent3 = leftToRight;
	Vector3 tangent4 = leftToRight;

	Vector3 bitangent1 = topToBottom;//normal1.crossProduct(tangent1);
	Vector3 bitangent2 = topToBottom;//normal2.crossProduct(tangent2);
	Vector3 bitangent3 = topToBottom;//normal3.crossProduct(tangent3);
	Vector3 bitangent4 = topToBottom;//normal4.crossProduct(tangent4);

	normal1 = leftToRight.crossProduct(topToBottom);
	normal2 = leftToRight.crossProduct(topToBottom);
	normal3 = leftToRight.crossProduct(topToBottom);
	normal4 = leftToRight.crossProduct(topToBottom);

	SEBottom.normal = normal1;
	SETop.normal = normal2;
	NETop.normal = normal3;
	NEBottom.normal = normal4;

	SEBottom.tangent = tangent1;
	SETop.tangent = tangent2;
	NETop.tangent = tangent3;
	NEBottom.tangent = tangent4;

	SEBottom.bitangent = bitangent1;
	SETop.bitangent = bitangent2;
	NETop.bitangent = bitangent3;
	NEBottom.bitangent = bitangent4;

	/*

	quads.push_back(Vertex_PCTT(Vector3(xVal, maxY, bottomLeftCorner.z), tint, texCoords.maxs, tangent1, bitangent1));
	quads.push_back(Vertex_PCTT(Vector3(xVal, maxY, maxZ), tint, Vector2(texCoords.maxs.x, texCoords.mins.y), tangent2, bitangent2));
	quads.push_back(Vertex_PCTT(Vector3(xVal, bottomLeftCorner.y, maxZ), tint, texCoords.mins, tangent3, bitangent3));
	quads.push_back(Vertex_PCTT(Vector3(xVal, bottomLeftCorner.y, bottomLeftCorner.z), tint, Vector2(texCoords.mins.x, texCoords.maxs.y), tangent4, bitangent4));
	*/

	SEBottom.bone_weights_and_indicies = skin_weight_t(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0, 0, 0, 0));
	SETop.bone_weights_and_indicies = skin_weight_t(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0, 0, 0, 0));
	NETop.bone_weights_and_indicies = skin_weight_t(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0, 0, 0, 0));
	NEBottom.bone_weights_and_indicies = skin_weight_t(Vector4(1.f, 0.f, 0.f, 0.f), UnsignedIntVector4(0, 0, 0, 0));

	SEBottom.texCoord[0] = Vector2(texCoords.mins.x, texCoords.maxs.y);
	SETop.texCoord[0] = texCoords.mins;
	NETop.texCoord[0] = Vector2(texCoords.maxs.x, texCoords.mins.y);
	NEBottom.texCoord[0] = texCoords.maxs;

	quads.push_back(SEBottom);
	quads.push_back(SETop);
	quads.push_back(NETop);
	quads.push_back(NEBottom);
}