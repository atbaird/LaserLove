#include "Engine/2D/Particle2D.hpp"

Particle2D Particle2D::s_empty = Particle2D();

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//Constructors
Particle2D::Particle2D(const SpriteResource* res, Rgba color, Vector2 pos, Vector2 vel, Vector2 acc, 
	float max_age_seconds, bool enableColorOverTime, const Rgba& startColor, const Rgba& endColor)
	: resource(res),
	tint(color),
	position(pos),
	velocity(vel),
	acceleration(acc),
	age(0.f),
	max_age(max_age_seconds),
	destroyMe(false),
	ColorTransitionEnabled(enableColorOverTime),
	m_startColor(startColor),
	m_endColor(endColor)
{
	m_colorDiffBetweenEndAndStartColor = m_endColor - m_startColor;
}

Particle2D::~Particle2D()
{

}


//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//Updates and Render
void Particle2D::Update(const float& DeltaSeconds)
{
	age += DeltaSeconds;

	//Movement related
	{
		velocity = velocity + (acceleration * DeltaSeconds);
		position = position + (velocity * DeltaSeconds);
	}

	if (ColorTransitionEnabled)
	{
		Rgba toAdd = m_colorDiffBetweenEndAndStartColor * (age / max_age);
		tint = m_startColor + toAdd;
	}

	if (age >= max_age)
	{
		destroyMe = true;
	}
}

void Particle2D::Render() const
{

}

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//Setters
void Particle2D::SetSpriteResource(const SpriteResource* res)
{
	resource = nullptr;
	resource = res;
}

void Particle2D::SetTint(const Rgba& color)
{
	tint = color;
}

void Particle2D::SetPosition(const Vector2& pos)
{
	position = pos;
}

void Particle2D::SetVelocity(const Vector2& vel)
{
	velocity = vel;
}

void Particle2D::SetAcceleration(const Vector2& acc)
{
	acceleration = acc;
}

void Particle2D::SetMaxAge(const float& max_age_seconds)
{
	max_age = max_age_seconds;
}

void Particle2D::SetColorOverTimeEnabled(const bool& enabled)
{
	ColorTransitionEnabled = enabled;
}
void Particle2D::SetStartColor(const Rgba& color)
{
	m_startColor = color;
	m_colorDiffBetweenEndAndStartColor = m_endColor - m_startColor;
}

void Particle2D::SetEndColor(const Rgba& color)
{
	m_endColor = color;
	m_colorDiffBetweenEndAndStartColor = m_endColor - m_startColor;
}

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
//Getters
const SpriteResource* Particle2D::GetSpriteResource() const
{
	return resource;
}

const Rgba Particle2D::GetTint() const
{
	return tint;
}

const Vector2 Particle2D::GetPosition() const
{
	return position;
}

const Vector2 Particle2D::GetVelocity() const
{
	return velocity;
}

const Vector2 Particle2D::GetAcceleration() const
{
	return acceleration;
}

const float Particle2D::GetMaxAge() const
{
	return max_age;
}

const float Particle2D::GetCurrentAge() const
{
	return age;
}

const bool Particle2D::GetDestroyMe() const
{
	return destroyMe;
}

const bool Particle2D::GetColorOverTimeEnabled() const
{
	return ColorTransitionEnabled;
}

const Rgba Particle2D::GetStartColor() const
{
	return m_startColor;
}

const Rgba Particle2D::GetEndColor() const
{
	return m_endColor;
}