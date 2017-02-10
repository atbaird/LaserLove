#pragma once

#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

//This is the poor man's Game State Machine that I created in SD4.
//Still need to go and create a better one; but this should work well as is.

enum GameStates
{
	State_Invalid = -1,
	State_StartUp,
	State_MainMenu,
	State_PickMap,
	State_Generation,
	State_Playing,
	State_Paused,
	State_ConfirmExit,
	State_Shutdown,
	State_Continue,
	NUMBER_OF_GAMESTATES
};
class GameSateCommon
{
protected:
	//Variables
	static GameStates s_previousState;
	static GameStates s_currentState;
	static GameStates s_stateToChangeTo;
	static bool s_stateEnding;
	static bool s_stateStarting;
	static bool s_stateFinished;
	static bool s_stateChanging;
	static float s_SecondsSinceLastStateChange;

public:
	//Simple State Machine Methods.
	static void StartOfStateUpdate();
	static void EndOfStateUpdate();
	static void FinishStateUpdate();

	//Core methods
	static void Update(const float& deltaSeconds);
	static void ChangeStates(const GameStates& state);
	static void StateChangeTest(bool& keyHeld, unsigned char keyVal, const GameStates& changeToThisState);
	
	//setters
	static void SetStateEnding(const bool& ending);
	static void SetStateStarting(const bool& starting);
	static void SetStateFinished(const bool& finished);
	static void SetStateChanging(const bool& changing);
	static void ResetSecondsSinceLastChangedState();

	//Getters
	static const GameStates GetCurrentState();
	static const GameStates GetPreviousState();
	static const GameStates GetStateToChangeTo();
	static const bool GetStateEnding();
	static const bool GetStateStarting();
	static const bool GetStateFinished();
	static const bool GetStateChanging();
};


#endif