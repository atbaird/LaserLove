#include "Game/TheGame.hpp"
#include "Engine/Core/Performance/Profiler.hpp"
#include "Engine/2D/SpriteRenderer.hpp"
#include "Engine/2D/SpriteDatabase.hpp"
#include "Engine/Renderer/3DModelling/MeshBuilder.hpp"
#include "Game/Ships/Controller/Controller.hpp"
#include "Game/GameListCleaner/GameListCleaner.hpp"
#include "Game/SpriteLoader/SpriteLoader.hpp"
#include "Game/BackgroundScroller/BackgroundScroller.hpp"
#include "Game/SpriteLoader/CommonVariables.hpp"
#include "Game/EnemyGenerator/EnemyConfigurationGenerator.hpp"
#include "Game/EnemyGenerator/EnemyShipGenerator.hpp"
#include "Engine/2D/ParticleSystem2D.hpp"
#include "Game/Bullets/Bullet.hpp"
#include "Engine/Core/JobSystem/JobSystem.hpp"

BoundsJobStruc::BoundsJobStruc(size_t start, size_t end)
	: m_start(start),
	m_end(end)
{

}

void TheGame::BoundsJob(void* data)
{
	if (g_TheGame == nullptr)
	{
		return;
	}
	BoundsJobStruc* jData = (BoundsJobStruc*)data;

	g_TheGame->BoundsTest();

	delete jData;
}

//--------------------------------------------
TheGame* g_TheGame = nullptr;


const IntVector2 TheGame::s_centralMousePosition = IntVector2(800, 700);
const Vector2 TheGame::s_mouseYawPitchSensitivity = Vector2(0.022f, 0.022f);
const char* TheGame::s_playerShotSoundLoc = "Data/SoundEffects/laser_shooting_sfx.wav";
size_t TheGame::s_numEntitiesToPerBoundsJob = 100;

int TheGame::WINDOW_PHYSICAL_WIDTH = 1600;
int TheGame::WINDOW_PHYSICAL_HEIGHT = 900;

