#include "Engine/Core/States/StateTransition.hpp"

//-------------------------------------------------------------------------
//Structures

//!---------
//stateCheckStruct
stateCheckStruct::stateCheckStruct(const state_ID& id, const state_ID& idTo, const stateCheckStructType& type)
	: beginningState(id),
	toChangeTo(idTo),
	stateCheck_type(type)
{
}
stateCheckStruct::stateCheckStruct(const stateCheckStruct& other)
	: beginningState(other.beginningState),
	toChangeTo(other.toChangeTo),
	stateCheck_type(other.GetStateStructType())
{
}
stateCheckStruct::~stateCheckStruct()
{
}
const bool stateCheckStruct::IsSameStateStructType(const stateCheckStruct& other) const
{
	if (other.GetStateStructType() == stateCheck_type)
	{
		return true;
	}
	return false;
}
const bool stateCheckStruct::operator>=(const stateCheckStruct& other) const
{
	if (stateCheck_type >= other.GetStateStructType() && beginningState >= other.beginningState && toChangeTo >= other.toChangeTo)
	{
		return true;
	}
	return false;
}
const bool stateCheckStruct::operator>(const stateCheckStruct& other) const
{
	if (stateCheck_type > other.GetStateStructType() && beginningState > other.beginningState && toChangeTo > other.toChangeTo)
	{
		return true;
	}
	return false;
}
const bool stateCheckStruct::operator==(const stateCheckStruct& other) const
{
	if (stateCheck_type == other.GetStateStructType() && beginningState == other.beginningState && toChangeTo == other.toChangeTo)
	{
		return true;
	}
	return false;
}
const bool stateCheckStruct::operator!=(const stateCheckStruct& other) const
{
	if (stateCheck_type != other.GetStateStructType() && beginningState != other.beginningState && toChangeTo != other.toChangeTo)
	{
		return true;
	}
	return false;
}
const bool stateCheckStruct::operator<(const stateCheckStruct& other) const
{
	if (stateCheck_type < other.GetStateStructType() && beginningState < other.beginningState && toChangeTo < other.toChangeTo)
	{
		return true;
	}
	return false;
}
const bool stateCheckStruct::operator<=(const stateCheckStruct& other) const
{
	if (stateCheck_type <= other.GetStateStructType() && beginningState <= other.beginningState && toChangeTo <= other.toChangeTo)
	{
		return true;
	}
	return false;
}

//!-----
//messageEnteredCheck
messageEnteredCheck::messageEnteredCheck(const state_ID& id, const state_ID& idTo, const std::string& messageBeforeChange)
	: stateCheckStruct(id, idTo, STATECHECKTYPE_MESSAGE),
	stateMessageToCauseStateChange(messageBeforeChange)
{
}
messageEnteredCheck::messageEnteredCheck(const messageEnteredCheck& other)
	: stateCheckStruct(other.beginningState, other.toChangeTo, STATECHECKTYPE_MESSAGE),
	stateMessageToCauseStateChange(other.stateMessageToCauseStateChange)
{
}
messageEnteredCheck::~messageEnteredCheck()
{
}
const bool messageEnteredCheck::verifyMessage(const std::string& message) const
{
	if (message == stateMessageToCauseStateChange)
	{
		return true;
	}
	return false;
}
const bool messageEnteredCheck::operator>=(const messageEnteredCheck& other) const
{
	bool messageGreaterThanEqual = stateCheckStruct::operator>=(other);
	if (messageGreaterThanEqual == false)
	{
		return false;
	}
	if (stateMessageToCauseStateChange >= other.stateMessageToCauseStateChange)
	{
		return true;
	}
	return false;
}
const bool messageEnteredCheck::operator==(const messageEnteredCheck& other) const
{
	bool messageSame = stateCheckStruct::operator==(other);
	if (messageSame == false)
	{
		return false;
	}
	if (stateMessageToCauseStateChange == other.stateMessageToCauseStateChange)
	{
		return true;
	}
	return false;
}
const bool messageEnteredCheck::operator!=(const messageEnteredCheck& other) const
{
	bool messageNotSame = stateCheckStruct::operator!=(other);
	if (messageNotSame == false)
	{
		return false;
	}
	if (stateMessageToCauseStateChange != other.stateMessageToCauseStateChange)
	{
		return true;
	}
	return false;
}
const bool messageEnteredCheck::operator<(const messageEnteredCheck& other) const
{
	bool stateIDLessThan = stateCheckStruct::operator<(other);
	if (stateIDLessThan == false)
	{
		return false;
	}
	if (stateMessageToCauseStateChange < other.stateMessageToCauseStateChange)
	{
		return true;
	}
	return false;
}
const bool messageEnteredCheck::operator<=(const messageEnteredCheck& other) const
{
	bool stateIDlessThanEqual = stateCheckStruct::operator<=(other);
	if (stateIDlessThanEqual == false)
	{
		return false;
	}
	if (stateMessageToCauseStateChange <= other.stateMessageToCauseStateChange)
	{
		return true;
	}
	return false;
}

