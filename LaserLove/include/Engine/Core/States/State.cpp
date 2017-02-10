#include "Engine/Core/States/State.hpp"


State::State()
{

}
State::~State()
{

}

void State::addStateChangeBoolCondition(const state_ID& stateIDToGoTo, const std::string& messageToCauseStateChange)
{

}
void State::addStateChangeTimerCondition(const state_ID& stateIDToGoTo, const float& timeSecondsBeforeChange)
{

}

void State::Update(const float& deltaSeconds)
{

}