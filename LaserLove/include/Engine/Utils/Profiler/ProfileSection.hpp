#pragma once

#ifndef PROFILESECTION_HPP
#define PROFILESECTION_HPP

class ProfileSection
{
private:
	bool m_enabled;
	float m_secondsPassed;
public:
	ProfileSection();
	~ProfileSection();

	void start();
	void end();

	const float GetSeconds() const;
	const bool GetEnabled() const;
};
#endif