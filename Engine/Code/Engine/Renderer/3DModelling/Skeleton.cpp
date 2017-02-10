#include "Engine/Renderer/3DModelling/Skeleton.hpp"
#include "Engine/Core/FileBinaryReader.hpp"
#include "Engine/Core/FileBinaryWriter.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Math/Vertices/Vertex_PCT.hpp"

//----------------------------------------------------------------
//----------------------------------------------------------------
//Joint

//joint constructors
Skeleton_Joint::Skeleton_Joint()
	: parent(nullptr)
{
}
Skeleton_Joint::~Skeleton_Joint()
{

}

//joint setters
void Skeleton_Joint::AddChild(Skeleton_Joint* joint, const int& jointIdx)
{
	if (joint == nullptr)
	{
		return;
	}
	m_childrenIndices.push_back(jointIdx);
	m_children.push_back(joint);
}
void Skeleton_Joint::SetName(const std::string& nam)
{
	name = nam;
}
void Skeleton_Joint::SetParent(Skeleton_Joint* pare)
{
	parent = nullptr;
	parent = pare;
}
void Skeleton_Joint::SetParentIndice(const int& parent_ini)
{
	parent_indice = parent_ini;
}
void Skeleton_Joint::SetModelToBone(const Matrix44& mat)
{
	currentMatrices.model_to_bone_matrix = mat;
}
void Skeleton_Joint::SetBoneToModel(const Matrix44& mat)
{
	currentMatrices.bone_to_model_matrix = mat;
}
void Skeleton_Joint::SetLocalBoneToModel(const Matrix44& mat)
{
	m_localBoneToModel = mat;
}
void Skeleton_Joint::SetModelToBoneOriginal(const Matrix44& mat)
{
	originalMatrices.model_to_bone_matrix = mat;
}
void Skeleton_Joint::SetBoneToModelOriginal(const Matrix44& mat)
{
	originalMatrices.bone_to_model_matrix = mat;
}
void Skeleton_Joint::SetOwningSkeletonID(const size_t& skeletonID)
{
	owningSkeletonID = skeletonID;
}
void Skeleton_Joint::SetOriginalJointIdx(const size_t& jointIDX)
{
	originalJointIdx = jointIDX;
}
void Skeleton_Joint::SetMatricesFromWorldPosition(const Matrix44& fullTransform, const bool& setModelToBone)
{
	SetBoneToModel(fullTransform);

	if (setModelToBone)
	{
		Matrix44 iTransform = fullTransform;
		iTransform.MatrixInvert();
		SetModelToBone(iTransform);
	}
}

void Skeleton_Joint::SetOriginalMatrices(const Matrix44& orgMatricesMTB, const Matrix44& orgMatricesBTM)
{
	originalMatrices.model_to_bone_matrix = orgMatricesMTB;
	originalMatrices.bone_to_model_matrix = orgMatricesBTM;
}

void Skeleton_Joint::SetChildrenIndices(const std::vector<int>& indices)
{
	m_childrenIndices = indices;
}
void Skeleton_Joint::SetChildren(const std::vector<Skeleton_Joint*>& children)
{
	m_children = children;
}
void Skeleton_Joint::CopyCurrentToOriginalMatrices()
{
	originalMatrices.bone_to_model_matrix = currentMatrices.bone_to_model_matrix;
	originalMatrices.model_to_bone_matrix = currentMatrices.model_to_bone_matrix;

}
void Skeleton_Joint::ResetToOriginalMatrices()
{
	currentMatrices.model_to_bone_matrix = originalMatrices.model_to_bone_matrix;
	currentMatrices.bone_to_model_matrix = originalMatrices.bone_to_model_matrix;
}

