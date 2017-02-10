#include "Engine/Renderer/SpriteAnim.hpp"


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//Constructors

SpriteAnim::SpriteAnim()
: m_spriteSheet(),
m_durationSeconds(1.f),
m_elapsedSeconds(0.f),
m_elapsedSecondsSinceLastFrameSwitch(0.f),
m_startSpriteIndex(0),
m_endSpriteIndex(0),
m_currentSpriteIndex(0),
m_isFinished(false),
m_isPlaying(true),
m_reverse(false),
m_mode(SPRITE_ANIM_MODE_PLAY_TO_END)
{

}
SpriteAnim::SpriteAnim(SpriteSheet* spriteSheet, const float& durationSeconds, const SpriteAnimMode& mode, const int& startSpriteIndex, const int& endSpriteIndex)
: m_spriteSheet(spriteSheet),
m_durationSeconds(durationSeconds),
m_elapsedSeconds(0.f),
m_elapsedSecondsSinceLastFrameSwitch(0.f),
m_startSpriteIndex(startSpriteIndex),
m_endSpriteIndex(endSpriteIndex),
m_currentSpriteIndex(startSpriteIndex),
m_isFinished(false),
m_isPlaying(true),
m_reverse(false),
m_mode(mode)
{
	m_durationSecondsBetweenFrames = m_durationSeconds / (endSpriteIndex - startSpriteIndex);
}



//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//Update and Render

void SpriteAnim::Update(const float& deltaSeconds)
{
	if (m_isPlaying == true && m_isFinished == false)
	{
		m_elapsedSeconds += deltaSeconds;
		m_elapsedSecondsSinceLastFrameSwitch += deltaSeconds;
		if (m_elapsedSecondsSinceLastFrameSwitch >= m_durationSecondsBetweenFrames)
		{
			if (m_reverse == true)
			{
				m_currentSpriteIndex -= 1;
			}
			else
			{
				m_currentSpriteIndex += 1;
			}
			m_elapsedSecondsSinceLastFrameSwitch = 0.f;
		}

		if (m_reverse == false && m_currentSpriteIndex >= m_endSpriteIndex)
		{
			if (m_mode == SPRITE_ANIM_MODE_LOOPING)
			{
				m_elapsedSeconds = 0.f;
				m_currentSpriteIndex = m_startSpriteIndex;
			}
			else
			{
				m_elapsedSeconds = m_durationSeconds;
				m_isFinished = true;
			}
		}
		else if (m_reverse == true && m_currentSpriteIndex < m_startSpriteIndex)
		{
			if (m_mode == SPRITE_ANIM_MODE_LOOPING)
			{
				m_elapsedSeconds = 0.f;
				m_currentSpriteIndex = m_endSpriteIndex;
			}
			else
			{
				m_elapsedSeconds = m_durationSeconds;
				m_isFinished = true;
			}
		}
	}
}

const void SpriteAnim::Render(const AABB2& positionToDraw, const float& m_angularDisplacementDegrees, const Rgba& tint)
{
	g_Renderer->Rotate(m_angularDisplacementDegrees);
	AABB2 spriteCoords = m_spriteSheet->GetTexCoordsForSpriteIndex(m_startSpriteIndex + m_currentSpriteIndex);
	g_Renderer->DrawTexturedAABB(positionToDraw, m_spriteSheet->getSpriteSheetTexture(), AABB2(spriteCoords.mins, spriteCoords.maxs), tint);
}

void SpriteAnim::Pause()
{
	m_isPlaying = false;
}
void SpriteAnim::Resume()
{
	m_isPlaying = true;
}
void SpriteAnim::Reset()
{
	m_isFinished = false;
	m_elapsedSeconds = 0.f;
	if (m_reverse == true)
	{
		m_currentSpriteIndex = m_endSpriteIndex;
	}
	else
	{
		m_currentSpriteIndex = m_startSpriteIndex;
	}
}
void SpriteAnim::Reverse()
{
	m_reverse = !m_reverse;
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//Getters

const AABB2 SpriteAnim::GetCurrentTexCoords() const
{
	return m_spriteSheet->GetTexCoordsForSpriteIndex(m_currentSpriteIndex);
}
const Texture* SpriteAnim::GetTexture() const
{
	return (m_spriteSheet->getSpriteSheetTexture());
}

const bool SpriteAnim::IsFinished() const
{
	return m_isFinished; 
}
const bool SpriteAnim::IsPlaying() const
{
	return m_isPlaying; 
}
const bool SpriteAnim::IsReversed() const
{
	return m_reverse; 
}
const float SpriteAnim::GetDurationSeconds() const
{
	return m_durationSeconds; 
}
const float SpriteAnim::GetSecondsElapsed() const
{
	return m_elapsedSeconds; 
}

const float SpriteAnim::GetSecondsRemaining() const
{
	return m_durationSeconds - m_elapsedSeconds;
}
const float SpriteAnim::GetFractionElapsed() const
{
	return m_elapsedSeconds / m_durationSeconds;
}
const float SpriteAnim::GetFractionRemaining() const
{
	return 1.f - (m_elapsedSeconds / m_durationSeconds);
}

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//Setters

const void SpriteAnim::SetDurationSeconds(const float& seconds)
{
	m_durationSeconds = seconds;
	m_durationSecondsBetweenFrames = m_durationSeconds / (m_endSpriteIndex - m_startSpriteIndex);
}
const float SpriteAnim::SetSecondsElapsed(const float& secondsElapsed)
{
	float previousVersionOfElapsedSeconds = m_elapsedSeconds;

	m_elapsedSeconds = secondsElapsed;

	return previousVersionOfElapsedSeconds;
}// jump to specific time
const float SpriteAnim::SetFractionElapsed(const float& fractionElapsed)
{
	float previousSecondsElapsed = m_elapsedSeconds / m_durationSeconds;

	m_elapsedSeconds = fractionElapsed * m_durationSeconds;

	return previousSecondsElapsed;
	
}//e.g. 0.33f for one-third in