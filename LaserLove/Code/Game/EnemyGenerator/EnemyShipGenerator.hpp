#pragma once

#ifndef ENEMYSHIPGENERATOR_HPP
#define ENEMYSHIPGENERATOR_HPP
#include "Game/EnemyGenerator/EnemyTemplate.hpp"
#include "Engine/Core/EngineXMLParser.hpp"
#include "Engine/Utils/AllocationsSpecial/untrackedallocations.hpp"

class EnemyShipGenerator
{
private:
	std::string m_name;
	EnemyTemplate m_template;

protected:
	static const char* s_EnemyShipGeneratorsFileDirectory;
	static const char* s_baseEnemyShipNodeName;
	static const char* s_enemyShipNodeName;
	static const char* s_PlayerShipGeneratorsFileDirectory;
	static const char* s_basePlayerShipNodeName;
	static const char* s_playerShipNodeName;
	static std::map<std::string, EnemyShipGenerator*>* s_factories;
	static std::map<std::string, EnemyShipGenerator*>* s_playerFactories;
	static std::vector<std::string>* s_factoryNames;
	static std::vector<std::string>* s_playerFactoryNames;
public:
	//Constructors
	EnemyShipGenerator();
	EnemyShipGenerator(const XMLNode& node);
	virtual ~EnemyShipGenerator();

	//Setters
	void SetEnemyTemplate(const EnemyTemplate& templ);
	void SetName(const std::string& name);

	//Getters
	const EnemyTemplate GetEnemyTemplate() const;
	const std::string GetName() const;

	//Spawners
	Ship* SpawnNPCControlledShip(const Vector2& position) const;
	Ship* SpawnNoControllerShip(const Vector2& position) const;

	//static methods
	static EnemyShipGenerator* GetEnemyShipGeneratorByName(const std::string& name);
	static EnemyShipGenerator* GetPlayerShipGeneratorByName(const std::string& name);
	static Ship* SpawnRandomEnemyShipCenterNoController();
	static Ship* SpawnRandomPlayerShipCenterNoController();
	static Ship* SpawnPlayerShipCenterNoControllerByIdx(int idx);
	static const SpriteResource* GetResourceByPlayerShipIdx(int idx);
	static const Vector2 GetScaleByPlayerShipIdx(int idx);
	static const Vector2 GetTotalSizeByScaleOfPlayerShips();
	static size_t NumberOfPlayerShipTypes();
	static const bool GetDoesEnemyShipGeneratorExistByName(const std::string& name);
	static const bool GetDoesPlayerShipGeneratorExistByName(const std::string& name);
	static void ClearAllEnemyGenerators();
	static void LoadAllShipGenerators();
	static void LoadAllEnemyGenerators();
	static void LoadAllPlayerGenerators();
};
#endif