#pragma once
#include "Engine/Renderer/SpriteSheet.hpp"
#include "Engine/Renderer/Renderer.hpp"

#ifndef SPRITEANIM_HPP
#define SPRITEANIM_HPP

enum SpriteAnimMode
{
	SPRITE_ANIM_MODE_PLAY_TO_END = 0, //play from time=0 to durationSeconds, then finish
	SPRITE_ANIM_MODE_LOOPING,     //Play from time=0 to end then repeat (never finish)
	NUM_SPRITE_ANIM_MODES
};

class SpriteAnim
{
private:
	//Variables
	SpriteSheet* m_spriteSheet;
	SpriteAnimMode m_mode;
	float m_durationSeconds;
	float m_elapsedSeconds;
	float m_elapsedSecondsSinceLastFrameSwitch;
	float m_durationSecondsBetweenFrames;
	int m_startSpriteIndex;
	int m_endSpriteIndex;
	int m_currentSpriteIndex;
	bool m_isFinished;
	bool m_isPlaying;
	bool m_reverse;
public:
	//Constructor
	SpriteAnim();
	SpriteAnim(SpriteSheet* spriteSheet, const float& durationSeconds, const SpriteAnimMode& mode, const int& startSpriteIndex, 
		const int& endSpriteIndex);

	//Update and Render
	void Update(const float& deltaSeconds);
	const void Render(const AABB2& positionToDraw, const float& m_angularDisplacementDegrees, const Rgba& tint);
	void Pause();
	void Resume();
	void Reset();
	void Reverse();

	//Getters
	const AABB2 GetCurrentTexCoords() const; //based on the current elapsed time
	const Texture* GetTexture() const;
	const bool IsFinished() const;
	const bool IsPlaying() const;
	const bool IsReversed() const;
	const float GetDurationSeconds() const;
	const float GetSecondsElapsed() const;
	const float GetSecondsRemaining() const;
	const float GetFractionElapsed() const;
	const float GetFractionRemaining() const;

	//Setters
	const float SetSecondsElapsed(const float& secondsElapsed); // jump to specific time
	const float SetFractionElapsed(const float& fractionElapsed); //e.g. 0.33f for one-third in
	const void SetDurationSeconds(const float& seconds);
};

#endif