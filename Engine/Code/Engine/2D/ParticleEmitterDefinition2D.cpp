#include "Engine/2D/ParticleEmitterDefintion2D.hpp"
#include "Engine/Math/ExtendedMath.hpp"
#include "Engine/Core/StringUtils.hpp"

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//Constructors
ParticleEmitterDefinition2D::ParticleEmitterDefinition2D(XMLNode node)
	: m_name(""),
	resource(nullptr),
	start_spawn_count(0),
	relativePosition(Vector2::vec2_zeros),
	particle_spawn_rate_per_second(0.f),
	life_time(1.f),
	tint(Rgba::s_White),
	start_velocity_MinsMaxs(AABB2::ZERO_TO_ONE),
	start_acceleration_MinsMaxs(AABB2::ZERO_TO_ONE),
	scale(Vector2::vec2_ones),
	radius(1.f),
	secondsBeforeStopSpawningParticles(-1.f),
	secondsBeforeAutoDestroyThis(-1.f),
	enableColorTransition(false),
	m_startColor(Rgba::s_White),
	m_endColor(Rgba::s_White)
{
	for (int i = 0; i < node.nAttribute(); i++)
	{
		XMLAttribute attr = node.getAttribute(i);
		if (SimpleStrCmpLower("Name", attr.lpszName))
		{
			m_name = std::string(attr.lpszValue);
		}
		else if (SimpleStrCmpLower("StartSpawnCount", attr.lpszName))
		{
			start_spawn_count = (size_t)atoi(attr.lpszValue);
		}
		else if (SimpleStrCmpLower("LocalPosition", attr.lpszName))
		{
			float x = 0.f;
			float y = 0.f;
			std::string val = ReplaceCharInString(attr.lpszValue, ',', ' ');
			std::vector<std::string> parsed = ParseString(val, ' ');
			if (parsed.size() > 0)
			{
				x = (float)atof(parsed.at(0).c_str());
			}
			if (parsed.size() > 1)
			{
				y = (float)atof(parsed.at(1).c_str());
			}
			relativePosition = Vector2(x, y);
		}
		else if (SimpleStrCmpLower("particleSpawnRate", attr.lpszName))
		{
			particle_spawn_rate_per_second = (float)atof(attr.lpszValue);
		}
		else if (SimpleStrCmpLower("lifeTime", attr.lpszName))
		{
			life_time = (float)atof(attr.lpszValue);
		}
		else if (SimpleStrCmpLower("tint", attr.lpszName))
		{
			float red = 0.f;
			float green = 0.f;
			float blue = 0.f;
			float alpha = 1.f;
			std::string val = ReplaceCharInString(attr.lpszValue, ',', ' ');
			std::vector<std::string> parsed = ParseString(val, ' ');
			if (parsed.size() > 0)
			{
				red = (float)atof(parsed.at(0).c_str());
			}
			if (parsed.size() > 1)
			{
				green = (float)atof(parsed.at(1).c_str());
			}
			if (parsed.size() > 2)
			{
				blue = (float)atof(parsed.at(2).c_str());
			}
			if (parsed.size() > 3)
			{
				alpha = (float)atof(parsed.at(3).c_str());
			}

			tint = Rgba(red, green, blue, alpha);
		}
		else if (SimpleStrCmpLower("scale", attr.lpszName))
		{
			float x = 0.f;
			float y = 0.f;
			std::string val = ReplaceCharInString(attr.lpszValue, ',', ' ');
			std::vector<std::string> parsed = ParseString(val, ' ');
			if (parsed.size() > 0)
			{
				x = (float)atof(parsed.at(0).c_str());
			}
			if (parsed.size() > 1)
			{
				y = (float)atof(parsed.at(1).c_str());
			}

			scale = Vector2(x,y);
		}
		else if (SimpleStrCmpLower("radius", attr.lpszName))
		{
			radius = (float)atof(attr.lpszValue);
		}
		else if (SimpleStrCmpLower("secondsBeforeStopSpawningParticles", attr.lpszName))
		{
			secondsBeforeStopSpawningParticles = (float)atof(attr.lpszValue);
		}
		else if (SimpleStrCmpLower("secondsBeforedestroyEmitterRegardlessOfParticles", attr.lpszName))
		{
			secondsBeforeAutoDestroyThis = (float)atof(attr.lpszValue);
		}
		else if (SimpleStrCmpLower("enableColorTransition", attr.lpszName))
		{
			std::string lower = MakeLower(attr.lpszValue);
			if (SimpleStrCmp(lower, "t")
				|| SimpleStrCmp(lower, "true"))
			{
				enableColorTransition = true;
			}
			else
			{
				enableColorTransition = false;
			}
		}
		else if (SimpleStrCmpLower("startColor", attr.lpszName))
		{
			float red = 0.f;
			float green = 0.f;
			float blue = 0.f;
			float alpha = 1.f;
			std::string val = ReplaceCharInString(attr.lpszValue, ',', ' ');
			std::vector<std::string> parsed = ParseString(val, ' ');
			if (parsed.size() > 0)
			{
				red = (float)atof(parsed.at(0).c_str());
			}
			if (parsed.size() > 1)
			{
				green = (float)atof(parsed.at(1).c_str());
			}
			if (parsed.size() > 2)
			{
				blue = (float)atof(parsed.at(2).c_str());
			}
			if (parsed.size() > 3)
			{
				alpha = (float)atof(parsed.at(3).c_str());
			}

			m_startColor = Rgba(red, green, blue, alpha);
		}
		else if (SimpleStrCmpLower("endColor", attr.lpszName))
		{
			float red = 0.f;
			float green = 0.f;
			float blue = 0.f;
			float alpha = 1.f;
			std::string val = ReplaceCharInString(attr.lpszValue, ',', ' ');
			std::vector<std::string> parsed = ParseString(val, ' ');
			if (parsed.size() > 0)
			{
				red = (float)atof(parsed.at(0).c_str());
			}
			if (parsed.size() > 1)
			{
				green = (float)atof(parsed.at(1).c_str());
			}
			if (parsed.size() > 2)
			{
				blue = (float)atof(parsed.at(2).c_str());
			}
			if (parsed.size() > 3)
			{
				alpha = (float)atof(parsed.at(3).c_str());
			}

			m_endColor = Rgba(red, green, blue, alpha);
		}
	}
}

