#include "Engine/2D/ParticleSystemDefinition2D.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/2D/ParticleSystem2D.hpp"
#include "Engine/Core/FileUtils.hpp"

std::map<std::string, ParticleSystemDefinition2D*>* ParticleSystemDefinition2D::s_ParticleSystemDefinitions = nullptr;
bool ParticleSystemDefinition2D::s_ClearedParticleSystemDefinitions = false;
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//Constructors

ParticleSystemDefinition2D::ParticleSystemDefinition2D(XMLNode node)
	: m_name("")
{
	for (int attrIdx = 0; attrIdx < node.nAttribute(); attrIdx++)
	{
		XMLAttribute attr = node.getAttribute(attrIdx);
		if (SimpleStrCmpLower(attr.lpszName, "name"))
		{
			m_name = std::string(attr.lpszValue);
		}
	}

	for (int childIdx = 0; childIdx < node.nChildNode(); childIdx++)
	{
		XMLNode child = node.getChildNode(childIdx);
		if (SimpleStrCmpLower(child.getName(), "ParticleEmitterDefinition2D") == true)
		{
			ParticleEmitterDefinition2D* newDef = new ParticleEmitterDefinition2D(child);
			particleEmitterDefinitions.push_back(newDef);
		}
	}
}
ParticleSystemDefinition2D::ParticleSystemDefinition2D(const std::string& name, 
	const std::vector<ParticleEmitterDefinition2D*>& particleEmitDefs)
	: m_name(name),
	particleEmitterDefinitions(particleEmitDefs)
{

}
ParticleSystemDefinition2D::~ParticleSystemDefinition2D()
{
	for (size_t i = 0; i < particleEmitterDefinitions.size(); i++)
	{
		if (particleEmitterDefinitions[i] != nullptr)
		{
			delete particleEmitterDefinitions[i];
			particleEmitterDefinitions[i] = nullptr;
		}
	}
	particleEmitterDefinitions.clear();
}

//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//Adders and Removers
void ParticleSystemDefinition2D::AddParticleEmitterDefinitionName(ParticleEmitterDefinition2D* emitterDefName)
{
	particleEmitterDefinitions.push_back(emitterDefName);
}

void ParticleSystemDefinition2D::RemoveParticleEmitterDefinitionByIdx(const int& idx)
{
	if (idx < 0 || (size_t)idx >= particleEmitterDefinitions.size())
	{
		return;
	}
	particleEmitterDefinitions.erase(particleEmitterDefinitions.begin() + idx);
}

//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//Getters
const std::string ParticleSystemDefinition2D::GetName() const
{
	return m_name;
}
const std::vector<ParticleEmitterDefinition2D*>* ParticleSystemDefinition2D::GetParticleEmitterDefinitionNames() const
{
	return &particleEmitterDefinitions;
}

//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------
//Static Methods
const bool ParticleSystemDefinition2D::RegisterParticleSystemDefinition(const std::string& name, ParticleSystemDefinition2D* def)
{
	if (s_ParticleSystemDefinitions == nullptr)
	{
		s_ParticleSystemDefinitions = new std::map<std::string, ParticleSystemDefinition2D*>();
	}
	std::map<std::string, ParticleSystemDefinition2D*>::iterator it = s_ParticleSystemDefinitions->find(name);
	if(it == s_ParticleSystemDefinitions->end())
	{
		s_ParticleSystemDefinitions->insert(std::pair<std::string, ParticleSystemDefinition2D*>(name, def));
		return true;
	}
	else if (it->second == nullptr)
	{
		it->second = def;
		return true;
	}
	return false;
}

ParticleSystemDefinition2D* ParticleSystemDefinition2D::FindOrCreateParticleSystemDefinition(const std::string& name)
{
	if (s_ParticleSystemDefinitions == nullptr)
	{
		s_ParticleSystemDefinitions = new std::map<std::string, ParticleSystemDefinition2D*>();
	}
	std::map<std::string, ParticleSystemDefinition2D*>::iterator it = s_ParticleSystemDefinitions->find(name);
	ParticleSystemDefinition2D* ptr = nullptr;
	if (it == s_ParticleSystemDefinitions->end())
	{
		ptr = new ParticleSystemDefinition2D();
		s_ParticleSystemDefinitions->insert(std::pair<std::string, ParticleSystemDefinition2D*>(name, ptr));
	}
	else
	{
		ptr = it->second;
	}
	return ptr;
}

