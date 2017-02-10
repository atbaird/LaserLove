#include "Engine/2D/SpriteAnimationSequence.hpp"
#include <cmath>


//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//frame_t
frame_t::frame_t(const SpriteResource* resource, const float& startTimeAfterLastFrame)
	: sprite(resource),
	duration(startTimeAfterLastFrame)
{
}


//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//Constructors
SpriteAnimationSequence::SpriteAnimationSequence(const Motion_WrapModes& loopMode, const bool& flipp)
	: loop_mode(loopMode),
	end_time(0.f),
	flipped(flipp)
{
}
SpriteAnimationSequence::~SpriteAnimationSequence()
{
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//Insert Frame
const bool SpriteAnimationSequence::PushBackFrame(const frame_t& frame)
{
	if (frame.sprite == nullptr)
	{
		return false;
	}
	frames.push_back(frame);
	end_time += frame.duration;
	return true;
}
const bool SpriteAnimationSequence::AddFrame(const frame_t& frame, const int& idx)
{
	if (frame.sprite == nullptr
		|| idx < 0 || (size_t)idx >= frames.size())
	{
		return false;
	}
	frames.insert(frames.begin() + idx, 1, frame);
	return true;
}

//--------------------------------------------------------------------------
//--------------------------------------------------------------------------
//Getters
const SpriteResource* SpriteAnimationSequence::GetFrame(const float& time)
{
	//This one is suppose to go through and find the correct frame to use.
	//PingPong is the only case that requires special treatment.
	if (frames.size() <= 0)
	{
		return nullptr;
	}
	float timeHolder = 0.f;
	float stepForwardTest = 0.f;


	switch (loop_mode)
	{
	case MOTION_WRAPMODE_PINGPONG:
		if (flipped == true)
		{
			for (size_t i = 0; i < frames.size(); i++)
			{
				stepForwardTest = frames.at(frames.size() - (i + 1)).duration;
				if (time >= timeHolder && time < (timeHolder + stepForwardTest))
				{
					return frames.at(i).sprite;
				}
				else
				{
					timeHolder += stepForwardTest;
				}
			}
			return frames[frames.size() - 1].sprite;
		}
		else
		{
			for (size_t i = 0; i < frames.size(); i++)
			{
				stepForwardTest = frames.at(i).duration;
				if (time >= timeHolder && time < (timeHolder + stepForwardTest))
				{
					return frames.at(i).sprite;
				}
				else
				{
					timeHolder += stepForwardTest;
				}
			}
			return frames[frames.size() - 1].sprite;
		}
	default:
		for (size_t i = 0; i < frames.size(); i++)
		{
			stepForwardTest = frames.at(i).duration;
			if (time >= timeHolder && time < (timeHolder + stepForwardTest))
			{
				return frames.at(i).sprite;
			}
			else
			{
				timeHolder += stepForwardTest;
			}
		}
		return frames[frames.size() - 1].sprite;
		break;
	}

	return nullptr;
}
const float SpriteAnimationSequence::UpdateTime(const float& time, const float& deltaSeconds)
{
	//This method returns the time suppose to use to get the correct sprite.
	//Allows for clamped animation, looped animation, and pingponging

	float test = time + deltaSeconds;

	const float percentage = test / end_time;
	const int floored = (int)floor(percentage);
	const float remainder = percentage - (float)floored;

	switch (loop_mode)
	{
	case MOTION_WRAPMODE_CLAMP:
		if (test >= end_time)
		{
			return end_time;
		}
		else
		{
			return test;
		}
	case MOTION_WRAPMODE_LOOP:
		if (percentage >= 1.f)
		{
			test = test - (floored * end_time);
		}
		return test;
	case MOTION_WRAPMODE_PINGPONG:
		if (percentage >= 1.f)
		{
			flipped = !flipped;
		}
		test -= floored;

		return test;
	default:
		break;
	}
	return time;
}