ParticleEmitterDefinition2D::ParticleEmitterDefinition2D(std::string name, const Vector2& relPos, size_t start_spawn_cnt,
	float spawn_rate_per_second,
	float life_time_seconds, const Rgba& color, AABB2 start_velocity, AABB2 start_acceleration,
	const Vector2& scal, float radiu, bool enableColorTrans, const Rgba& startColor, const Rgba& endColor)
	: m_name(name), 
	resource(nullptr),
	start_spawn_count(start_spawn_cnt),
	relativePosition(relPos),
	particle_spawn_rate_per_second(spawn_rate_per_second),
	life_time(life_time_seconds),
	tint(color),
	start_velocity_MinsMaxs(start_velocity),
	start_acceleration_MinsMaxs(start_acceleration),
	scale(scal),
	radius(radiu),
	secondsBeforeStopSpawningParticles(-1.f),
	secondsBeforeAutoDestroyThis(-1.f),
	enableColorTransition(enableColorTrans),
	m_startColor(startColor),
	m_endColor(endColor)
{

}

ParticleEmitterDefinition2D::ParticleEmitterDefinition2D(const SpriteResource* res, std::string name, const Vector2& relPos, size_t start_spawn_cnt,
	float spawn_rate_per_second,
	float life_time_seconds, const Rgba& color, AABB2 start_velocity, AABB2 start_acceleration,
	const Vector2& scal, float radiu, bool enableColorTrans, const Rgba& startColor, const Rgba& endColor)
	: resource(res),
	m_name(name),
	start_spawn_count(start_spawn_cnt),
	relativePosition(relPos),
	particle_spawn_rate_per_second(spawn_rate_per_second),
	life_time(life_time_seconds),
	tint(color),
	start_velocity_MinsMaxs(start_velocity),
	start_acceleration_MinsMaxs(start_acceleration),
	scale(scal),
	radius(radiu),
	secondsBeforeStopSpawningParticles(-1.f),
	secondsBeforeAutoDestroyThis(-1.f),
	enableColorTransition(enableColorTrans),
	m_startColor(startColor),
	m_endColor(endColor)
{

}

ParticleEmitterDefinition2D::~ParticleEmitterDefinition2D()
{

}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//Operations

Particle2D ParticleEmitterDefinition2D::SpawnParticle(const Vector2& emitterCenter)
{
	//Pre-Position, Velocity and Acceleration work
	float randAngle = (float)(rand() % 360);
	Vector2 normDir = Vector2(CosDegrees(randAngle), SinDegrees(randAngle));
	normDir.normalize();
	float randDistPercentage = 1.f / ((float)(rand() % 10000) + 1.f);
	float distance = radius * randDistPercentage;
	Vector2 displacement = normDir * distance;

	float ranVel = 1.f / ((float)(rand() % 10000) + 1.f);
	float ranAcc = 1.f / ((float)(rand() % 10000) + 1.f);
	Vector2 startVelDiff = start_velocity_MinsMaxs.maxs - start_velocity_MinsMaxs.mins;
	Vector2 startAccDiff = start_acceleration_MinsMaxs.maxs - start_acceleration_MinsMaxs.mins;

	//Position, velocity and acceleration
	Vector2 startPos = emitterCenter + displacement;
	Vector2 startVel = start_velocity_MinsMaxs.mins + (startVelDiff * ranVel);
	Vector2 startAcc = start_acceleration_MinsMaxs.mins + (startAccDiff * ranAcc);

	Particle2D particle = Particle2D(resource, tint, startPos, startVel, startAcc, life_time, enableColorTransition,
		m_startColor, m_endColor);
	return particle;
}

