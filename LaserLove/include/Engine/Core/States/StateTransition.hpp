#pragma once

#ifndef STATETRANSITION_HPP
#define STATETRANSITION_HPP

#include "Engine/Core/States/State.hpp"

#ifndef state_ID
#define state_ID int
#endif
//----------------------------------
//StateTransitionTypes
enum stateCheckStructType
{
	STATECHECKTYPE_INVALID = -1,
	STATECHECKTYPE_STATECHECKSTRUCT,
	STATECHECKTYPE_MESSAGE,
	STATECHECKTYPE_TIMER,
	TOTAL_NUM_STATECHECKTYPES
};
//std::map<state_ID, std::string> m_statesCanEnterBoolConditions; //state_ID = state to go to, std::string = input string to call state change.

class stateCheckStruct
{
private:
	stateCheckStructType stateCheck_type;
public:
	state_ID beginningState;
	state_ID toChangeTo;
	stateCheckStruct(const state_ID& id = -1, const state_ID& idTo = -1, const stateCheckStructType& type = STATECHECKTYPE_STATECHECKSTRUCT);
	stateCheckStruct(const stateCheckStruct& other);
	virtual ~stateCheckStruct();
	virtual const stateCheckStructType GetStateStructType() const { return stateCheck_type; };
	virtual const bool IsSameStateStructType(const stateCheckStruct& other) const;

	virtual const bool operator>=(const stateCheckStruct& other) const;
	virtual const bool operator>(const stateCheckStruct& other) const;
	virtual const bool operator==(const stateCheckStruct& other) const;
	virtual const bool operator!=(const stateCheckStruct& other) const;
	virtual const bool operator<(const stateCheckStruct& other) const;
	virtual const bool operator<= (const stateCheckStruct& other) const;
};

class messageEnteredCheck : stateCheckStruct
{
public:
	std::string stateMessageToCauseStateChange;
	messageEnteredCheck(const state_ID& id = -1, const state_ID& idTo = -1, const std::string& messageBeforeChange = "");
	messageEnteredCheck(const messageEnteredCheck& other);
	~messageEnteredCheck();
	const bool verifyMessage(const std::string& message) const;
	const bool operator>=(const messageEnteredCheck& other) const;
	const bool operator==(const messageEnteredCheck& other) const;
	const bool operator!=(const messageEnteredCheck& other) const;
	const bool operator<(const messageEnteredCheck& other) const;
	const bool operator<=(const messageEnteredCheck& other) const;
};
class timerCheck : stateCheckStruct
{
public:
	float timeSecondsBeforeStateChange;
	timerCheck(const state_ID& id = -1, const state_ID& idTo = -1, const float& timeSecondsBeforeChange = -1.f);
	timerCheck(const timerCheck& other);
	~timerCheck();
	const bool verifyTimeSame(const float& time) const;
	const bool verifyTimeSameOrPast(const float& time) const;
	const bool operator>=(const timerCheck& other) const;
	const bool operator>(const timerCheck& other) const;
	const bool operator==(const timerCheck& other) const;
	const bool operator!=(const timerCheck& other) const;
	const bool operator<(const timerCheck& other) const;
	const bool operator<=(const timerCheck& other) const;
};


//--------------------------------------------------------
//StateTransition;
class StateTransition
{
private:
	State* FromState;
	State* ToState;
	unsigned char m_transitionOnKey;
public:
	StateTransition();
	virtual ~StateTransition();
};
#endif