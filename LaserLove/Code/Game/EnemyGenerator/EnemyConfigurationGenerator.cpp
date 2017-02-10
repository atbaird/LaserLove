#include "Game/EnemyGenerator/EnemyConfigurationGenerator.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/FileUtils.hpp"
#include "Game/EnemyGenerator/EnemyShipGenerator.hpp"
#include <algorithm>

std::map<std::string, EnemyConfigurationGenerator*>* EnemyConfigurationGenerator::s_factories = nullptr;
std::vector<std::string>* EnemyConfigurationGenerator::s_factoryNames = nullptr;
const char* EnemyConfigurationGenerator::s_EnemyShipConfigurationGeneratorsFileDirectory = "Data/XML/EnemyShipConfigurations/";
const char* EnemyConfigurationGenerator::s_baseEnemyShipConfigurationNodeName = "EnemyShipConfigurations";
const char* EnemyConfigurationGenerator::s_enemyShipConfigurationNodeName = "EnemyShipConfiguration";

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//Constructors
EnemyConfigurationGenerator::EnemyConfigurationGenerator()
	: m_name(""),
	m_template()
{

}
EnemyConfigurationGenerator::EnemyConfigurationGenerator(const XMLNode& node)
	: m_name(""),
	m_template()
{
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		if (SimpleStrCmp(attr.lpszName, "name") == true)
		{
			m_name = std::string(attr.lpszValue);
		}
	}
	for (int childIdx = 0; childIdx < node.nChildNode(); childIdx++)
	{
		XMLNode child = node.getChildNode(childIdx);
		if (SimpleStrCmp(child.getName(), "EnemyPosition") == true)
		{
			ShipNameAndPlacement placement;
			for (int attrIdx = 0; attrIdx < child.nAttribute(); attrIdx++)
			{
				XMLAttribute attr = child.getAttribute(attrIdx);
				if (SimpleStrCmp(attr.lpszName, "shipname") == true)
				{
					placement.m_shipGeneratorName = std::string(attr.lpszValue);
				}
				else if (SimpleStrCmp(attr.lpszName, "relativeposition") == true)
				{
					std::string val = ReplaceCharInString(attr.lpszValue, ',', ' ');
					std::vector<std::string> parsed = ParseString(val);
					Vector2 relPos;
					if (parsed.size() > 0)
					{
						relPos.x = (float)atof(parsed.at(0).c_str());
					}
					if (parsed.size() > 1)
					{
						relPos.y = (float)atof(parsed.at(1).c_str());
					}
					
					placement.m_relativePosition = relPos;
				}
			}
			m_template.AddShipAndPlacement(placement);
		}
	}
}
EnemyConfigurationGenerator::~EnemyConfigurationGenerator()
{

}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//Setters
void EnemyConfigurationGenerator::SetTemplate(const EnemyConfigurationTemplate& templ)
{
	m_template = templ;
}
void EnemyConfigurationGenerator::SetName(const std::string& name)
{
	m_name = name;
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//Getters
const EnemyConfigurationTemplate EnemyConfigurationGenerator::GetTemplate() const
{
	return m_template;
}
const std::string EnemyConfigurationGenerator::GetName() const
{
	return m_name;
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//Spawners
const std::vector<Ship*> EnemyConfigurationGenerator::SpawnNPCControlledShipFormation(const Vector2& centerPosition)
{
	std::vector<Ship*> ships = m_template.GetConfigurationWithNPCControllers(centerPosition);
	return ships;
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------
//static methods
void EnemyConfigurationGenerator::ClearAllEnemyConfigurationGenerators()
{
	if (s_factories != nullptr)
	{
		for (std::map<std::string, EnemyConfigurationGenerator*>::iterator it = s_factories->begin(); it != s_factories->end(); it++)
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
	if (s_factoryNames != nullptr)
	{
		s_factoryNames->clear();
		delete s_factoryNames;
		s_factoryNames = nullptr;
	}
	
}
void EnemyConfigurationGenerator::ClearAllEnemyShipsAndEnemyConfigurationGenerators()
{
	ClearAllEnemyConfigurationGenerators();
	EnemyShipGenerator::ClearAllEnemyGenerators();
}
void EnemyConfigurationGenerator::LoadAllEnemyConfigurationGenerators()
{
	EnemyShipGenerator::LoadAllShipGenerators();
	std::vector<std::string> fileLocs = FileUtils::EnumerateFilesInDirectory(s_EnemyShipConfigurationGeneratorsFileDirectory, "*");
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
		XMLNode root = EngineXMLParser::ParseXMLFile(fileLoc, s_baseEnemyShipConfigurationNodeName, couldNotLoad);
		if (couldNotLoad == true)
		{
			continue;
		}
		for (int childIdx = 0; childIdx < root.nChildNode(); childIdx++)
		{
			XMLNode child = root.getChildNode(childIdx);

			if (SimpleStrCmp(child.getName(), s_enemyShipConfigurationNodeName) == true)
			{
				EnemyConfigurationGenerator* generator = new EnemyConfigurationGenerator(child);
				std::string name = generator->GetName();
				if (SimpleStrCmp(name, "") == true || GetDoesEnemyConfigurationGeneratorExistForName(name) == true)
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
					s_factories = new std::map<std::string, EnemyConfigurationGenerator*>();
				}
				s_factories->insert(std::pair<std::string, EnemyConfigurationGenerator*>(name, generator));
			}
		}
	}
}
EnemyConfigurationGenerator* EnemyConfigurationGenerator::GetEnemyConfigurationGeneratorForName(const std::string& name)
{
	if (s_factories == nullptr)
	{
		return nullptr;
	}
	std::map<std::string, EnemyConfigurationGenerator*>::iterator it = s_factories->find(name);
	if (it == s_factories->end())
	{
		return nullptr;
	}
	return it->second;
}
const bool EnemyConfigurationGenerator::GetDoesEnemyConfigurationGeneratorExistForName(const std::string& name)
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
const std::vector<Ship*> EnemyConfigurationGenerator::GenerateRandomNPCControlledShipFormation(const Vector2& centerPosition)
{
	if (s_factoryNames == nullptr || s_factories == nullptr)
	{
		return std::vector<Ship*>();
	}
	size_t ran = rand() % s_factoryNames->size();
	std::string name = s_factoryNames->at(ran);
	std::map<std::string, EnemyConfigurationGenerator*>::iterator it = s_factories->find(name);
	if (it == s_factories->end() || it->second == nullptr)
	{
		return std::vector<Ship*>();
	}

	std::vector<Ship*> ships = it->second->SpawnNPCControlledShipFormation(centerPosition);
	return ships;
}