//--------------------------------------------
TheGame::TheGame()
	: m_WindowWidthHeight(16.f, 9.f),
	m_mainCamera(Vector3(1.f, -5.f, 1.f), EulerAngles(0.f, 0.f, 0.f), true),
	cameraVelocity(0.f, 0.f, 0.f),
	m_zNearFar(-1.f, 1.f),
	m_defaultScreenColor(0.0f, 0.5f, 0.5f, 1.f),
	m_defaultShader(nullptr),
	m_testSpritePassthrough(nullptr),
	m_playerController(nullptr),
	m_background(nullptr),
	m_state(State_Menu),
	m_title(nullptr),
	m_spaceMessage(nullptr),
	m_escMessage(nullptr),
	m_audio(nullptr),
	m_loadedSound(false),
	m_spaceHeldDown(false),
	m_secondsBeforeCanSpawnEnemyFormation(1.f),
	m_secondsSinceLastSpawnedAnEnemyFormation(0.f),
	m_gameEntityBounds(Vector2(-3.f, -3.f), Vector2(3.f, 3.f)),
	m_killsBeforeDied(0),
	m_hud(nullptr),
	m_testParticleSystem(nullptr),
	m_selectedShipType(0),
	m_selectedAType(false),
	m_shipSelectorImageHandler(nullptr),
	m_scoreHandler(nullptr)
{
	SpriteDatabase::g_SpriteDatabase = new SpriteDatabase();

	m_defaultShader = ShaderProgram::CreateOrGetShaderProgram("Data/Shaders/SpritePassthrough.vert", "Data/Shaders/SpritePassthrough.frag");
	m_testSpritePassthrough = ShaderProgram::CreateOrGetShaderProgram("Data/Shaders/bareBonesPassthrough.vert", "Data/Shaders/bareBonesPassthrough.frag");
	ShaderProgram* collisionShader = ShaderProgram::CreateOrGetShaderProgram("Data/Shaders/SpriteColorPassthrough.vert", "Data/Shaders/SpriteColorPassthrough.frag");
	ShaderProgram* titleShader = ShaderProgram::CreateOrGetShaderProgram("Data/Shaders/SpriteTitleRainbow.vert", "Data/Shaders/SpriteTitleRainbow.frag");
	CommonVariables::SetCollisionShaderProgram(collisionShader);


	ShaderProgram::SetDefaultShaderProgram(m_defaultShader);

	SpriteRenderer::g_SpriteRenderer = new SpriteRenderer(false, 6);
	SpriteRenderer::g_SpriteRenderer->SetScreenResolution(Vector2((float)WINDOW_PHYSICAL_WIDTH, (float)WINDOW_PHYSICAL_HEIGHT));
	SpriteRenderer::g_SpriteRenderer->SetVirtualSize(2);
	SpriteRenderer::g_SpriteRenderer->SetDebugAxisLinesShader(ShaderProgram::CreateOrGetShaderProgram("Data/Shaders/SpriteColorPassthrough.vert", "Data/Shaders/SpriteColorPassthrough.frag"));
	SpriteRenderer::g_SpriteRenderer->SetClearScreenColor(m_defaultScreenColor.GetRGB());
	SpriteRenderer::g_SpriteRenderer->SetFrameBufferShader(ShaderProgram::CreateOrGetShaderProgram("Data/Shaders/Framebuffers/ScreenPassthrough.vert", "Data/Shaders/Framebuffers/ScreenPassthrough.frag"));
	m_quitNow = false;


	SpriteLoader::LoadAllSpritesAsNew();
	BulletConfiguration::LoadAllBulletConfigurations();
	EnemyConfigurationGenerator::LoadAllEnemyConfigurationGenerators();

	m_smileyLoc = Vector2(4.5f, m_WindowWidthHeight.y - 1.f);// , Vector2(6.f, WindowHeight - 2.f));

	m_background = new BackgroundScroller(SpriteResourceList::GetSpriteResourceByName(ResourceNames::s_backgroundTile1));

	m_hud = new HUD(collisionShader);
	ParticleSystemDefinition2D* def = ParticleSystemDefinition2D::FindOrCreateParticleSystemDefinition("testParticleSystem");
	ParticleEmitterDefinition2D* emitterDef = new ParticleEmitterDefinition2D();
	emitterDef->SetLifeTime(2.f);
	emitterDef->SetLocalPosition(Vector2::vec2_zeros);
	emitterDef->SetParticleSpawnRate(0.1f);
	emitterDef->SetSpawnRadius(3.f);
	emitterDef->SetStartSpawnCount(10);
	emitterDef->SetStartAcceleration(AABB2(Vector2(-1.f, -1.f), Vector2::vec2_ones));
	emitterDef->SetStartVelocity(AABB2(Vector2(-1.f, -1.f), Vector2::vec2_ones));
	emitterDef->SetTint(Rgba(0.f, 1.f, 1.f, 1.f));
	emitterDef->SetScale(Vector2(0.1f, 0.1f));
	def->AddParticleEmitterDefinitionName(emitterDef);
	m_testParticleSystem = ParticleSystemDefinition2D::CreateParticleSystem("testParticleSystem", CommonVariables::s_textLayer, Vector2::vec2_zeros);

	m_title = new Sprite(SpriteResourceList::GetSpriteResourceByName(ResourceNames::s_MenuTitle));
	m_title->SetPosition(Vector2(0.f, 0.8f));
	m_title->CopyMaterialFromResourceAndSetShaderProgram(titleShader);
	m_title->ChangeLayers(CommonVariables::s_textLayer);
	m_spaceMessage = new Sprite(SpriteResourceList::GetSpriteResourceByName(ResourceNames::s_MenuPressSpace));
	m_spaceMessage->SetPosition(Vector2(0.f, 0.5f));
	m_spaceMessage->ChangeLayers(CommonVariables::s_textLayer);
	m_escMessage = new Sprite(SpriteResourceList::GetSpriteResourceByName(ResourceNames::s_MenuPressEsc));
	m_escMessage->SetPosition(Vector2(0.f, 0.2f));
	m_escMessage->ChangeLayers(CommonVariables::s_textLayer);



	m_title->SetEnabled(true);
	m_spaceMessage->SetEnabled(true);
	m_escMessage->SetEnabled(true);
	m_state = State_Menu;
	const SpriteResource* outlineResource = SpriteResourceList::GetSpriteResourceByName(ResourceNames::s_whiteOutline);
	m_shipSelectorImageHandler = new PlayerShipSelectImageHandler(outlineResource);
	m_scoreHandler = new ScoreImageUpdater();
}
TheGame::~TheGame()
{
	if (m_scoreHandler != nullptr)
	{
		delete m_scoreHandler;
		m_scoreHandler = nullptr;
	}
	if (m_shipSelectorImageHandler != nullptr)
	{
		delete m_shipSelectorImageHandler;
		m_shipSelectorImageHandler = nullptr;
	}
	if (m_title != nullptr)
	{
		delete m_title;
		m_title = nullptr;
	}
	if (m_spaceMessage != nullptr)
	{
		delete m_spaceMessage;
		m_spaceMessage = nullptr;
	}
	if (m_escMessage != nullptr)
	{
		delete m_escMessage;
		m_spaceMessage = nullptr;
	}
	if (m_background != nullptr)
	{
		delete m_background;
		m_background = nullptr;
	}
	if(m_hud != nullptr)
	{
		delete m_hud;
		m_hud = nullptr;
	}
	if (m_testParticleSystem != nullptr)
	{
		ParticleSystem2D::ParticleSystemDestroy(m_testParticleSystem);
		m_testParticleSystem = nullptr;
	}
	if (Ship::s_playerShip != nullptr)
	{
		delete Ship::s_playerShip;
		Ship::s_playerShip = nullptr;
	}
	if (SpriteRenderer::g_SpriteRenderer != nullptr)
	{
		delete SpriteRenderer::g_SpriteRenderer;
		SpriteRenderer::g_SpriteRenderer = nullptr;
	}
	if (SpriteDatabase::g_SpriteDatabase != nullptr)
	{
		delete SpriteDatabase::g_SpriteDatabase;
		SpriteDatabase::g_SpriteDatabase = nullptr;
	}
	ParticleSystemDefinition2D::ClearAllParticleSystemDefinitions();
	EnemyConfigurationGenerator::ClearAllEnemyShipsAndEnemyConfigurationGenerators();
	BulletConfiguration::ClearAllBulletConfigurations();
	Sprite::DeleteAllSprites();
	if (ControllerGenerator::GetControllerGenerators() != nullptr)
	{
		ControllerGenerator::DeleteControllerGenerators();
	}
	ClearAllControllers();
	GameListCleaner::CleanAllEntities();
	GameListCleaner::CleanAllLibraries();
	SpriteResourceList::ClearAll();
	GameListCleaner::ClearCleanerFuncs();
}


