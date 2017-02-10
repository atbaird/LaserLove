#pragma once
#include "Engine/Core/Time.hpp"
#include "Engine/Core/Input.hpp"
#include "Engine/Core/Camera3D.hpp"
#include "Engine/Math/ExtendedMath.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Debug/DebugRenderring.hpp"
#include "Engine/Font/Font.hpp"
#include "Engine/2D/Sprite.hpp"
#include "Engine/2D/SpriteDatabase.hpp"
#include "Game/Snow/SnowManager.hpp"
#include "Game/GameEntity/GameEntity.hpp"
#include "Engine/Core/Audio.hpp"
#include "Game/HUD/HUD.hpp"
#include "Game/RealGameState.hpp"
#include "Engine/2D/ParticleSystemDefinition2D.hpp"
#include "Engine/2D/ParticleEmitterDefintion2D.hpp"
#include "Game/PlayerShipSelectImageHandler.hpp"
#include "Game/ScoreImageUpdater.hpp"

#ifndef THEGAME_HPP
#define THEGAME_HPP
//-----------------------------------------------------
class TheGame;
class Ship;
class Controller;
class BackgroundScroller;

//-----------------------------------------------------
extern TheGame* g_TheGame;


//-----------------------------------------------
struct BoundsJobStruc
{
	BoundsJobStruc(size_t start, size_t end);
	size_t m_start;
	size_t m_end;
};

class TheGame
{
public:
	static void BoundsJob(void*);

	static int WINDOW_PHYSICAL_WIDTH;
	static int WINDOW_PHYSICAL_HEIGHT;

	static size_t s_numEntitiesToPerBoundsJob;

	TheGame();
	~TheGame();
	void Update(const float& deltaSeconds, const bool& focus);
	void Render() const;
	void CameraControls(const float& deltaSeconds);
	void XYZPlaneMovement(const float& deltaSeconds);
	void SetUp3DView() const;
	void SetUp2DView() const;
	void DrawDebugAxisLines() const;
	bool isQuitting();
	void SetAudio(AudioSystem* audio);
protected:
	void SetUpForGamePlayState();
	void SetUpSelectShipState();
	void BoundsTest();
	void BoundsTest(size_t start, size_t end);
	void SpawnEnemyFormation();

	//Variables
	Vector2 m_smileyLoc;

	AABB2 m_boundaries;
	Camera3D m_mainCamera;
	Vector3 cameraVelocity;
	Vector2 m_WindowWidthHeight;
	Vector2 m_zNearFar;
	Rgba m_defaultScreenColor;
	AABB2 m_gameEntityBounds;
	gameState m_state;
	float m_secondsSinceLastSpawnedAnEnemyFormation;
	float m_secondsBeforeCanSpawnEnemyFormation;
	size_t m_killsBeforeDied;
	SoundID m_playerShootSoundID;
	bool m_loadedSound;
	size_t m_selectedShipType;
	bool m_selectedAType;
	bool m_spaceHeldDown;
	bool m_quitNow;

	//ptr Variables
	ShaderProgram* m_defaultShader;
	ShaderProgram* m_testSpritePassthrough;
	ParticleSystem2D* m_testParticleSystem;
	Sprite* m_title;
	Sprite* m_spaceMessage;
	Sprite* m_escMessage;
	Controller* m_playerController;
	BackgroundScroller* m_background;
	HUD* m_hud;
	AudioSystem* m_audio;
	AudioChannelHandle m_playerShotHandle;
	PlayerShipSelectImageHandler* m_shipSelectorImageHandler;
	ScoreImageUpdater* m_scoreHandler;

	//Private Static
	static const IntVector2 s_centralMousePosition;
	static const Vector2 s_mouseYawPitchSensitivity;
	static const char* s_playerShotSoundLoc;
};

#endif //THEGAME_HPP