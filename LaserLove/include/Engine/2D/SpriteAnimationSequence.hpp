#pragma once

#ifndef SPRITEANIMATIONSEQUENCE_HPP
#define SPRITEANIMATIONSEQUENCE_HPP
#include "Engine/2D/SpriteResource.hpp"
#include "Engine/Renderer/3DModelling/Motion_WrapModes.hpp"

struct frame_t
{
	//frame_t
	frame_t(const SpriteResource* resource = nullptr, const float& startTimeAfterLastFrame = 1.f);

	const SpriteResource* sprite;
	float duration;
};

class SpriteAnimationSequence
{
private:
	std::vector<frame_t> frames;
	Motion_WrapModes loop_mode;
	float end_time;
	bool flipped;

public:
	//Constructors
	SpriteAnimationSequence(const Motion_WrapModes& loopMode = MOTION_WRAPMODE_CLAMP, const bool& flipp = false);
	virtual ~SpriteAnimationSequence();

	//Insert Frame
	const bool PushBackFrame(const frame_t&);
	const bool AddFrame(const frame_t&, const int& idx);

	//Getters
	const SpriteResource* GetFrame(const float& time);
	const float UpdateTime(const float& time, const float& deltaSeconds);

};
#endif