//joint Getters
const std::string Skeleton_Joint::GetName() const
{
	return name;
}
Skeleton_Joint* Skeleton_Joint::GetParent() const
{
	return parent;
}
const std::vector<Skeleton_Joint*> Skeleton_Joint::GetChildren() const
{
	return m_children;
}
const std::vector<int> Skeleton_Joint::GetChildrenIndices() const
{
	return m_childrenIndices;
}
const int Skeleton_Joint::GetParentIndice() const
{
	return parent_indice;
}
const Matrix44 Skeleton_Joint::GetModelToBone() const
{
	return currentMatrices.model_to_bone_matrix;
}
const Matrix44 Skeleton_Joint::GetBoneToModel() const
{
	return currentMatrices.bone_to_model_matrix;
}
const Matrix44 Skeleton_Joint::GetLocalBoneToModel() const
{
	return m_localBoneToModel;
}
const boneAndModelMatrices Skeleton_Joint::GetOriginalMatrices() const
{ 
	return originalMatrices;
};
const Matrix44 Skeleton_Joint::GetWorldPosition() const
{
	//Matrix44 parentMat = GetWorldPositionRecursive(parent);
	return currentMatrices.bone_to_model_matrix;// *parentMat;
}
const Matrix44 Skeleton_Joint::GetWorldPositionRecursive(Skeleton_Joint* cur)
{
	if (cur == nullptr)
	{
		return Matrix44(true);
	}
	Matrix44 parentMat = Skeleton_Joint::GetWorldPositionRecursive(cur->GetParent());
	return parentMat * cur->GetBoneToModel();
}
const size_t Skeleton_Joint::GetOwningSkeletonID() const
{
	return owningSkeletonID;
}
const size_t Skeleton_Joint::GetOriginalJointIDX() const 
{
	return originalJointIdx;
}


//----------------------------------------------------------------
//----------------------------------------------------------------
//Skeleton



Skeleton::~Skeleton()
{
	if (destroyJoints)
	{
		ClearJoints();
	}
}

//---------------------------------------------------------------
//rendering


void Skeleton::Render() const
{
	if (g_Renderer == nullptr)
	{
		return;
	}

	std::vector<Vertex_PCT> lines;
	std::vector<Vertex_PCT> points;

	for (size_t i = 0; i < joints.size(); i++)
	{
		Skeleton_Joint* cur = joints.at(i);
		Skeleton_Joint* par = cur->GetParent();

		Matrix44 curMat = cur->GetWorldPosition(); //cur->GetBoneToModel();
		lines.push_back(Vertex_PCT(curMat.MatrixGetPosition(), m_lineColor));
		if (par != nullptr)
		{
			Matrix44 parMat = par->GetWorldPosition(); //par->GetBoneToModel();
			lines.push_back(Vertex_PCT(parMat.MatrixGetPosition(), m_lineColor));
		}
		else
		{
			lines.push_back(lines.at(lines.size() - 1));
		}
		points.push_back(Vertex_PCT(curMat.MatrixGetPosition(), m_pointColor));
	}

	g_Renderer->setLineSize(5.f);
	g_Renderer->setPointSize(10.f);
	g_Renderer->DrawVertexArray(lines, PRIMITIVE_LINES, nullptr);
	g_Renderer->DrawVertexArray(points, PRIMITIVE_POINTS, nullptr);
}
void Skeleton::ResetJoints()
{
	for (size_t i = 0; i < joints.size(); i++)
	{
		joints.at(i)->ResetToOriginalMatrices();
	}
}

//-------------------------------------------------------------
//Getters
const std::vector<Matrix44> Skeleton::GetBoneSpaceTransforms() const
{
	std::vector<Matrix44> matrices;
	matrices.resize(joints.size());

	for (size_t i = 0; i < joints.size(); i++)
	{
		Skeleton_Joint* joint = joints.at(i);
		if (joint != nullptr)
		{
			Matrix44 boneToModel = joint->GetBoneToModel();
			matrices[i] = joint->GetModelToBone() * boneToModel;
		}
	}

	return matrices;
}
const std::vector<Matrix44> Skeleton::GetJointBoneToModels() const
{
	std::vector<Matrix44> matrices;
	matrices.resize(joints.size());

	for (size_t i = 0; i < joints.size(); i++)
	{
		Skeleton_Joint* joint = joints.at(i);
		if (joint != nullptr)
		{
			matrices[i] = joint->GetBoneToModel();
		}
	}

	return matrices;
}
const std::vector<Matrix44> Skeleton::GetJointModelToBones() const
{
	std::vector<Matrix44> matrices;
	matrices.resize(joints.size());

	for (size_t i = 0; i < joints.size(); i++)
	{
		Skeleton_Joint* joint = joints.at(i);
		if (joint != nullptr)
		{
			matrices[i] = joint->GetModelToBone();
		}
	}

	return matrices;
}
const int Skeleton::GetLastAddedJointIndex() const
{
	return joints.size() -1;
}