ParticleEmitterDefinition2D* ParticleEmitterDefinition2D::Clone()
{
	ParticleEmitterDefinition2D* Clone = new ParticleEmitterDefinition2D(resource, m_name, relativePosition, 
		start_spawn_count, particle_spawn_rate_per_second, life_time, tint, start_velocity_MinsMaxs, 
		start_acceleration_MinsMaxs, scale, radius, enableColorTransition, m_startColor, m_endColor);
	Clone->SetSecondsBeforeAutoDestroyEmitter(secondsBeforeAutoDestroyThis);
	Clone->SetSecondsBeforeStopSpawningParticles(secondsBeforeStopSpawningParticles);
	return Clone;
}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//Setters
void ParticleEmitterDefinition2D::SetSpriteResource(const SpriteResource* res)
{
	resource = nullptr;
	resource = res;
}

void ParticleEmitterDefinition2D::SetStartSpawnCount(size_t start_spawn_cnt)
{
	start_spawn_count = start_spawn_cnt;
}

void ParticleEmitterDefinition2D::SetParticleSpawnRate(float spawn_rate_per_second)
{
	particle_spawn_rate_per_second = spawn_rate_per_second;
}

void ParticleEmitterDefinition2D::SetLifeTime(float life_time_seconds)
{
	life_time = life_time_seconds;
}

void ParticleEmitterDefinition2D::SetTint(const Rgba& color)
{
	tint = color;
}

void ParticleEmitterDefinition2D::SetStartVelocity(const AABB2& start_velocity)
{
	start_velocity_MinsMaxs = start_velocity;
}

void ParticleEmitterDefinition2D::SetStartAcceleration(const AABB2& start_acceleration)
{
	start_acceleration_MinsMaxs = start_acceleration;
}

void ParticleEmitterDefinition2D::SetScale(const Vector2& scal)
{
	scale = scal;
}

void ParticleEmitterDefinition2D::SetSpawnRadius(float radiu)
{
	radius = radiu;
}

void ParticleEmitterDefinition2D::SetSecondsBeforeStopSpawningParticles(float seconds)
{
	secondsBeforeStopSpawningParticles = seconds;
}

void ParticleEmitterDefinition2D::SetSecondsBeforeAutoDestroyEmitter(float seconds)
{
	secondsBeforeAutoDestroyThis = seconds;
}

void ParticleEmitterDefinition2D::SetLocalPosition(const Vector2& pos)
{
	relativePosition = pos;
}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//Getters
const SpriteResource* ParticleEmitterDefinition2D::GetSpriteResource() const
{
	return resource;
}

const size_t ParticleEmitterDefinition2D::GetStartSpawnCount() const
{
	return start_spawn_count;
}

const float ParticleEmitterDefinition2D::GetParticleSpawnRatePerSecond() const
{
	return particle_spawn_rate_per_second;
}

const float ParticleEmitterDefinition2D::GetLifeTime() const
{
	return life_time;
}

const Rgba ParticleEmitterDefinition2D::GetTint() const
{
	return tint;
}

const AABB2 ParticleEmitterDefinition2D::GetStartVelocity() const
{
	return start_velocity_MinsMaxs;
}

const AABB2 ParticleEmitterDefinition2D::GetStartAcceleration() const
{
	return start_acceleration_MinsMaxs;
}

const Vector2 ParticleEmitterDefinition2D::GetScale() const
{
	return scale;
}

const float ParticleEmitterDefinition2D::GetSpawnRadius() const
{
	return radius;
}

const std::string ParticleEmitterDefinition2D::GetName() const
{
	return m_name;
}

const float ParticleEmitterDefinition2D::GetSecondsBeforeStopSpawningParticles() const
{
	return secondsBeforeStopSpawningParticles;
}

const float ParticleEmitterDefinition2D::GetSecondsbeforeAutoDestroyEmitter() const
{
	return secondsBeforeAutoDestroyThis;
}

const Vector2 ParticleEmitterDefinition2D::GetLocalPosition() const
{
	return relativePosition;
}