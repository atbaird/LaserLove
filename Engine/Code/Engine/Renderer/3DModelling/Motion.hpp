#pragma once

#ifndef MOTION_HPP
#define MOTION_HPP
#include "Engine/Math/Matrix44.hpp"
#include "Engine/Renderer/3DModelling/Skeleton.hpp"
#include "Engine/Renderer/3DModelling/Motion_WrapModes.hpp"
#include "Engine/Math/ExtendedMath.hpp"
#include "Engine/Core/IBinaryReader.hpp"
#include "Engine/Core/IBinaryWriter.hpp"

//==============================================================
//==============================================================
//FbxAnimStack //* // Number of these, is number of motions you can import.
//FbxAnimLayer //* // Collection of pointers to FbxAnimCurveNodes
//FbxAnimCurveNode
//FbxAnimCurve
//FbxAnimCurveKey


class Motion
{
private:
	const unsigned int Version = 1;
	std::string name;
	uint32_t frame_count;
	float total_length_seconds;
	float frame_rate;
	float frame_time;
	Motion_WrapModes m_wrapMode;

	// 2D array of matrices, stride of sizeof(mat44) * joint_count
	Matrix44* keyframes;
	int joint_count;
public:
	//Constructors
	Motion();
	Motion(const Motion& other);
	Motion(const std::string& motion_name, const float& time_span, const float& framerate, Skeleton* skeleton);
	~Motion();

	//Setters
	void SetWrapMode(Motion_WrapModes mode);
	const bool SetFrameOfMotionForJoint(const int& frameIdx, const int& joint_Idx, const Matrix44& newFrame);

	//Getters
	const uint32_t GetFrameCount() const;
	const float GetDuration() const;
	const Matrix44 GetFrameOfMotionForJoint(const int& frameIdx, const int& joint_idx, bool& exists);
	void GetFrameIndicesWithBlend(uint32_t& out_frame_idx0,
		uint32_t& out_frame_idx1,
		float& out_blend, 		// normalized distance between frames.
		const float& in_time);
	Matrix44* GetJointKeyFrames(const uint32_t& joint_idx);
	Motion_WrapModes GetWrapMode() const;

	//Operations
	const bool InsertMotionFromMotionAtPoint(const int& frameStart, const int& frameEnd, const int& jointStart, 
		const int& jointEnd, const int& insertStart, const Motion& other);
	const bool CopyMotion(const Motion& other);
	void ApplyMotionToSkeleton(Skeleton *skeleton, const float& time);
	const bool RetargetMotionForJoint(const int& startFrameIdx, const int& endFrameIdx, const int& joint_idx, 
		Matrix44& newEndPos);
	const bool GuaranteeKeyFramesInitialized(const int& numJointsDefault = 1, const int& numFramesDefault = 1);
	const bool RemoveNumOfFramesPerJoint(const int& startFrame, const int& endFrame, const int& numOfFrames);
	const bool RemoveNumOfJoints(const int& startJoint, const int& endJoint, const int& numOfJoints);
	void AddNumOfDefaultFramesPerJoint(const int& numOfFrames);
	void AddNumOfDefaultFramesPerJointBasedOnSeconds(const float& secondsOfFramesToAdd);
	void AddNumOfJoints(const int& numOfJoints);

	//Writers and Readers
	void WriteMotion(const std::string& fileLoc);
	void ReadMotion(const std::string& fileLoc);
private:
	void WriteToStream(IBinaryWriter& writer);
	void WriteFrame(IBinaryWriter& writer);
	void ReadFromStream(IBinaryReader& reader);
	void ReadFrames(IBinaryReader& reader, const int& jointCNT, const uint32_t& frameCNT, const unsigned int& version);

};
#endif