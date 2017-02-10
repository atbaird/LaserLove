#include "Engine/Renderer/3DModelling/Motion.hpp"
#include "Engine/Core/FileBinaryWriter.hpp"
#include "Engine/Core/FileBinaryReader.hpp"

//--------------------------------------------------------
//Constructors
Motion::Motion()
	: keyframes(nullptr),
	m_wrapMode(MOTION_WRAPMODE_CLAMP)
{

}
Motion::Motion(const Motion& other)
	: keyframes(nullptr)
{
	CopyMotion(other);
}
Motion::Motion(const std::string& motion_name, const float& time_span, const float& framerate, Skeleton* skeleton)
: keyframes(nullptr),
m_wrapMode(MOTION_WRAPMODE_CLAMP)
{
	frame_count = (uint32_t)ceil(framerate * time_span) + 1;
	joint_count = skeleton->GetJointCount();
	total_length_seconds = time_span;
	frame_time = 1.0f / (float)framerate;
	if (frame_count * joint_count > 0)
	{
		keyframes = new Matrix44[frame_count * joint_count];
	}

	name = motion_name;
}

Motion::~Motion()
{
	if (keyframes != nullptr)
	{
		delete[] keyframes;
		keyframes = nullptr;
	}
}


//-------------------------------------------------------------------
//Setters
void Motion::SetWrapMode(Motion_WrapModes mode)
{
	m_wrapMode = mode;
}

const bool Motion::SetFrameOfMotionForJoint(const int& frameIdx, const int& joint_idx, const Matrix44& newFrame)
{
	if (frameIdx < 0 || joint_idx < 0
		|| ((uint32_t)frameIdx >= frame_count) || (joint_idx >= joint_count))
	{
		return false;
	}

	for (size_t i = 0; i < 16; i++)
	{
		keyframes[(joint_idx * frame_count) + frameIdx].data[i] = newFrame.data[i];
	}

	return true;
}



//---------------------------------------------------------------------
//Getters
const uint32_t Motion::GetFrameCount() const
{
	return frame_count; 
}

const float Motion::GetDuration() const
{ 
	return total_length_seconds; 
}

const Matrix44 Motion::GetFrameOfMotionForJoint(const int& frameIdx, const int& joint_idx, bool& exists)
{
	if (frameIdx < 0 || joint_idx < 0
		|| ((uint32_t)frameIdx >= frame_count) || (joint_idx >= joint_count))
	{
		exists = false;
		return Matrix44::mat44_identity;
	}
	exists = true;
	return keyframes[(joint_idx * frame_count) + frameIdx];
}

void Motion::GetFrameIndicesWithBlend(uint32_t& out_frame_idx0,
	uint32_t& out_frame_idx1,
	float& out_blend, 		// normalized distance between frames.
	const float& in_time)
{
	out_frame_idx0 = (uint32_t)floor(in_time / frame_time);
	out_frame_idx1 = out_frame_idx0 + 1;

	if (out_frame_idx0 == (frame_count - 1)) {
		out_frame_idx1 = frame_count - 1;
		out_blend = 0.0f;
	}
	else if (out_frame_idx0 == (frame_count - 2)) {
		float last_frame_time = total_length_seconds - (frame_time * out_frame_idx0);
		out_blend = fmodf(in_time, frame_time) / last_frame_time;
		out_blend = ClampZeroToOne(out_blend);
	}
	else {
		out_blend = fmodf(in_time, frame_time) / frame_time;
	}
}

Matrix44* Motion::GetJointKeyFrames(const uint32_t& joint_idx)
{
	if (joint_idx < 0)
	{
		return keyframes;
	}
	else if (joint_idx >= (uint32_t)joint_count)
	{
		return keyframes + (frame_count * (joint_count - 1));
	}
	return keyframes + (frame_count * joint_idx);
}

Motion_WrapModes Motion::GetWrapMode() const
{ 
	return m_wrapMode;
}