//!-----
//timerCheck
timerCheck::timerCheck(const state_ID& id, const state_ID& idTo, const float& timeSecondsBeforeChange)
	:stateCheckStruct(id, idTo, STATECHECKTYPE_TIMER),
	timeSecondsBeforeStateChange(timeSecondsBeforeChange)
{
}
timerCheck::timerCheck(const timerCheck& other)
	: stateCheckStruct(other.beginningState, other.toChangeTo, STATECHECKTYPE_TIMER),
	timeSecondsBeforeStateChange(other.timeSecondsBeforeStateChange)
{
}
timerCheck::~timerCheck()
{
}
const bool timerCheck::verifyTimeSame(const float& time) const
{
	if (timeSecondsBeforeStateChange == time)
	{
		return true;
	}
	return false;
}
const bool timerCheck::verifyTimeSameOrPast(const float& time) const
{

	if (timeSecondsBeforeStateChange <= time)
	{
		return true;
	}
	return false;
}
const bool timerCheck::operator>=(const timerCheck& other) const
{
	bool stateIds = stateCheckStruct::operator>=(other);
	if (stateIds == false)
	{
		return false;
	}
	if (timeSecondsBeforeStateChange >= other.timeSecondsBeforeStateChange)
	{
		return true;
	}
	return false;
}
const bool timerCheck::operator>(const timerCheck& other) const
{
	bool stateIds = stateCheckStruct::operator>(other);
	if (stateIds == false)
	{
		return false;
	}
	if (timeSecondsBeforeStateChange > other.timeSecondsBeforeStateChange)
	{
		return true;
	}
	return false;
}
const bool timerCheck::operator==(const timerCheck& other) const
{
	bool stateIds = stateCheckStruct::operator==(other);
	if(stateIds == false)
	{
		return false;
	}
	if (timeSecondsBeforeStateChange == other.timeSecondsBeforeStateChange)
	{
		return true;
	}
	return false;
}
const bool timerCheck::operator!=(const timerCheck& other) const
{
	bool stateIds = stateCheckStruct::operator<(other);
	if (stateIds == false)
	{
		return false;
	}
	if (timeSecondsBeforeStateChange != other.timeSecondsBeforeStateChange)
	{
		return true;
	}
	return false;
}
const bool timerCheck::operator<(const timerCheck& other) const
{
	bool stateIds = stateCheckStruct::operator<(other);
	if (stateIds == false)
	{
		return false;
	}
	if (timeSecondsBeforeStateChange < other.timeSecondsBeforeStateChange)
	{
		return true;
	}
	return false;
}
const bool timerCheck::operator<=(const timerCheck& other) const
{
	bool stateIds = stateCheckStruct::operator<=(other);
	if (stateIds == false)
	{
		return false;
	}
	if (timeSecondsBeforeStateChange <= other.timeSecondsBeforeStateChange)
	{
		return true;
	}
	return false;
}


StateTransition::StateTransition()
{

}
StateTransition::~StateTransition()
{

}