void TheGame::CameraControls(const float& deltaSeconds)
{
	//Looking
	IntVector2 mousePosition;
	g_input->GetFirstCursorPosition(mousePosition);
	IntVector2 cursorDelta = mousePosition - s_centralMousePosition;
	g_input->setCursorPosition(s_centralMousePosition);
	g_input->setCursorShow(false);

	float camRoll = 0.f;
	float camYaw = m_mainCamera.m_orientation.m_zYaw - ((float)cursorDelta.x * s_mouseYawPitchSensitivity.x); //180.f + degreesIs
	float camPitch = m_mainCamera.m_orientation.m_yPitch + ((float)cursorDelta.y * s_mouseYawPitchSensitivity.y);

	m_mainCamera.m_orientation = EulerAngles(camRoll, camPitch, camYaw);
	m_mainCamera.FixAndClampAngles();

	//Movement
	XYZPlaneMovement(deltaSeconds);
}
void TheGame::XYZPlaneMovement(const float& deltaSeconds)
{
	float movementMultiplier = deltaSeconds;
	if (g_input->isKeyDown(VK_SHIFT) == true)
	{
		movementMultiplier *= 2.f;
	}
	cameraVelocity = Vector3(0.f, 0.f, 0.f);

	//forward and back
	if (g_input->isKeyDown('W') == true && g_input->isKeyDown('S') == false)
	{
		Vector3 forwardXY = m_mainCamera.GetForwardXY() * movementMultiplier;
		cameraVelocity += forwardXY;
	}
	else if (g_input->isKeyDown('S') == true && g_input->isKeyDown('W') == false)
	{
		Vector3 forwardXY = m_mainCamera.GetForwardXY() * movementMultiplier;
		cameraVelocity -= forwardXY;
	}

	//left and right
	if (g_input->isKeyDown('A') == true && g_input->isKeyDown('D') == false)
	{
		Vector3 leftXY = (m_mainCamera.GetLeftXY()) * movementMultiplier;
		cameraVelocity += leftXY;
	}
	else if (g_input->isKeyDown('D') == true && g_input->isKeyDown('A') == false)
	{
		Vector3 leftXY = (m_mainCamera.GetLeftXY()) * movementMultiplier;
		cameraVelocity -= leftXY;
	}

	//up and down
	if (g_input->isKeyDown('Z') == true && g_input->isKeyDown(VK_SPACE) == false)
	{
		cameraVelocity -= Vector3(0.f, 0.f, movementMultiplier);
	}
	else if (g_input->isKeyDown(VK_SPACE) == true && g_input->isKeyDown('Z') == false)
	{
		cameraVelocity += Vector3(0.f, 0.f, movementMultiplier);
	}
	m_mainCamera.m_position = m_mainCamera.m_position + cameraVelocity;
}
void TheGame::Update(const float& deltaSeconds, const bool& focus)
{
	Profiler::Push("Game");
	if (m_loadedSound == false && m_audio != nullptr)
	{
		m_loadedSound = true;
		m_playerShootSoundID = m_audio->CreateOrGetSound(s_playerShotSoundLoc);
	}
	if (focus)
	{
		if (g_input->isKeyDown(VK_ESCAPE) == true)
		{
			m_quitNow = true;
		}
		if (m_state == State_Menu)
		{
			if (g_input->isKeyDown(VK_SPACE) == true && m_spaceHeldDown == false)
			{
				//SetUpForGamePlayState();
				SetUpSelectShipState();
				m_spaceHeldDown = true;
			}
		}
		else if (m_state == State_SelectShipType)
		{
			if (g_input->isKeyDown(VK_SPACE) == true && m_spaceHeldDown == false && m_selectedAType == true)
			{
				SetUpForGamePlayState();
				m_spaceHeldDown = true;
			}

			if (m_selectedAType == true)
			{
				m_spaceMessage->SetEnabled(true);
			}
			for (size_t i = 0; i < 10 && i < EnemyShipGenerator::NumberOfPlayerShipTypes(); i++)
			{
				if (g_input->isKeyDown((char)('0' + i)) == true)
				{
					m_selectedAType = true;
					m_selectedShipType = i;
				}
			}
			if (m_shipSelectorImageHandler != nullptr)
			{
				m_shipSelectorImageHandler->Update(m_selectedAType, m_selectedShipType);
			}
		}
		else if (m_state == State_Game)
		{
			if ((Ship::s_playerShip == nullptr || Ship::s_playerShip->GetIsAlive() == false))
			{
				m_state = State_Lose;
				m_escMessage->SetEnabled(true);
				m_spaceMessage->SetEnabled(true);
			}
			else
			{
				m_spaceMessage->SetEnabled(false);
				m_escMessage->SetEnabled(false);
			}
			m_secondsSinceLastSpawnedAnEnemyFormation += deltaSeconds;
			m_scoreHandler->Update(Ship::s_playerPoints);
			if (m_secondsSinceLastSpawnedAnEnemyFormation >= m_secondsBeforeCanSpawnEnemyFormation)
			{
				m_secondsSinceLastSpawnedAnEnemyFormation = 0.f;
				SpawnEnemyFormation();
			}
		}
		else if (m_state == State_Lose)
		{
			if (g_input->isKeyDown(VK_SPACE) == true)
			{
				//SetUpForGamePlayState();
				SetUpSelectShipState();
			}
		}
		else if (m_state == State_Win)
		{

		}
		// Some simple example OpenGL code; remove this and move all rendering inside theApp and child classes
		//sleep(100); //10 frames a second
		//sleep(50); //20 frames a second
		Profiler::Push("Background_Update");
		//m_background->RegisterJobs(deltaSeconds);
		m_background->Update(deltaSeconds);
		Profiler::Pop();
		//CameraControls(deltaSeconds);
		//m_snowManager->Update(deltaSeconds);
		if (m_hud != nullptr)
		{
			m_hud->Update(deltaSeconds, m_state);
		}
		Sprite::UpdateAll(deltaSeconds);

		if (m_audio != nullptr && Ship::s_playerShip != nullptr && Ship::s_playerShip->GetJustShotABullet())
		{
			m_playerShotHandle->stop();
			m_playerShotHandle = m_audio->PlaySound(m_playerShootSoundID);
		}

		if (Ship::s_playerShip != nullptr && Ship::s_playerShip->GetIsAlive() == false)
		{
			Ship::s_playerShip = nullptr;
			m_playerController = nullptr;
		}

		PROFILE_LOG_SECTION(3);
		for (size_t i = 0;GameEntity::s_gameEntities != nullptr && i < GameEntity::s_gameEntities->size(); i = i + s_numEntitiesToPerBoundsJob)
		{
			size_t end = i + s_numEntitiesToPerBoundsJob;
			if (end >= GameEntity::s_gameEntities->size())
			{
				end = GameEntity::s_gameEntities->size();
			}
			BoundsJobStruc* data = new BoundsJobStruc(i, end);
			JobSystem::CreateAndAddJob(0, BoundsJob, data);
		}
		//BoundsTest();
		PROFILE_LOG_SECTION(3);
		GameEntity::FinalizationPassAll();
	}
	SpriteRenderer::g_SpriteRenderer->Update(deltaSeconds);
	Profiler::Pop();

	if (g_input->isKeyDown(VK_SPACE) == false)
	{
		m_spaceHeldDown = false;
	}
}

