#pragma once

#ifndef CLOCK_HPP
#define CLOCK_HPP
#include <vector>

class Clock
{
private:
	static size_t s_availableClockID;

	bool m_paused;
	double m_scale;
	std::vector<Clock*> m_children;
	Clock* m_parent;
	size_t m_clockID;
	double current_time;
	double previous_time;

public:
	static Clock* gSystemClock;

	//Constructors
	Clock();
	Clock(const double& startTime);
	Clock(Clock* parent);
	~Clock();

	//Update
	void Update(const double& deltaSeconds);
	void RegisterChild(Clock* clock);
	void UnregisterChild(Clock* clock);

	//Static calls
	static void TimeInit();
	static void TimeUpdate();

	//Getters
	const double GetCurrent() const;
	const double GetDelta() const;
	const double GetPreviousTime() const;
};
#endif