//------------------------------------------------------------------------------
//Operations
const bool Motion::InsertMotionFromMotionAtPoint(const int& frameStart, const int& frameEnd, const int& jointStart, const int& jointEnd,
	const int& insertStart, const Motion& other)
{
	if (frameStart > frameEnd || jointStart > jointEnd
		|| joint_count > (jointEnd - jointStart)
		|| jointEnd < 0 || jointStart < 0 || jointStart >= (int)other.joint_count || jointEnd >= (int)other.joint_count
		|| frameStart < 0 || frameEnd < 0 || frameStart >= (int)other.frame_count || frameEnd >= (int)other.frame_count)
	{
		return false;
	}
	int trueStart = insertStart;
	if (trueStart < 0)
	{
		trueStart = 0;
	}
	else if (trueStart > (int)frame_count)
	{
		trueStart = frame_count;
	}
	int numToAdd = frameEnd - frameStart;
	int newFrameCount = frame_count + numToAdd;

	Matrix44* newFrames = new Matrix44[(joint_count * newFrameCount)];

	for (int jointIdx = 0; jointIdx < (int)joint_count; jointIdx++)
	{
		int frameIdxOrg = 0;
		int frameIdxCpy = frameStart;
		for (int frameIdx = 0; frameIdx < newFrameCount; frameIdx++)
		{
			if ((jointIdx < jointStart) || jointIdx > jointEnd)
			{
				if (frameIdxOrg < (int)frame_count)
				{
					newFrames[(jointIdx * newFrameCount) + frameIdx] = keyframes[(jointIdx * frame_count) + frameIdxOrg];
					frameIdxOrg++;
				}
				else
				{
					newFrames[(jointIdx * newFrameCount) + frameIdx] = Matrix44::mat44_identity;
				}
			}
			else
			{
				if ((frameIdxOrg + (frameIdxCpy - frameStart)) < frameStart || (frameIdxOrg + (frameIdxCpy - frameStart)) > frameEnd)
				{
					newFrames[(jointIdx * newFrameCount) + frameIdx] = keyframes[(jointIdx * frame_count) + frameIdxOrg];
					frameIdxOrg++;
				}
				else
				{
					newFrames[(jointIdx * newFrameCount) + frameIdx] = other.keyframes[(jointIdx * other.frame_count) + frameIdxCpy];
					frameIdxCpy++;
				}
			}
		}
	}

	frame_count = newFrameCount;
	delete keyframes;
	keyframes = newFrames;
	return true;
}

const bool Motion::CopyMotion(const Motion& other)
{
	if(other.keyframes == nullptr
		|| other.frame_count <= 0
		|| other.joint_count <= 0)
	{
		return false;
	}

	if (keyframes != nullptr)
	{
		delete keyframes;
		keyframes = nullptr;
	}
	keyframes = new Matrix44[other.frame_count * other.joint_count];
	frame_count = other.frame_count;
	frame_rate = other.frame_rate;
	frame_time = other.frame_time;
	joint_count = other.joint_count;
	m_wrapMode = other.m_wrapMode;
	name = other.name;
	for (size_t i = 0; i < (frame_count * joint_count); i++)
	{
		keyframes[i] = other.keyframes[i];
	}

	return true;
}

void Motion::ApplyMotionToSkeleton(Skeleton *skeleton, const float& time)
{
	float timeCopy = time;
	float diff;
	switch (m_wrapMode)
	{
	case MOTION_WRAPMODE_CLAMP:
		if (time >= total_length_seconds)
		{
			timeCopy = total_length_seconds;
		}
		else if (time <= 0.f)
		{
			timeCopy = 0.f;
		}
		break;
	case MOTION_WRAPMODE_LOOP:
		diff = floor(time / total_length_seconds);
		if (diff >= 1.f)
		{
			timeCopy = time - (diff * total_length_seconds);
		}
		break;
	case MOTION_WRAPMODE_PINGPONG:
		diff = floor(time / total_length_seconds);
		if (diff >= 1.f)
		{
			timeCopy = time - (diff * total_length_seconds);
			if ((int)diff % 2 == 1)
			{
				timeCopy = total_length_seconds - timeCopy;
			}
		}
		break;
	default:
		break;
	}


	uint32_t frame0;
	uint32_t frame1;
	float blend;
	GetFrameIndicesWithBlend(frame0, frame1, blend, timeCopy);

	uint32_t other_joint_count = skeleton->GetJointCount();
	for (uint32_t joint_idx = 0; joint_idx < other_joint_count; ++joint_idx) {
		Matrix44* joint_keyframes = GetJointKeyFrames(joint_idx);
		Matrix44& mat0 = joint_keyframes[frame0];
		Matrix44& mat1 = joint_keyframes[frame1];

		Matrix44 new_model = Matrix44::MatrixLerp(mat0, mat1, blend);

		// Needs to set bone_to_model
		// (or set your matrix tree's world to this, and set
		// bone to model on Skeleton world's array)
		//skeleton->SetTransformBasedOnParents(joint_idx, new_model);
		skeleton->SetTransformBasedOnWorld(joint_idx, new_model);
	}
}