const int Skeleton::FindJointIndexByName(const std::string& jointName) const
{
	for (size_t i = 0; i < joints.size(); i++)
	{
		if (joints.at(i)->GetName() == jointName)
		{
			return i;
		}
	}
	return -1;
}
const std::vector<Skeleton_Joint*>* Skeleton::GetJoints() const
{
	return &joints;
}
Skeleton_Joint* Skeleton::GetJointByName(const std::string& jointName) const
{
	for (size_t i = 0; i < joints.size(); i++)
	{
		if (joints.at(i)->GetName() == jointName)
		{
			return joints.at(i);
		}
	}

	return nullptr;
}
Skeleton_Joint* Skeleton::GetLastAddedJoint() const
{
	if (joints.size() <= 0)
	{
		return nullptr;
	}
	return joints.at(joints.size() - 1);
}
Skeleton_Joint* Skeleton::GetJointByIndex(const int& index) const
{
	if (index < 0 || (size_t)index >= joints.size())
	{
		return nullptr;
	}
	return joints.at(index);
}
const size_t Skeleton::GetJointCount() const
{
	return joints.size(); 
}
const Rgba Skeleton::GetPointColor() const
{
	return m_pointColor;
}
const Rgba Skeleton::GetLineColor() const
{
	return m_lineColor;
}
const Matrix44 Skeleton::GetWorldPositionForLocal(const int& jointIdx) const
{
	if (jointIdx < 0 || jointIdx >= (int)joints.size() || joints.at(jointIdx) == nullptr)
	{
		return Matrix44::mat44_identity;
	}

	Skeleton_Joint* cur = joints.at(jointIdx);
	Matrix44 localDispl = cur->GetLocalBoneToModel();
	Matrix44 worldDispl = localDispl;
	if (cur->GetParentIndice() != -1)
	{
		worldDispl = worldDispl * GetWorldPositionForLocal(cur->GetParentIndice());
	}
	return worldDispl;
}


//--------------------------------------------------------------------------------------------
//Setters

