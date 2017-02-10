#include "Engine/2D/AnimatedSprite.hpp"

AnimatedSprite::AnimatedSprite()
	: Sprite(),
	m_sequence(nullptr),
	m_time(0.f)
{

}
AnimatedSprite::AnimatedSprite(const SpriteResource* sprite, const float& time)
	: Sprite(sprite),
	m_sequence(nullptr),
	m_time(time)
{

}
AnimatedSprite::AnimatedSprite(const size_t& spriteResourceID, const float& time)
	: Sprite(spriteResourceID),
	m_sequence(nullptr),
	m_time(time)
{

}
AnimatedSprite::AnimatedSprite(SpriteAnimationSequence* sequence, const float& time)
	: Sprite(),
	m_sequence(sequence),
	m_time(time)
{

}
AnimatedSprite::~AnimatedSprite()
{
	Sprite::~Sprite();
}


void AnimatedSprite::Update(const float& dt)
{
	Sprite::Update(dt);
	if (m_sequence == nullptr)
	{
		return;
	}
	m_time = m_sequence->UpdateTime(m_time, dt);
	ChangeSpriteResource(m_sequence->GetFrame(m_time));
}


const SpriteAnimationSequence* AnimatedSprite::GetSequence() const
{
	return m_sequence;
}
SpriteAnimationSequence* AnimatedSprite::GetEditableSequence() const
{
	return m_sequence;
}
const float AnimatedSprite::GetTime() const
{
	return m_time;
}