#pragma once

#ifndef SKELETON_HPP
#define SKELETON_HPP
#include <vector>
#include <string>
#include "Engine/Math/Matrix44.hpp"
#include "Engine/Core/IBinaryReader.hpp"
#include "Engine/Core/IBinaryWriter.hpp"
#include "Engine/Renderer/Rgba.hpp"


struct boneAndModelMatrices
{
	Matrix44 model_to_bone_matrix;
	Matrix44 bone_to_model_matrix;
};

class Skeleton_Joint
{
private:
	std::string name;
	Skeleton_Joint* parent = nullptr;
	int parent_indice;
	size_t originalJointIdx;
	size_t owningSkeletonID;
	Matrix44 m_localBoneToModel;
	boneAndModelMatrices currentMatrices;
	boneAndModelMatrices originalMatrices;
	std::vector<int> m_childrenIndices;
	std::vector<Skeleton_Joint*> m_children;
public:
	Skeleton_Joint();
	~Skeleton_Joint();

	void AddChild(Skeleton_Joint* joint, const int& jointIdx);
	void SetName(const std::string& nam);
	void SetParent(Skeleton_Joint* pare);
	void SetParentIndice(const int& parent_ini);
	void SetModelToBone(const Matrix44& mat);
	void SetBoneToModel(const Matrix44& mat);
	void SetLocalBoneToModel(const Matrix44& mat);
	void SetModelToBoneOriginal(const Matrix44& mat);
	void SetBoneToModelOriginal(const Matrix44& mat);
	void SetOwningSkeletonID(const size_t& skeletonID);
	void SetOriginalJointIdx(const size_t& jointIDX);
	void SetMatricesFromWorldPosition(const Matrix44& fullTransform, const bool& setModelToBone = false);
	void SetOriginalMatrices(const Matrix44& orgMatricesMTB, const Matrix44& orgMatricesBTM);
	void SetChildrenIndices(const std::vector<int>& indices);
	void SetChildren(const std::vector<Skeleton_Joint*>& children);
	void CopyCurrentToOriginalMatrices();
	void ResetToOriginalMatrices();

	const std::string GetName() const;
	Skeleton_Joint* GetParent() const;
	const std::vector<Skeleton_Joint*> GetChildren() const;
	const std::vector<int> GetChildrenIndices() const;
	const int GetParentIndice() const;
	const Matrix44 GetModelToBone() const;
	const Matrix44 GetBoneToModel() const;
	const Matrix44 GetLocalBoneToModel() const;
	const boneAndModelMatrices GetOriginalMatrices() const;
	const size_t GetOwningSkeletonID() const;
	const size_t GetOriginalJointIDX() const;

	const Matrix44 GetWorldPosition() const;

private:
	static const Matrix44 GetWorldPositionRecursive(Skeleton_Joint* cur);

};

class Skeleton
{
protected:
	static const uint32_t FILE_VERSION = 2;
public:
	std::vector<Skeleton_Joint*> joints;
	bool destroyJoints = false;
	size_t skeleton_ID = 0;
	Rgba m_pointColor = Rgba(1.f, 0.f, 1.f, 1.f);
	Rgba m_lineColor = Rgba(1.f, 1.f, 0.f, 1.f);

	//constructors
	~Skeleton();

	//Rendering
	void Render() const;
	void ResetJoints();

	//getters
	const std::vector<Matrix44> GetBoneSpaceTransforms() const;
	const std::vector<Matrix44> GetJointBoneToModels() const;
	const std::vector<Matrix44> GetJointModelToBones() const;
	const int GetLastAddedJointIndex() const;
	const int FindJointIndexByName(const std::string& jointName) const;
	const std::vector<Skeleton_Joint*> GetJoints() const;
	Skeleton_Joint* GetJointByName(const std::string& jointName) const;
	Skeleton_Joint* GetJointByIndex(const int& index) const;
	Skeleton_Joint* GetLastAddedJoint() const;
	const size_t GetJointCount() const;
	const Rgba GetPointColor() const;
	const Rgba GetLineColor() const;
	const Matrix44 GetWorldPositionForLocal(const int& jointIdx) const;

	//setters
	void setDestroyJoints(const bool& destroy);
	void SetTransformBasedOnWorld(const int& joint_idx, const Matrix44& new_model);
	void AddJoint(const char* str, int parent_joint_idx, const Matrix44& initial_bone_to_model_matrix);
	void AddSkeleton(Skeleton* skele);
	void ClearJoints();
	void SetPointColor(const Rgba& pointColor);
	void SetLineColor(const Rgba& lineColor);
	void SetWorldBoneToModelAndCacheLocal(const Matrix44& mat, const int& index);
	void SetLocalBoneToModelAndWorldUpdate(const Matrix44& mat, const int& index);

	//writing and saving
	void WriteSkeleton(const std::string& fileLoc);
	void ReadSkeleton(const std::string& fileLoc);

private:
	void WriteToStream(IBinaryWriter& writer);
	void WriteJoints(IBinaryWriter& writer);
	void ReadFromStream(IBinaryReader& reader);
	void ReadJoints(IBinaryReader& reader, const unsigned int& jointCount, const unsigned int& version);
};
#endif // !SKELETON_HPP