void TheGame::DrawDebugAxisLines() const
{
	g_Renderer->DepthTestingTrueFalse(true);
	g_Renderer->DrawGridLinesNEU(2.f, 2.f, 2.f, 3.f);
	g_Renderer->DepthTestingTrueFalse(false);
	g_Renderer->DrawGridLinesNEU(2.f, 2.f, 2.f, 1.f);
}
void TheGame::SetUp3DView() const
{
	const float aspect = (16.f / 9.f);
	const float fovDegreesVertical = 70.f;
	const float zNear = 0.1f;
	const float zFar = 1000.f;

	g_Renderer->SetPerspective(fovDegreesVertical, aspect, zNear, zFar);
}
void TheGame::SetUp2DView() const
{
	g_Renderer->SetOrtho(Vector2::vec2_zeros, m_WindowWidthHeight, m_zNearFar.x, m_zNearFar.y);
}
void TheGame::Render() const
{
	g_Renderer->clearScreen(Rgba::s_Black, true);
	SetUp2DView();


	SpriteRenderer::g_SpriteRenderer->RenderFrame(); //Moving this below "testMesh->DrawWith(testMat);" covers up the white box for some reason, and nothing gets rendered.
}


bool TheGame::isQuitting()
{
	return m_quitNow;
}
void TheGame::SetAudio(AudioSystem* audio)
{
	m_audio = nullptr;
	m_audio = audio;
}
void TheGame::SetUpForGamePlayState()
{
	m_state = State_Game;
	m_title->SetEnabled(false);
	m_spaceMessage->SetEnabled(false);
	m_escMessage->SetEnabled(false);
	m_scoreHandler->ToggleVisible(true);
	if (m_shipSelectorImageHandler != nullptr)
	{
		m_shipSelectorImageHandler->SetVisible(false);
	}
	if (Ship::s_playerShip == nullptr)
	{
		ClearAllGameEntities();
		Ship::s_playerShip = EnemyShipGenerator::SpawnPlayerShipCenterNoControllerByIdx(m_selectedShipType);
		//Ship::s_playerShip = EnemyShipGenerator::SpawnRandomPlayerShipCenterNoController();
		Ship::s_playerShip->ChangeLayers(CommonVariables::s_playerLayer);
		Ship::s_playerShip->SetCanMove(true);
		Ship::s_playerShip->SetTeamID(GameEntity::s_playerTeamID);
		if (m_playerController == nullptr)
		{
			m_playerController = ControllerGenerator::CreateController(CONTROLLER_PLAYER_TYPE, Ship::s_playerShip);
		}
		else
		{
			m_playerController->SetShip(Ship::s_playerShip);
		}
	}
	Bullet::s_playerKillsBeforeDeath = 0;
	Ship::s_playerPoints = 0;
	Ship::s_playerShip->SetEnabled(true);
	Ship::s_playerShip->SetCanMove(true);
}
void TheGame::SetUpSelectShipState()
{
	m_scoreHandler->ToggleVisible(false);
	m_title->SetEnabled(false);
	m_spaceMessage->SetEnabled(false);
	m_escMessage->SetEnabled(false);
	m_state = State_SelectShipType;
	if (m_shipSelectorImageHandler != nullptr)
	{
		m_shipSelectorImageHandler->SetVisible(true);
	}
	m_selectedAType = false;
}

