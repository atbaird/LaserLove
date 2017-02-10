#include "Engine/Core/Clock.hpp"
#include "Engine/Core/Time.hpp"

Clock* Clock::gSystemClock = nullptr;
size_t Clock::s_availableClockID = 0;

Clock::Clock()
	: m_clockID(s_availableClockID),
	m_parent(nullptr)
{
	s_availableClockID++;
	if (this != gSystemClock)
	{
		m_parent = gSystemClock;
		m_parent->RegisterChild(this);
	}
}
Clock::Clock(const double& startTime)
	: m_clockID(s_availableClockID),
	m_parent(nullptr)
{
	s_availableClockID++;
	if (this != gSystemClock)
	{
		m_parent = gSystemClock;
		m_parent->RegisterChild(this);
	}
	current_time = startTime;
}
Clock::Clock(Clock* parent)
	: m_parent(parent),
	m_clockID(s_availableClockID)
{
	s_availableClockID++;
	if (this != parent)
	{
		m_parent = parent;
		m_parent->RegisterChild(this);
	}

}
Clock::~Clock()
{
	if (m_parent != nullptr)
	{
		m_parent->UnregisterChild(this);
	}
	for (size_t i = 0; i < m_children.size(); i++)
	{
		if (m_children.at(i) != nullptr)
		{
			delete m_children[i];
			m_children[i] = nullptr;
		}
	}
	m_children.clear();
}

void Clock::Update(const double& deltaSeconds)
{
	double dt = deltaSeconds;
	if (m_paused)
	{
		dt = 0.f;
	}
	else
	{
		dt *= m_scale;
	}

	previous_time = current_time;
	current_time += dt;

	for(size_t idx = 0; idx < m_children.size(); idx++)
	{
		Clock* child = m_children.at(idx);
		if (child != nullptr)
		{
			child->Update(dt);
		}
	}
}

void Clock::RegisterChild(Clock* clock)
{
	if (clock == m_parent)
	{
		return;
	}

	for (size_t i = 0; i < m_children.size(); i++)
	{
		if (m_children.at(i) == clock)
		{
			return;
		}
	}
	m_children.push_back(clock);
}
void Clock::UnregisterChild(Clock* clock)
{
	for (size_t i = 0; i < m_children.size(); i++)
	{
		if (m_children.at(i) == clock)
		{
			m_children.erase(m_children.begin() + i);
			break;
		}
	}
}

void Clock::TimeInit()
{
	gSystemClock = new Clock(GetCurrentTimeSeconds());
}
void Clock::TimeUpdate()
{
	if (gSystemClock != nullptr)
	{
		double realCurTime = GetCurrentTimeSeconds();
		double dt = realCurTime - gSystemClock->GetPreviousTime();
		gSystemClock->Update(dt);
	}
}

const double Clock::GetCurrent() const
{
	return current_time;
}
const double Clock::GetDelta() const
{
	return current_time - previous_time;
}

const double Clock::GetPreviousTime() const
{
	return previous_time;
}