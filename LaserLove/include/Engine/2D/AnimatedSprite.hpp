#pragma once

#ifndef ANIMATEDSPRITE_HPP
#define ANIMATEDSPRITE_HPP
#include "Engine/2D/Sprite.hpp"
#include "Engine/2D/SpriteAnimationSequence.hpp"

class AnimatedSprite : public Sprite
{
	SpriteAnimationSequence* m_sequence;
	float m_time;
public:
	AnimatedSprite();
	AnimatedSprite(const SpriteResource* sprite, const float& time = 0.f);
	AnimatedSprite(const size_t& spriteResourceID, const float& time = 0.f);
	AnimatedSprite(SpriteAnimationSequence* sequence, const float& time = 0.f);
	virtual ~AnimatedSprite();

	virtual void Update(const float& dt) override;

	virtual const SpriteAnimationSequence* GetSequence() const;
	virtual SpriteAnimationSequence* GetEditableSequence() const;
	virtual const float GetTime() const;
};
#endif