void TheGame::BoundsTest()
{
	BoundsTest(0, GameEntity::s_gameEntities->size());
}

void TheGame::BoundsTest(size_t start, size_t end)
{
	if (GameEntity::s_gameEntities == nullptr)
	{
		return;
	}
	for (size_t i = start; i < end; i++)
	{
		GameEntity* entity = GameEntity::s_gameEntities->at(i);
		if (entity != nullptr)
		{
			Vector2 pos = entity->GetPosition();
			if (pos.x >= m_gameEntityBounds.maxs.x || pos.y >= m_gameEntityBounds.maxs.y
				|| pos.x <= m_gameEntityBounds.mins.x || pos.y <= m_gameEntityBounds.mins.y)
			{
				if (entity == Ship::s_playerShip)
				{
					Ship::s_playerShip = nullptr;
				}
				entity->SetIsAlive(false);
				GameEntityType type = entity->GetGameEntityType();
				if (type == TYPE_BULLET)
				{
					Bullet* bulletEntity = (Bullet*)entity;
					bulletEntity->SetCanExplode(false);
				}
			}
		}
	}
}
void TheGame::SpawnEnemyFormation()
{
	float ranPerc = (1.f / (float)((rand() % 100) + 1));
	float neg = (float)(rand() % 2) - 2.f;
	const Vector2 maxWidth = Vector2(6.f, 0.f);
	const Vector2 positionJustAboveScreen = Vector2(0.f, 2.f);
	Vector2 centerPosition = positionJustAboveScreen + (maxWidth * ranPerc * neg);

	EnemyConfigurationGenerator::GenerateRandomNPCControlledShipFormation(centerPosition);
}