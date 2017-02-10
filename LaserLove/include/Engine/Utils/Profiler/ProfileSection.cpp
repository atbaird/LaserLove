#include "Engine/Utils/Profiler/ProfileSection.hpp"


ProfileSection::ProfileSection()
	: m_secondsPassed(0.f),
	m_enabled(false)
{

}
ProfileSection::~ProfileSection()
{

}

void ProfileSection::start()
{
	m_enabled = true;
	m_secondsPassed = 0.f;
}
void ProfileSection::end()
{
	m_enabled = false;
}
const float ProfileSection::GetSeconds() const
{
	return 0.f;
}
const bool ProfileSection::GetEnabled() const
{
	return m_enabled;
}