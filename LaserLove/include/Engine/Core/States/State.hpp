#pragma once

#ifndef STATE_HPP
#define STATE_HPP

#include <string>
#include <map>

#ifndef state_ID
#define state_ID int
#endif


class State
{
protected:
	bool m_isStartState;
	float m_timeSecondsSinceStateStart;
	state_ID m_myStateID;
	std::map<state_ID, float> m_statesToTransitionToAfterXSeconds; //state_ID = state to go to, float = input timeSeconds before switch.
	//list of potential states...?
public:
	//constructors
	State();
	virtual ~State();
	virtual void addStateChangeBoolCondition(const state_ID& stateIDToGoTo, const std::string& messageToCauseStateChange);
	virtual void addStateChangeTimerCondition(const state_ID& stateIDToGoTo, const float& timeSecondsBeforeChange);

	virtual void Start();
	virtual void Update(const float& deltaSeconds);
	virtual void Stop();

};
#endif