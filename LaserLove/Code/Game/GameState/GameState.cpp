#include "Game/GameState/GameState.hpp"
#include "Engine/Core/Input.hpp"


//--------------------------------------------------------------------------------------
//Variables
GameStates GameSateCommon::s_previousState = State_StartUp;
GameStates GameSateCommon::s_currentState = State_StartUp;
GameStates GameSateCommon::s_stateToChangeTo = State_StartUp;
bool GameSateCommon::s_stateEnding = false;
bool GameSateCommon::s_stateStarting = false;
bool GameSateCommon::s_stateFinished = false;
bool GameSateCommon::s_stateChanging = false;
float GameSateCommon::s_SecondsSinceLastStateChange = 0.f;

//-------------------------------------------------------------------------------------
//Simple State Machine Methods.
void GameSateCommon::StartOfStateUpdate()
{
	s_stateToChangeTo = State_Invalid;
	s_stateChanging = false;
	s_stateEnding = false;
}
void GameSateCommon::EndOfStateUpdate()
{
	s_stateEnding = true;
	s_stateStarting = false;
}
void GameSateCommon::FinishStateUpdate()
{
	s_stateFinished = true;
	s_stateFinished = false;
}

//-------------------------------------------------------------------------------------
//Core methods
void GameSateCommon::Update(const float& deltaSeconds)
{
	if (s_stateChanging == false)
	{
		s_SecondsSinceLastStateChange += deltaSeconds;
	}
}
void GameSateCommon::ChangeStates(const GameStates& state)
{
	if (state != State_Invalid)
	{
		if (state != s_currentState)
		{
			s_previousState = s_currentState;
			s_currentState = state;
		}
		SetStateFinished(false);
		SetStateChanging(false);
		ResetSecondsSinceLastChangedState();
		s_stateToChangeTo = State_Invalid;
	}
}
void GameSateCommon::StateChangeTest(bool& keyHeld, unsigned char keyVal, const GameStates& changeToThisState)
{
	if (g_input == nullptr)
	{
		return;
	}
	if (keyHeld == false && g_input->isKeyDown(keyVal) == true)
	{
		keyHeld = true;
		if (s_stateChanging == false)
		{
			s_stateChanging = true;
			s_stateToChangeTo = changeToThisState;
		}
	}
}

//-----------------------------------------------------------------------------------
//Setters
void GameSateCommon::SetStateEnding(const bool& ending)
{
	s_stateEnding = ending;
}
void GameSateCommon::SetStateStarting(const bool& starting)
{
	s_stateStarting = starting;
}
void GameSateCommon::SetStateFinished(const bool& finished)
{
	s_stateFinished = finished;
}
void GameSateCommon::SetStateChanging(const bool& changing)
{
	s_stateChanging = changing;
}
void GameSateCommon::ResetSecondsSinceLastChangedState()
{
	s_SecondsSinceLastStateChange = 0.f;
}

//-----------------------------------------------------------------------------------
//Getters
const bool GameSateCommon::GetStateEnding()
{
	return s_stateEnding;
}
const GameStates GameSateCommon::GetCurrentState()
{
	return s_currentState;
}
const GameStates GameSateCommon::GetPreviousState()
{
	return s_previousState;
}
const GameStates GameSateCommon::GetStateToChangeTo()
{
	return s_stateToChangeTo;
}
const bool GameSateCommon::GetStateStarting()
{
	return s_stateStarting;
}
const bool GameSateCommon::GetStateFinished()
{
	return s_stateFinished;
}
const bool GameSateCommon::GetStateChanging()
{
	return s_stateChanging;
}