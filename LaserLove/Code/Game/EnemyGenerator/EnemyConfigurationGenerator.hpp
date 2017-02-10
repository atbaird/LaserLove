#pragma once

#ifndef ENEMYCONFIGURATIONGENERATOR_HPP
#define ENEMYCONFIGURATIONGENERATOR_HPP
#include "Game/EnemyGenerator/EnemyConfigurationTemplate.hpp"
#include "Engine/Core/EngineXMLParser.hpp"
#include "Engine/Utils/AllocationsSpecial/untrackedallocations.hpp"
#include <map>

class EnemyConfigurationGenerator
{
private:
	std::string m_name;
	EnemyConfigurationTemplate m_template;
protected:
	static const char* s_EnemyShipConfigurationGeneratorsFileDirectory;
	static const char* s_baseEnemyShipConfigurationNodeName;
	static const char* s_enemyShipConfigurationNodeName;
	static std::map<std::string, EnemyConfigurationGenerator*>* s_factories;
	static std::vector<std::string>* s_factoryNames;
public:
	//Constructors
	EnemyConfigurationGenerator();
	EnemyConfigurationGenerator(const XMLNode& node);
	virtual ~EnemyConfigurationGenerator();

	//Setters
	void SetTemplate(const EnemyConfigurationTemplate& templ);
	void SetName(const std::string& name);

	//Getters
	const EnemyConfigurationTemplate GetTemplate() const;
	const std::string GetName() const;

	//Spawners
	const std::vector<Ship*> SpawnNPCControlledShipFormation(const Vector2& centerPosition);

	//static methods
	static void ClearAllEnemyConfigurationGenerators();
	static void ClearAllEnemyShipsAndEnemyConfigurationGenerators();
	static void LoadAllEnemyConfigurationGenerators();
	static EnemyConfigurationGenerator* GetEnemyConfigurationGeneratorForName(const std::string& name);
	static const bool GetDoesEnemyConfigurationGeneratorExistForName(const std::string& name);
	static const std::vector<Ship*> GenerateRandomNPCControlledShipFormation(const Vector2& centerPosition);

};
#endif