void Skeleton::setDestroyJoints(const bool& destroy)
{
	destroyJoints = destroy;
}
void Skeleton::SetTransformBasedOnWorld(const int& joint_idx, const Matrix44& new_model)
{
	if (joint_idx < 0 || joint_idx >= (int)joints.size())
	{
		return;
	}
	Skeleton_Joint* jointSetting = joints.at(joint_idx);
	if (jointSetting != nullptr)
	{
		jointSetting->SetMatricesFromWorldPosition(new_model);

		std::vector<Skeleton_Joint*> toUpdate = jointSetting->GetChildren();
		for (size_t i = 0; i < toUpdate.size(); i++)
		{
			if (toUpdate.at(i) != nullptr)
			{
				Skeleton_Joint* joint = toUpdate.at(i);
				joint;
			}

			toUpdate.erase(toUpdate.begin());
			i--;
		}

	}
}
void Skeleton::AddJoint(const char* str, int parent_joint_idx, const Matrix44& initial_bone_to_model_matrix)
{
	Skeleton_Joint* joint = new Skeleton_Joint();
	Skeleton_Joint* parent = GetJointByIndex(parent_joint_idx);
	joint->SetParent(parent);

	joint->SetOwningSkeletonID(skeleton_ID);
	joint->SetOriginalJointIdx(joints.size());
	joint->SetName(std::string(str));
	joint->SetParentIndice(parent_joint_idx);
	joint->SetMatricesFromWorldPosition(initial_bone_to_model_matrix, false);
	Matrix44 idententTest;

	Vector3 translation = initial_bone_to_model_matrix.MatrixGetPosition();
	translation = translation * -1.f;
	Matrix44 initialModelToBone = initial_bone_to_model_matrix;
	//initialModelToBone.SetTranslate(Vector3(0.f, 0.f, 0.f));
	//initialModelToBone.SetCol(Vector4(0.f, 0.f, 0.f, 1.f), 3);
	initialModelToBone.MatrixInvert();
	//Matrix44 translate = Matrix44(true);
	//translate.SetTranslate(translation);
	//initialModelToBone = translate * initialModelToBone;
	idententTest = initial_bone_to_model_matrix * initialModelToBone;
	joint->SetModelToBone(initialModelToBone);
	//joint->SetMatricesFromLocalPosition(initial_bone_to_model_matrix, true);
	
	
	//joint->SetBoneToModel(initial_bone_to_model_matrix);
	//
	//Matrix44 model_to_bone_matrix = initial_bone_to_model_matrix;
	//model_to_bone_matrix.MatrixInvert();
	//joint->SetModelToBone(model_to_bone_matrix);

	joint->CopyCurrentToOriginalMatrices();

	joints.push_back(joint);
}
void Skeleton::AddSkeleton(Skeleton* skele)
{
	int initialPosition = (int)joints.size();

	std::vector<Skeleton_Joint*> otherJoints = skele->joints;
	for (size_t i = 0; i < otherJoints.size(); i++)
	{
		Skeleton_Joint* joint = otherJoints.at(i);
		if (joint->GetParentIndice() != -1)
		{
			joint->SetParentIndice(joint->GetParentIndice() + initialPosition);
		}
		joints.push_back(joint);
	}
}
void Skeleton::ClearJoints()
{
	for (size_t i = 0; i < joints.size(); i++)
	{
		if (joints[i] != nullptr)
		{
			delete joints[i];
			joints[i] = nullptr;
		}
	}
}
void Skeleton::SetPointColor(const Rgba& pointColor)
{
	m_pointColor = pointColor;
}
void Skeleton::SetLineColor(const Rgba& lineColor)
{
	m_lineColor = lineColor;
}
void Skeleton::SetWorldBoneToModelAndCacheLocal(const Matrix44& mat, const int& index)
{
	//Verify not accessing invalid index
	if (index < 0 || index >= (int)joints.size())
	{
		return;
	}

	//Set World Position, and Calc new Local Position.
	//Calc local position for parent. basically, parent should be only one we have to check that it's parent is not -1.
	Matrix44 copyAble = mat;
	joints.at(index)->SetBoneToModel(mat);
	if (joints.at(index)->GetParentIndice() != -1)
	{
		Matrix44 parentMat = GetWorldPositionForLocal(joints.at(index)->GetParentIndice());
		parentMat.MatrixInvert();
		copyAble = mat * parentMat;
		//Matrix44::MatrixMultiply(&copyAble, &mat, &parentMat);
	}
	joints.at(index)->SetLocalBoneToModel(copyAble);


	//calc local position for children.
	std::vector<int> indexesToUpdate;

	std::vector<int> orgChildren = joints.at(index)->GetChildrenIndices();

	indexesToUpdate.insert(indexesToUpdate.end(), orgChildren.begin(), orgChildren.end());
	for (size_t i = 0; i < indexesToUpdate.size(); i++)
	{
		//Get Indices
		int currentJoint = indexesToUpdate.at(i);

		//Calc new local position
		Matrix44 curLocal = GetWorldPositionForLocal(currentJoint);
		joints.at(currentJoint)->SetBoneToModel(curLocal);

		//Add Children to iterate through.
		std::vector<int> toAdd = joints.at(currentJoint)->GetChildrenIndices();
		indexesToUpdate.insert(indexesToUpdate.end(), toAdd.begin(), toAdd.end());

		indexesToUpdate.erase(indexesToUpdate.begin());
		i--;
	}

}
void Skeleton::SetLocalBoneToModelAndWorldUpdate(const Matrix44& mat, const int& index)
{
	if (index < 0 || index >= (int)joints.size())
	{
		return;
	}

	//Set new Local Bone to Model
	joints.at(index)->SetLocalBoneToModel(mat);
	//Update World Position.
	Matrix44 copyAble = GetWorldPositionForLocal(index);
	joints.at(index)->SetBoneToModel(copyAble);

	//calc local position for children.
	std::vector<int> indexesToUpdate;
	std::vector<int> orgChildren = joints.at(index)->GetChildrenIndices();

	indexesToUpdate.insert(indexesToUpdate.end(), orgChildren.begin(), orgChildren.end());
	for (size_t i = 0; i < indexesToUpdate.size(); i++)
	{
		//Get Indices
		int currentJoint = indexesToUpdate.at(i);

		//Calc new local position
		Matrix44 curLocal = GetWorldPositionForLocal(currentJoint);
		joints.at(currentJoint)->SetBoneToModel(curLocal);

		//Add Children to iterate through.
		std::vector<int> toAdd = joints.at(currentJoint)->GetChildrenIndices();
		indexesToUpdate.insert(indexesToUpdate.end(), toAdd.begin(), toAdd.end());

		indexesToUpdate.erase(indexesToUpdate.begin());
		i--;
	}
}

