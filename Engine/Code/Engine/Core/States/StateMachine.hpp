#pragma once

#ifndef STATEMACHINE_HPP
#define STATEMACHINE_HPP
#include "Engine/Core/States/State.hpp"

template <typename ...Args>
class StateMachine
{
private:
protected:
	std::vector<State<Args...>*> states;
	State<Args...>* next_state;
	State<Args...>* current_state;
	State<Args...>* previous_state;
	state_ID prevStateID;
	state_ID currentStateID;
	state_ID nextStateID;
public:
	//Constructors
	StateMachine(unsigned int numOfStatesToStart = 5)
		: previous_state(nullptr),
		current_state(nullptr),
		next_state(nullptr)
	{
		ResizeNumberOfStatesBigger(numOfStatesToStart);
	}

	virtual ~StateMachine()
	{
		next_state = nullptr;
		current_state = nullptr;
		previous_state = nullptr;
		for (size_t i = 0; i < states.size(); i++)
		{
			if (states[i] != nullptr)
			{
				delete states[i];
				states[i] = nullptr;
			}
		}
		states.clear();
	}

	//Updates
	void Update(Args... updateArgs);
	void ResizeNumberOfStatesBigger(unsigned int numOfStates);
	void RegisterState(state_ID id, State<Args...>* state);

	//Setters
	void ChangeStates(state_ID id);
	void SetCurrentStateFinished();

	//Getters
	state_ID GetCurrentStateID() const;
	state_ID GetPreviousStateID() const;
	state_ID GetNextStateID() const;
};


//Updates
template <typename ...Args>
void StateMachine<Args...>::Update(Args... updateArgs)
{
	if (next_state != nullptr)
	{
		if (current_state != nullptr)
		{
			current_state->Stop(updateArgs...);
			if (current_state->GetIsFinishedState() == false)
			{
				return;
			}
		}
		if (current_state != next_state)
		{
			prevStateID = currentStateID;
			currentStateID = nextStateID;

			previous_state = current_state;
			current_state = next_state;
			next_state = nullptr;
			current_state->Start(updateArgs...);
		}
		else
		{
			next_state = nullptr;
		}
	}
	if (current_state != nullptr)
	{
		current_state->Update(updateArgs...);
	}
}

template <typename ...Args>
void StateMachine<Args...>::ResizeNumberOfStatesBigger(unsigned int numOfStates)
{
	size_t curNumStates = states.size();
	if (numOfStates > curNumStates)
	{
		states.resize(numOfStates);
		for (size_t i = curNumStates; i < states.size(); i++)
		{
			states[i] = nullptr;
		}
	}
}

template <typename ...Args>
void StateMachine<Args...>::RegisterState(state_ID id, State<Args...>* state)
{
	if (state == nullptr)
	{
		return;
	}
	if (id >= states.size())
	{
		ResizeNumberOfStatesBigger(id + 1);
	}
	states[id] = state;
}


//Setters
template <typename ...Args>
void StateMachine<Args...>::ChangeStates(state_ID id)
{
	if (id >= states.size())
	{
		return;
	}
	nextStateID = id;
	next_state = states[id];
}

template <typename ...Args>
void StateMachine<Args...>::SetCurrentStateFinished()
{
	if (current_state == nullptr)
	{
		return;
	}
	current_state->SetStateIsFinished(true);
}

//Getters
template <typename ...Args>
state_ID StateMachine<Args...>::GetCurrentStateID() const
{
	return currentStateID;
}

template <typename ...Args>
state_ID StateMachine<Args...>::GetPreviousStateID() const
{
	return prevStateID;
}

template <typename ...Args>
state_ID StateMachine<Args...>::GetNextStateID() const
{
	return nextStateID;
}


#endif // !STATEMACHINE_HPP
