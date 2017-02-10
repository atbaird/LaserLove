#pragma once

#ifndef STATE_HPP
#define STATE_HPP

#include <string>
#include <map>

#ifndef state_ID
#define state_ID unsigned int
#endif

#include "Engine/EventSystem/Event.hpp"

template <typename ...Args>
class State
{
protected:
	bool m_isStartState;
	bool m_isRunningState;
	bool m_isEndingState;
	bool m_isFinishedState;
	state_ID m_myStateID;
public:
	Event<Args...> start_up_state_trigger;
	Event<Args...> state_trigger;
	Event<Args...> end_state_trigger;
	//constructors
	State()
	{

	}

	virtual ~State()
	{

	}

	//Start, Update, and Stop
	virtual void Start(Args... args);
	virtual void Update(Args... args);
	virtual void Stop(Args... args);

	//Setters
	void SetStateIsStarting(bool start);
	void SetStateIsEnding(bool end);
	void SetStateIsFinished(bool finished);
	void SetStateID(state_ID id);

	//Getters
	bool GetIsStartingState() const;
	bool GetIsRunningState() const;
	bool GetIsEndingState() const;
	bool GetIsFinishedState() const;

};


//Start, Update, and Stop
template <typename ...Args>
void State<Args...>::Start(Args... args)
{
	m_isEndingState = false;
	m_isRunningState = false;
	m_isFinishedState = false;
	m_isStartState = true;
	start_up_state_trigger.Trigger(args...);
	m_isStartState = false;
}

template <typename ...Args>
void State<Args...>::Update(Args... args)
{
	m_isFinishedState = false;
	m_isEndingState = false;
	m_isStartState = false;
	m_isRunningState = true;

	state_trigger.Trigger(args...);
}

template <typename ...Args>
void State<Args...>::Stop(Args... args)
{
	m_isFinishedState = false;
	m_isRunningState = false;
	m_isEndingState = true;
	m_isStartState = false;

	end_state_trigger.Trigger(args...);

	m_isEndingState = false;
}

//Setters
template <typename ...Args>
void State<Args...>::SetStateIsStarting(bool start)
{
	m_isStartState = start;
}

template <typename ...Args>
void State<Args...>::SetStateIsEnding(bool end)
{
	m_isEndingState = end;
}

template <typename ...Args>
void State<Args...>::SetStateIsFinished(bool finished)
{
	m_isFinishedState = finished;
}

template <typename ...Args>
void State<Args...>::SetStateID(state_ID id)
{
	m_myStateID = id;
}

//Getters
template <typename ...Args>
bool State<Args...>::GetIsStartingState() const
{
	return m_isStartState;
}

template <typename ...Args>
bool State<Args...>::GetIsRunningState() const
{
	return m_isRunningState;
}

template <typename ...Args>
bool State<Args...>::GetIsEndingState() const
{
	return m_isEndingState;
}

template <typename ...Args>
bool State<Args...>::GetIsFinishedState() const
{
	return m_isFinishedState;
}

#endif