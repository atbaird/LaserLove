#include "Game/EnemyGenerator/EnemyConfigurationTemplate.hpp"
#include "Game/EnemyGenerator/EnemyShipGenerator.hpp"

//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
//Constructors
EnemyConfigurationTemplate::EnemyConfigurationTemplate()
{

}
EnemyConfigurationTemplate::EnemyConfigurationTemplate(const std::vector<ShipNameAndPlacement>& shipsAndPlacement)
	: m_shipsAndPlacement(shipsAndPlacement)
{

}

//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
//Setters
void EnemyConfigurationTemplate::AddShipAndPlacement(const ShipNameAndPlacement& placement)
{
	m_shipsAndPlacement.push_back(placement);
}
void EnemyConfigurationTemplate::SetShipAndPlacement(const std::vector<ShipNameAndPlacement>& placements)
{
	m_shipsAndPlacement = placements;
}

//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
//Getters
const std::vector<ShipNameAndPlacement> EnemyConfigurationTemplate::GetShipPlacements()
{
	return m_shipsAndPlacement;
}

//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
//Spawners
const std::vector<Ship*> EnemyConfigurationTemplate::GetConfigurationWithNPCControllers(const Vector2& centerPos)
{
	std::vector<Ship*> ships;
	for (size_t i = 0; i < m_shipsAndPlacement.size(); i++)
	{
		ShipNameAndPlacement currentShip = m_shipsAndPlacement.at(i);
		EnemyShipGenerator* shipGen = EnemyShipGenerator::GetEnemyShipGeneratorByName(currentShip.m_shipGeneratorName);
		if (shipGen != nullptr)
		{
			Ship* newShip = shipGen->SpawnNPCControlledShip(currentShip.m_relativePosition + centerPos);
			ships.push_back(newShip);
		}
	}
	return ships;
}