const bool Motion::RetargetMotionForJoint(const int& startFrameIdx, const int& endFrameIdx, const int& joint_idx, Matrix44& newEndPos)
{
	if (startFrameIdx < 0 || endFrameIdx < 0 || joint_idx < 0
		|| (uint32_t)startFrameIdx >= frame_count || (uint32_t)endFrameIdx >= frame_count || joint_idx >= joint_count
		|| startFrameIdx > endFrameIdx)
	{
		return false;
	}
	Matrix44* joint_keyframes = GetJointKeyFrames(joint_idx);
	Matrix44& start = joint_keyframes[startFrameIdx];
	Matrix44& end = joint_keyframes[endFrameIdx];
	int currentFrameIdx = startFrameIdx + 1;
	float denominator = (float)endFrameIdx - (float)startFrameIdx;
	while (currentFrameIdx <= endFrameIdx)
	{
		Matrix44& currentFrame = joint_keyframes[currentFrameIdx];

		float percentLerp = ((float)currentFrameIdx - (float)startFrameIdx) / denominator;
		currentFrame = Matrix44::MatrixLerp(start, end, percentLerp);
		newEndPos = currentFrame;

		currentFrameIdx++;
	}
	return true;
}

const bool Motion::GuaranteeKeyFramesInitialized(const int& numJointsDefault, const int& numFramesDefault)
{
	if (keyframes == nullptr)
	{
		if (joint_count <= 0)
		{
			if (numJointsDefault <= 0)
			{
				return false;
			}
			joint_count = numJointsDefault;
		}
		if (frame_count <= 0)
		{
			if (numFramesDefault <= 0)
			{
				return false;
			}
			frame_count = numFramesDefault;
		}
		keyframes = new Matrix44[joint_count * frame_count];
	}
	return true;
}

const bool Motion::RemoveNumOfFramesPerJoint(const int& startFrame, const int& endFrame, const int& numOfFrames)
{
	if (startFrame < (int)frame_count || endFrame < (int)frame_count
		|| numOfFrames <= 0 || startFrame >= (int)frame_count || endFrame >= (int)frame_count
		|| keyframes == nullptr)
	{
		return false;
	}
	int toRemove = numOfFrames;
	if ((endFrame - startFrame) < toRemove)
	{
		toRemove = endFrame - startFrame;
	}
	if (toRemove <= 0)
	{
		return false;
	}
	int newFrame_count = frame_count - toRemove;
	Matrix44* newFrames = new Matrix44[newFrame_count * joint_count];

	for (int jointIdx = 0; jointIdx < joint_count; jointIdx++)
	{
		int curFrameIndex = 0;
		for (int frameIdx = 0; frameIdx < (int)frame_count; frameIdx++)
		{
			if (frameIdx < startFrame || frameIdx > endFrame)
			{
				newFrames[(jointIdx * newFrame_count) + curFrameIndex] = keyframes[(jointIdx * frame_count) + frameIdx];
				curFrameIndex++;
			}
		}
	}
	frame_count = newFrame_count;
	delete keyframes;
	keyframes = newFrames;
	return true;

}

const bool Motion::RemoveNumOfJoints(const int& startJoint, const int& endJoint, const int& numOfJoints)
{
	if (startJoint < 0 || endJoint < 0 || numOfJoints <= 0
		|| startJoint >= (int)joint_count || endJoint >= (int)joint_count
		|| numOfJoints >= (int)joint_count
		|| keyframes == nullptr)
	{
		return false;
	}
	int toRemove = numOfJoints;
	if ((endJoint - startJoint) < toRemove)
	{
		toRemove = (endJoint - startJoint);
	}
	if (toRemove <= 0)
	{
		return false;
	}

	int newJoint_count = joint_count - toRemove;
	Matrix44* newFrames = new Matrix44[frame_count * newJoint_count];

	int curJoint = 0;
	for (int jointIdx = 0; jointIdx < joint_count; jointIdx++)
	{
		if (jointIdx < startJoint || jointIdx > endJoint)
		{
			for (size_t frameIdx = 0; frameIdx < frame_count; frameIdx++)
			{
				newFrames[(curJoint * frame_count) + frameIdx] = keyframes[(jointIdx * frame_count) + frameIdx];
			}

			curJoint++;
		}
	}
	joint_count = newJoint_count;
	delete keyframes;
	keyframes = newFrames;

	return true;
}

void Motion::AddNumOfDefaultFramesPerJoint(const int& numOfFrames)
{
	if (numOfFrames <= 0)
	{
		return;
	}
	int newframe_count = frame_count + (numOfFrames * joint_count);
	Matrix44* newFrames = new Matrix44[newframe_count];

	for (int jointIdx = 0; jointIdx < joint_count; jointIdx++)
	{
		for (size_t frameIdx = 0; frameIdx < frame_count; frameIdx++)
		{
			newFrames[(jointIdx * newframe_count) + frameIdx] = keyframes[(jointIdx * frame_count) + frameIdx];
		}
		for (size_t frameIdx = frame_count; frameIdx < (size_t)newframe_count; frameIdx++)
		{
			newFrames[(jointIdx * newframe_count) + frameIdx] = Matrix44::mat44_identity;
		}
	}

	float secondsPerFrame = total_length_seconds / (float)frame_count;
	total_length_seconds = secondsPerFrame * newframe_count;

	delete keyframes;
	keyframes = newFrames;
	frame_count = newframe_count;
}