//--------------------------------------------------------------------------
//writing and saving
void Skeleton::WriteSkeleton(const std::string& fileLoc)
{
	FileBinaryWriter writer;
	writer.open(fileLoc.c_str());
	WriteToStream(writer);
	writer.close();
}
void Skeleton::WriteToStream(IBinaryWriter& writer)
{
	writer.Write<unsigned int>(FILE_VERSION);
	writer.Write<unsigned int>(joints.size());
	WriteJoints(writer);
}
void Skeleton::WriteJoints(IBinaryWriter& writer)
{
	for (size_t jointIdx = 0; jointIdx < joints.size(); jointIdx++)
	{
		Skeleton_Joint* curJoint = joints[jointIdx];

		//name
		writer.WriteString(curJoint->GetName().c_str());
		
		//parent indice
		writer.Write<int>(curJoint->GetParentIndice());
		std::vector<int> children = curJoint->GetChildrenIndices();
		writer.Write<int>(children.size());
		for (size_t childIdx = 0; childIdx < children.size(); childIdx++)
		{
			writer.Write<int>(children.at(childIdx));
		}

		//matrices
		Matrix44 mTB = curJoint->GetModelToBone();
		Matrix44 bTM = curJoint->GetBoneToModel();
		Matrix44 lBTM = curJoint->GetLocalBoneToModel();
		for (size_t modelToBoneIdx = 0; modelToBoneIdx < 16; modelToBoneIdx++)
		{
			writer.Write<float>(mTB.data[modelToBoneIdx]);
		}
		for (size_t boneToModelIdx = 0; boneToModelIdx < 16; boneToModelIdx++)
		{
			writer.Write<float>(bTM.data[boneToModelIdx]);
		}
		for (size_t localBoneToModelIdx = 0; localBoneToModelIdx < 16; localBoneToModelIdx++)
		{
			writer.Write<float>(lBTM.data[localBoneToModelIdx]);
		}

		//VERSION 2 and up
		size_t orgID = curJoint->GetOriginalJointIDX();
		size_t ownID = curJoint->GetOwningSkeletonID();
		boneAndModelMatrices org = curJoint->GetOriginalMatrices();
		writer.Write<size_t>(orgID);
		writer.Write<size_t>(ownID);
		for (size_t OrgBoneToModelIdx = 0; OrgBoneToModelIdx < 16; OrgBoneToModelIdx++)
		{
			writer.Write<float>(org.bone_to_model_matrix.data[OrgBoneToModelIdx]);
		}
		for (size_t OrgModelToBoneIdx = 0; OrgModelToBoneIdx < 16; OrgModelToBoneIdx++)
		{
			writer.Write<float>(org.model_to_bone_matrix.data[OrgModelToBoneIdx]);
		}
	}
}
void Skeleton::ReadSkeleton(const std::string& fileLoc)
{
	ClearJoints();
	FileBinaryReader reader;
	reader.open(fileLoc.c_str());
	ReadFromStream(reader);
	reader.close();
}
void Skeleton::ReadFromStream(IBinaryReader& reader)
{
	unsigned int sizeOfUInt = sizeof(unsigned int);
	unsigned int version = 0;
	unsigned int numOfJoints = 0;
	reader.Read<unsigned int>(version, sizeOfUInt);
	reader.Read<unsigned int>(numOfJoints, sizeOfUInt);
	ReadJoints(reader, numOfJoints, version);
}
void Skeleton::ReadJoints(IBinaryReader& reader, const unsigned int& jointCount, const unsigned int& version)
{
	unsigned int sizeOfInt = sizeof(int);
	unsigned int sizeOfSizeT = sizeof(size_t);
	unsigned int sizeOfFloat = sizeof(float);
	for (size_t jointIdx = 0; jointIdx < jointCount; jointIdx++)
	{
		Skeleton_Joint* curJoint = new Skeleton_Joint();
		//name
		char* str = new char[64];
		for (size_t strIdx = 0; strIdx < 64; strIdx++)
		{
			str[strIdx] = '\0';
		}
		reader.ReadString(str);
		std::string name;
		for (size_t nameIdx = 0; nameIdx < 64; nameIdx++)
		{
			if (str[nameIdx] != '\0')
			{
				name += str[nameIdx];
			}
			else
			{
				break;
			}
		}
		name += '\0';
		curJoint->SetName(name);


		//parentIndice
		int parentIndice = -1;
		reader.Read<int>(parentIndice, sizeOfInt);
		curJoint->SetParentIndice(parentIndice);
		for (size_t parentJointIdx = 0; parentJointIdx < joints.size(); parentJointIdx++)
		{
			if ((int)parentJointIdx == parentIndice)
			{
				curJoint->SetParent(joints.at(parentJointIdx));
			}
		}

		//children
		std::vector<int> childrenIdx;
		int childCount;
		reader.Read<int>(childCount, sizeOfInt);
		for (int childJointIdx = 0; childJointIdx < childCount; childJointIdx++)
		{
			int childIdx;
			reader.Read<int>(childIdx, sizeOfInt);
			childrenIdx.push_back(childIdx);
		}
		curJoint->SetChildrenIndices(childrenIdx);
		//matrices
		Matrix44 mTB;
		Matrix44 bTM;
		Matrix44 lBTM;
		for (size_t modelToBoneIdx = 0; modelToBoneIdx < 16; modelToBoneIdx++)
		{
			float val;
			reader.Read<float>(val, sizeOfFloat);
			mTB.data[modelToBoneIdx] = val;
		}
		for (size_t boneToModelIdx = 0; boneToModelIdx < 16; boneToModelIdx++)
		{
			float val;
			reader.Read<float>(val, sizeOfFloat);
			bTM.data[boneToModelIdx] = val;
		}
		for (size_t localBoneToModelIdx = 0; localBoneToModelIdx < 16; localBoneToModelIdx++)
		{
			float val;
			reader.Read<float>(val, sizeOfFloat);
			lBTM.data[localBoneToModelIdx] = val;
		}
		curJoint->SetBoneToModel(bTM);
		curJoint->SetModelToBone(mTB);
		curJoint->SetLocalBoneToModel(lBTM);
		joints.push_back(curJoint);

		//create Joint

		if (version >= 2)
		{
			//VERSION 2 and up
			size_t orgID;
			size_t ownID;
			reader.Read<size_t>(orgID, sizeOfSizeT);
			reader.Read<size_t>(ownID, sizeOfSizeT);
			curJoint->SetOriginalJointIdx(orgID);
			curJoint->SetOwningSkeletonID(ownID);
			Matrix44 mTBOrg;
			Matrix44 bTMOrg;
			for (size_t modelToBoneOrgIdx = 0; modelToBoneOrgIdx < 16; modelToBoneOrgIdx++)
			{
				float val;
				reader.Read<float>(val, sizeOfFloat);
				mTBOrg.data[modelToBoneOrgIdx] = val;
			}
			for (size_t boneToModelOrgIdx = 0; boneToModelOrgIdx < 16; boneToModelOrgIdx++)
			{
				float val;
				reader.Read<float>(val, sizeOfFloat);
				bTMOrg.data[boneToModelOrgIdx] = val;
			}
			curJoint->SetOriginalMatrices(bTMOrg, mTBOrg);
		}
		else
		{
			curJoint->CopyCurrentToOriginalMatrices();
			curJoint->SetOriginalJointIdx(0);
			curJoint->SetOwningSkeletonID(skeleton_ID);
		}
	}

	//Hook in children.
	for (size_t jointIdx = 0; jointIdx < jointCount; jointIdx++)
	{
		Skeleton_Joint* curJoint = joints.at(jointIdx);
		std::vector<int> children = curJoint->GetChildrenIndices();
		std::vector<Skeleton_Joint*> childrenJ;
		for (size_t j = 0; j < jointCount; j++)
		{
			if (j == jointIdx)
			{
				continue;
			}
			bool isChild = false;
			for (size_t childIdx = 0; childIdx < children.size(); childIdx++)
			{
				if (childIdx == j)
				{
					isChild = true;
					break;
				}
			}
			if (isChild == true)
			{
				childrenJ.push_back(joints.at(j));
			}
		}
		curJoint->SetChildren(childrenJ);
	}
}