#pragma once

#ifndef ENEMYCONFIGURATIONTEMPLATE_HPP
#define ENEMYCONFIGURATIONTEMPLATE_HPP
#include <string>
#include "Engine/Math/Vector2.hpp"
#include <vector>

class Ship;

struct ShipNameAndPlacement
{
	std::string m_shipGeneratorName;
	Vector2 m_relativePosition;
};

class EnemyConfigurationTemplate
{
private:
	std::vector<ShipNameAndPlacement> m_shipsAndPlacement;
public:
	//Constructors
	EnemyConfigurationTemplate();
	EnemyConfigurationTemplate(const std::vector<ShipNameAndPlacement>& shipsAndPlacement);

	//Setters
	void AddShipAndPlacement(const ShipNameAndPlacement& placement);
	void SetShipAndPlacement(const std::vector<ShipNameAndPlacement>& placements);

	//Getters
	const std::vector<ShipNameAndPlacement> GetShipPlacements();

	//Spawners
	const std::vector<Ship*> GetConfigurationWithNPCControllers(const Vector2& centerPos);
};
#endif