void ParticleSystemDefinition2D::ClearAllParticleSystemDefinitions()
{
	if (s_ParticleSystemDefinitions == nullptr)
	{
		return;
	}
	for (std::map<std::string, ParticleSystemDefinition2D*>::iterator it = s_ParticleSystemDefinitions->begin(); it != s_ParticleSystemDefinitions->end(); it++)
	{
		if (it->second != nullptr)
		{
			delete it->second;
			it->second = nullptr;
		}
	}
	if (s_ParticleSystemDefinitions != nullptr)
	{
		delete s_ParticleSystemDefinitions;
		s_ParticleSystemDefinitions = nullptr;
	}
	s_ClearedParticleSystemDefinitions = true;
}

ParticleSystem2D* ParticleSystemDefinition2D::CreateParticleSystem(const char* tag, size_t layer, 
	const Vector2& position)
{
	if (s_ParticleSystemDefinitions == nullptr)
	{
		return nullptr;
	}
	std::map<std::string, ParticleSystemDefinition2D*>::iterator it = s_ParticleSystemDefinitions->find(std::string(tag));
	if (it == s_ParticleSystemDefinitions->end())
	{
		return nullptr;
	}
	ParticleSystemDefinition2D* definition = it->second;
	if (definition == nullptr)
	{
		return nullptr;
	}
	ParticleSystem2D* system = new ParticleSystem2D(tag, layer, position);
	system->SetParticleSystemDefinition(definition);
	system->ResetFromParticleSystemDefinition();

	return system;
}

std::vector<ParticleSystemDefinition2D*> ParticleSystemDefinition2D::BuildAndRegisterParticleSystemDefinitionsFromXMLNode(const std::string& fileLocation)
{
	std::vector<ParticleSystemDefinition2D*> defs;
	if (!SimpleStrCmpLower(".xml", fileLocation.substr(fileLocation.size() - 5, 4)))
	{
		return defs;
	}
	bool errorOccured = false;
	XMLNode root = EngineXMLParser::ParseXMLFile(fileLocation, "ParticleSystemDefinitions", errorOccured);
	if (errorOccured)
	{
		return defs;
	}
	for (int i = 0; i < root.nChildNode(); i++)
	{
		XMLNode node = root.getChildNode(i);
		if (SimpleStrCmpLower(node.getName(), "ParticleSystemDefinition2D"))
		{
			ParticleSystemDefinition2D* def = new ParticleSystemDefinition2D(node);
			if (SimpleStrCmp("", def->GetName()) == true)
			{
				delete def;
				continue;
			}
			bool registered = RegisterParticleSystemDefinition(def->GetName(), def);
			if (registered == false)
			{
				delete def;
				continue;
			}
			defs.push_back(def);
		}
	}

	return defs;
}

std::vector<ParticleSystemDefinition2D*> ParticleSystemDefinition2D::BuildAndRegisterAllParticleSystemDefinitionsFromXMLFilesInFolder(const std::string& folderLocation)
{
	std::vector<ParticleSystemDefinition2D*> allBuiltParticleSystemDefs;
	std::vector<std::string> files = FileUtils::EnumerateFilesInDirectory(folderLocation, "*");
	for (size_t i = 0; i < files.size(); i++)
	{
		std::string fileLoc = files.at(i);
		std::vector<ParticleSystemDefinition2D*> allFound = BuildAndRegisterParticleSystemDefinitionsFromXMLNode(fileLoc);
		allBuiltParticleSystemDefs.insert(allBuiltParticleSystemDefs.end(), allFound.begin(), allFound.end());
	}

	return allBuiltParticleSystemDefs;
}