void Motion::AddNumOfDefaultFramesPerJointBasedOnSeconds(const float& secondsOfFrames)
{
	float secondsPerframe = total_length_seconds / (float)frame_count;
	int frameCount = (int)ceil(secondsOfFrames / secondsPerframe);
	AddNumOfDefaultFramesPerJoint(frameCount);
}

void Motion::AddNumOfJoints(const int& numOfJoints)
{
	if (numOfJoints <= 0)
	{
		return;
	}
	int newJoint_count = joint_count + numOfJoints;
	Matrix44* newFrames = new Matrix44[(newJoint_count * frame_count)];
	for (int jointIdx = 0; jointIdx < newJoint_count; jointIdx++)
	{
		for (size_t frameIdx = 0; frameIdx < frame_count; frameIdx++)
		{
			if (jointIdx < joint_count)
			{
				newFrames[(jointIdx * frame_count) + frameIdx] = keyframes[(jointIdx * frame_count) + frameIdx];
			}
			else
			{
				newFrames[(jointIdx * frame_count) + frameIdx] = Matrix44::mat44_identity;
			}
		}
	}

	joint_count = newJoint_count;
	delete keyframes;
	keyframes = newFrames;

}



//------------------------------------------------------------------------------------
//Writers and Readers

void Motion::WriteMotion(const std::string& fileLoc)
{
	FileBinaryWriter writer;
	writer.open(fileLoc.c_str());
	WriteToStream(writer);
	writer.close();
}

void Motion::ReadMotion(const std::string& fileLoc)
{
	if (keyframes != nullptr)
	{
		delete keyframes;
		keyframes = nullptr;
	}
	FileBinaryReader reader;
	reader.open(fileLoc.c_str());
	ReadFromStream(reader);
	reader.close();
}

void Motion::WriteToStream(IBinaryWriter& writer)
{
	writer.Write<unsigned int>(Version);
	writer.WriteString(name.c_str());
	writer.Write<float>(total_length_seconds);
	writer.Write<float>(frame_rate);
	writer.Write<float>(frame_time);
	writer.Write<uint32_t>(frame_count);
	writer.Write<int>(joint_count);
	writer.Write<Motion_WrapModes>(m_wrapMode);
	WriteFrame(writer);
}
void Motion::WriteFrame(IBinaryWriter& writer)
{
	for (int joint_idx = 0; joint_idx < joint_count; ++joint_idx) {
		Matrix44* joint_keyframes = GetJointKeyFrames(joint_idx);
		for (uint32_t frame_idx = 0; frame_idx < frame_count; frame_idx++)
		{
			Matrix44 keyframe = joint_keyframes[frame_idx];

			for (int i = 0; i < 16; i++)
			{
				writer.Write<float>(keyframe.data[i]);
			}
		}
	}
}

void Motion::ReadFromStream(IBinaryReader& reader)
{
	unsigned int sizeOfUInt = sizeof(unsigned int);
	unsigned int sizeOfFloat = sizeof(float);
	unsigned int sizeOfUint32_t = sizeof(uint32_t);
	unsigned int sizeOfInt = sizeof(int);
	unsigned int sizeOfWrapMode = sizeof(Motion_WrapModes);

	unsigned int version;
	char* str = new char[64];
	for (size_t j = 0; j < 64; j++)
	{
		str[j] = '\0';
	}

	reader.Read<unsigned int>(version, sizeOfUInt);
	reader.ReadString(str);
	name = str;
	delete str;
	reader.Read<float>(total_length_seconds, sizeOfFloat);
	reader.Read<float>(frame_rate, sizeOfFloat);
	reader.Read<float>(frame_time, sizeOfFloat);
	reader.Read<uint32_t>(frame_count, sizeOfUint32_t);
	reader.Read<int>(joint_count, sizeOfInt);
	reader.Read<Motion_WrapModes>(m_wrapMode, sizeOfWrapMode);

	ReadFrames(reader, joint_count, frame_count, version);
}

void Motion::ReadFrames(IBinaryReader& reader, const int& jointCNT, const uint32_t& frameCNT, const unsigned int& version)
{
	keyframes = new Matrix44[jointCNT * frameCNT];
	version;
	unsigned int sizeOfFloat = sizeof(float);
	for (int joint_idx = 0; joint_idx < jointCNT; ++joint_idx) {
		for (uint32_t frame_idx = 0; frame_idx < frameCNT; frame_idx++)
		{
			for (int i = 0; i < 16; i++)
			{
				float temp;
				reader.Read<float>(temp, sizeOfFloat);
				keyframes[(joint_idx * frameCNT) + frame_idx].data[i] = temp;
			}
		}
	}
}



