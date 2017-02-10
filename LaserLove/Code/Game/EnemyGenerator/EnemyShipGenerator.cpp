#include "Game/EnemyGenerator/EnemyShipGenerator.hpp"
#include "Game/Ships/Ship.hpp"
#include "Game/Ships/Controller/Controller.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/FileUtils.hpp"
#include "Game/SpriteLoader/SpriteResourceList.hpp"
#include "Game/Bullets/BulletConfiguration.hpp"
#include <algorithm>


const char* EnemyShipGenerator::s_EnemyShipGeneratorsFileDirectory = "Data/XML/EnemyShips/";
const char* EnemyShipGenerator::s_baseEnemyShipNodeName = "EnemyShips";
const char* EnemyShipGenerator::s_enemyShipNodeName = "EnemyShip";
const char* EnemyShipGenerator::s_PlayerShipGeneratorsFileDirectory = "Data/XML/PlayerShips/";
const char* EnemyShipGenerator::s_basePlayerShipNodeName = "PlayerShips";
const char* EnemyShipGenerator::s_playerShipNodeName = "PlayerShip";
std::map<std::string, EnemyShipGenerator*>* EnemyShipGenerator::s_factories = nullptr;
std::map<std::string, EnemyShipGenerator*>* EnemyShipGenerator::s_playerFactories = nullptr;
std::vector<std::string>* EnemyShipGenerator::s_factoryNames = nullptr;
std::vector<std::string>* EnemyShipGenerator::s_playerFactoryNames = nullptr;

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//Constructors
EnemyShipGenerator::EnemyShipGenerator()
	: m_name(""),
	m_template(EnemyTemplate())
{

}
EnemyShipGenerator::EnemyShipGenerator(const XMLNode& node)
	: m_name(""),
	m_template(EnemyTemplate())
{
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		if (SimpleStrCmpLower("name", attr.lpszName) == true)
		{
			m_name = std::string(attr.lpszValue);
			m_template.SetName(m_name);
		}
		else if (SimpleStrCmpLower("health", attr.lpszName) == true)
		{
			m_template.SetHealth(atoi(attr.lpszValue));
		}
		else if (SimpleStrCmpLower("scale", attr.lpszName) == true)
		{
			std::string str = ReplaceCharInString(attr.lpszValue, ',', ' ');
			std::vector<std::string> parsed = ParseString(str);
			Vector2 scale;
			if (parsed.size() > 0)
			{
				scale.x = (float)atof(parsed.at(0).c_str());
			}
			if (parsed.size() > 1)
			{
				scale.y = (float)atof(parsed.at(1).c_str());
			}
			
			m_template.SetScale(scale);
		}
		else if (SimpleStrCmpLower("resource", attr.lpszName) == true)
		{
			const SpriteResource* resource = SpriteResourceList::GetSpriteResourceByName(attr.lpszValue);
			m_template.SetSpriteResource(resource);
		}
		else if (SimpleStrCmpLower("collisionscale", attr.lpszName) == true)
		{
			std::string str = ReplaceCharInString(attr.lpszValue, ',', ' ');
			std::vector<std::string> parsed = ParseString(str);
			Vector2 collisionScale;
			if (parsed.size() > 0)
			{
				collisionScale.x = (float)atof(parsed.at(0).c_str());
			}
			if (parsed.size() > 1)
			{
				collisionScale.y = (float)atof(parsed.at(1).c_str());
			}
			m_template.SetCollisionScale(collisionScale);
		}
		else if (SimpleStrCmpLower("speed", attr.lpszName) == true)
		{
			m_template.SetSpeed((float)atof(attr.lpszValue));
		}
		else if (SimpleStrCmpLower("pointsWorth", attr.lpszName) == true)
		{
			m_template.SetPointsWorth(atoi(attr.lpszValue));
		}
	}
	for (int childIdx = 0; childIdx < node.nChildNode(); childIdx++)
	{
		XMLNode child = node.getChildNode(childIdx);
		if (SimpleStrCmp(child.getName(), "") == true)
		{
			continue;
		}
		const BulletConfiguration* config = BulletConfiguration::GetBulletConfiguration(child.getName());
		if (config == nullptr)
		{
			continue;
		}
		m_template.AddBulletConfig(config);
	}
}
EnemyShipGenerator::~EnemyShipGenerator()
{
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//Setters
void EnemyShipGenerator::SetEnemyTemplate(const EnemyTemplate& templ)
{
	m_template = templ;
}
void EnemyShipGenerator::SetName(const std::string& name)
{
	m_name = name;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//Getters
const EnemyTemplate EnemyShipGenerator::GetEnemyTemplate() const
{
	return m_template;
}
const std::string EnemyShipGenerator::GetName() const
{
	return m_name;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//Spawners
Ship* EnemyShipGenerator::SpawnNPCControlledShip(const Vector2& position) const
{
	Ship* ship = m_template.SpawnShipWithNPCController(position, 180.f);
	return ship;
}

Ship* EnemyShipGenerator::SpawnNoControllerShip(const Vector2& position) const
{
	Ship* ship = m_template.SpawnShipWithNPCController(position, 0.f);
	return ship;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//static methods
EnemyShipGenerator* EnemyShipGenerator::GetEnemyShipGeneratorByName(const std::string& name)
{
	if (s_factories == nullptr)
	{
		return nullptr;
	}
	std::map<std::string, EnemyShipGenerator*>::iterator it = s_factories->find(name);
	if (it == s_factories->end())
	{
		return nullptr;
	}
	return it->second;
}

EnemyShipGenerator* EnemyShipGenerator::GetPlayerShipGeneratorByName(const std::string& name)
{
	if (s_playerFactories == nullptr)
	{
		return nullptr;
	}
	std::map<std::string, EnemyShipGenerator*>::iterator it = s_playerFactories->find(name);
	if (it == s_playerFactories->end())
	{
		return nullptr;
	}
	return it->second;
}

Ship* EnemyShipGenerator::SpawnRandomEnemyShipCenterNoController()
{
	if (s_factoryNames == nullptr || s_factoryNames->size() == 0)
	{
		return nullptr;
	}
	size_t ran = rand() % s_factoryNames->size();
	EnemyShipGenerator* shipGen = GetEnemyShipGeneratorByName(s_factoryNames->at(ran));
	if (shipGen == nullptr)
	{
		return nullptr;
	}
	Ship* ship = shipGen->m_template.SpawnShipNoController(Vector2::vec2_zeros, 0.f);
	return ship;
}

Ship* EnemyShipGenerator::SpawnRandomPlayerShipCenterNoController()
{
	if (s_playerFactoryNames == nullptr || s_playerFactoryNames->size() == 0)
	{
		return nullptr;
	}
	size_t ran = rand() % s_playerFactoryNames->size();
	EnemyShipGenerator* shipGen = GetPlayerShipGeneratorByName(s_playerFactoryNames->at(ran));
	if (shipGen == nullptr)
	{
		return nullptr;
	}
	Ship* ship = shipGen->m_template.SpawnShipNoController(Vector2::vec2_zeros, 0.f);
	return ship;
}

Ship* EnemyShipGenerator::SpawnPlayerShipCenterNoControllerByIdx(int idx)
{
	if (s_playerFactoryNames == nullptr || (idx < 0 || idx >= (int)s_playerFactoryNames->size()))
	{
		return nullptr;
	}
	EnemyShipGenerator* shipGen = GetPlayerShipGeneratorByName(s_playerFactoryNames->at(idx));
	if (shipGen == nullptr)
	{
		return nullptr;
	}
	Ship* ship = shipGen->m_template.SpawnShipNoController(Vector2::vec2_zeros, 0.f);
	return ship;
}
const SpriteResource* EnemyShipGenerator::GetResourceByPlayerShipIdx(int idx)
{
	if (s_playerFactoryNames == nullptr || (idx < 0 || idx >= (int)s_playerFactoryNames->size()))
	{
		return nullptr;
	}
	EnemyShipGenerator* shipGen = GetPlayerShipGeneratorByName(s_playerFactoryNames->at(idx));
	if (shipGen == nullptr)
	{
		return nullptr;
	}
	return shipGen->m_template.GetSpriteResource();
}

const Vector2 EnemyShipGenerator::GetScaleByPlayerShipIdx(int idx)
{
	if (s_playerFactoryNames == nullptr || (idx < 0 || idx >= (int)s_playerFactoryNames->size()))
	{
		return Vector2::vec2_zeros;
	}
	EnemyShipGenerator* shipGen = GetPlayerShipGeneratorByName(s_playerFactoryNames->at(idx));
	if (shipGen == nullptr)
	{
		return Vector2::vec2_zeros;
	}
	return shipGen->m_template.GetScale();
}
const Vector2 EnemyShipGenerator::GetTotalSizeByScaleOfPlayerShips()
{
	if (s_playerFactoryNames == nullptr)
	{
		return Vector2::vec2_zeros;
	}
	Vector2 size = Vector2::vec2_zeros;
	for (size_t i = 0; i < s_playerFactoryNames->size(); i++)
	{
		EnemyShipGenerator* shipGen = GetPlayerShipGeneratorByName(s_playerFactoryNames->at(i));
		if (shipGen == nullptr)
		{
			continue;
		}
		Vector2 scale = shipGen->m_template.GetScale();
		size += scale;
	}
	return size;
}

size_t EnemyShipGenerator::NumberOfPlayerShipTypes()
{
	if (s_playerFactoryNames == nullptr)
	{
		return 0;
	}
	return s_playerFactoryNames->size();
}

const bool EnemyShipGenerator::GetDoesEnemyShipGeneratorExistByName(const std::string& name)
{
	if (s_factoryNames == nullptr)
	{
		return false;
	}
	bool exists = false;
	for (size_t i = 0; i < s_factoryNames->size(); i++)
	{
		if (SimpleStrCmp(s_factoryNames->at(i), name) == true)
		{
			exists = true;
			break;
		}
	}
	return exists;
}

const bool EnemyShipGenerator::GetDoesPlayerShipGeneratorExistByName(const std::string& name)
{
	if (s_playerFactoryNames == nullptr)
	{
		return false;
	}
	bool exists = false;
	for (size_t i = 0; i < s_playerFactoryNames->size(); i++)
	{
		if (SimpleStrCmp(s_playerFactoryNames->at(i), name) == true)
		{
			exists = true;
			break;
		}
	}
	return exists;
}

void EnemyShipGenerator::ClearAllEnemyGenerators()
{
	if (s_factories != nullptr)
	{
		for (std::map<std::string, EnemyShipGenerator*>::iterator it = s_factories->begin(); it != s_factories->end(); it++)
		{
			if (it->second != nullptr)
			{
				delete it->second;
				it->second = nullptr;
			}
		}
		s_factories->clear();
		delete s_factories;
		s_factories = nullptr;
	}
	if (s_playerFactories != nullptr)
	{
		for (std::map<std::string, EnemyShipGenerator*>::iterator it = s_playerFactories->begin(); it != s_playerFactories->end(); it++)
		{
			if (it->second != nullptr)
			{
				delete it->second;
				it->second = nullptr;
			}
		}
		s_playerFactories->clear();
		delete s_playerFactories;
		s_playerFactories = nullptr;
	}
	if (s_factoryNames != nullptr)
	{
		s_factoryNames->clear();
		delete s_factoryNames;
		s_factoryNames = nullptr;
	}
	if (s_playerFactoryNames != nullptr)
	{
		s_playerFactoryNames->clear();
		delete s_playerFactoryNames;
		s_playerFactoryNames = nullptr;
	}
}
void EnemyShipGenerator::LoadAllShipGenerators()
{
	LoadAllEnemyGenerators();
	LoadAllPlayerGenerators();
}
void EnemyShipGenerator::LoadAllEnemyGenerators()
{
	std::vector<std::string> fileLocs = FileUtils::EnumerateFilesInDirectory(s_EnemyShipGeneratorsFileDirectory, "*");
	for (size_t fileIdx = 0; fileIdx < fileLocs.size(); fileIdx++)
	{
		std::string file = fileLocs.at(fileIdx);
		std::string fileExtension = file.substr(file.size() - 3, file.size());
		std::transform(fileExtension.begin(), fileExtension.end(), fileExtension.begin(), ::tolower);
		if (SimpleStrCmp(fileExtension, "xml") == false)
		{
			fileLocs.erase(fileLocs.begin() + fileIdx);
			fileIdx--;
		}
	}
	for (size_t fileIdx = 0; fileIdx < fileLocs.size(); fileIdx++)
	{
		bool couldNotLoad = false;
		std::string fileLoc = fileLocs.at(fileIdx);
		XMLNode root = EngineXMLParser::ParseXMLFile(fileLoc, s_baseEnemyShipNodeName, couldNotLoad);
		if (couldNotLoad == true)
		{
			continue;
		}
		for (int childIdx = 0; childIdx < root.nChildNode(); childIdx++)
		{
			XMLNode child = root.getChildNode(childIdx);
			
			if (SimpleStrCmp(child.getName(), s_enemyShipNodeName) == true)
			{
				EnemyShipGenerator* generator = new EnemyShipGenerator(child);
				std::string name = generator->GetName();
				if (SimpleStrCmp(name, "") == true || GetDoesEnemyShipGeneratorExistByName(name) == true)
				{
					delete generator;
					continue;
				}
				if (s_factoryNames == nullptr)
				{
					s_factoryNames = new std::vector<std::string>();
				}
				s_factoryNames->push_back(name);
				if (s_factories == nullptr)
				{
					s_factories = new std::map<std::string, EnemyShipGenerator*>();
				}
				s_factories->insert(std::pair<std::string, EnemyShipGenerator*>(name, generator));
			}
		}
	}
}

void EnemyShipGenerator::LoadAllPlayerGenerators()
{
	std::vector<std::string> fileLocs = FileUtils::EnumerateFilesInDirectory(s_PlayerShipGeneratorsFileDirectory, "*");
	for (size_t fileIdx = 0; fileIdx < fileLocs.size(); fileIdx++)
	{
		std::string file = fileLocs.at(fileIdx);
		std::string fileExtension = file.substr(file.size() - 3, file.size());
		std::transform(fileExtension.begin(), fileExtension.end(), fileExtension.begin(), ::tolower);
		if (SimpleStrCmp(fileExtension, "xml") == false)
		{
			fileLocs.erase(fileLocs.begin() + fileIdx);
			fileIdx--;
		}
	}
	for (size_t fileIdx = 0; fileIdx < fileLocs.size(); fileIdx++)
	{
		bool couldNotLoad = false;
		std::string fileLoc = fileLocs.at(fileIdx);
		XMLNode root = EngineXMLParser::ParseXMLFile(fileLoc, s_basePlayerShipNodeName, couldNotLoad);
		if (couldNotLoad == true)
		{
			continue;
		}
		for (int childIdx = 0; childIdx < root.nChildNode(); childIdx++)
		{
			XMLNode child = root.getChildNode(childIdx);

			if (SimpleStrCmp(child.getName(), s_playerShipNodeName) == true)
			{
				EnemyShipGenerator* generator = new EnemyShipGenerator(child);
				std::string name = generator->GetName();
				if (SimpleStrCmp(name, "") == true || GetDoesEnemyShipGeneratorExistByName(name) == true)
				{
					delete generator;
					continue;
				}
				if (s_playerFactoryNames == nullptr)
				{
					s_playerFactoryNames = new std::vector<std::string>();
				}
				s_playerFactoryNames->push_back(name);
				if (s_playerFactories == nullptr)
				{
					s_playerFactories = new std::map<std::string, EnemyShipGenerator*>();
				}
				s_playerFactories->insert(std::pair<std::string, EnemyShipGenerator*>(name, generator));
			}
		}
	}
}