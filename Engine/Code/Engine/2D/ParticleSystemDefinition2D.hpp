#pragma once

#ifndef PARTICLESYSTEMDEFINITION_HPP
#define PARTICLESYSTEMDEFINITION_HPP
#include "Engine/2D/ParticleEmitterDefintion2D.hpp"
#include <vector>
#include <map>
#include <string>

class ParticleSystem2D;

class ParticleSystemDefinition2D
{
private:
	std::string m_name;
	std::vector<ParticleEmitterDefinition2D*> particleEmitterDefinitions;
protected:
	static std::map<std::string, ParticleSystemDefinition2D*>* s_ParticleSystemDefinitions;
public:
	static bool s_ClearedParticleSystemDefinitions;
	//Constructors
	ParticleSystemDefinition2D(XMLNode node);
	ParticleSystemDefinition2D(const std::string& name = "", const std::vector<ParticleEmitterDefinition2D*>& particleEmitDefs = std::vector<ParticleEmitterDefinition2D*>());
	virtual ~ParticleSystemDefinition2D();

	//Adders and Removers
	void AddParticleEmitterDefinitionName(ParticleEmitterDefinition2D* definition);
	void RemoveParticleEmitterDefinitionByIdx(const int& idx);

	//Getters
	const std::string GetName() const;
	const std::vector<ParticleEmitterDefinition2D*>* GetParticleEmitterDefinitionNames() const;

	//static methods
	static const bool RegisterParticleSystemDefinition(const std::string& name, ParticleSystemDefinition2D* def);
	static ParticleSystemDefinition2D* FindOrCreateParticleSystemDefinition(const std::string& name);
	static void ClearAllParticleSystemDefinitions();
	static ParticleSystem2D* CreateParticleSystem(const char* tag, size_t layer, const Vector2& position);
	static std::vector<ParticleSystemDefinition2D*> BuildAndRegisterParticleSystemDefinitionsFromXMLNode(const std::string& fileLocation);
	static std::vector<ParticleSystemDefinition2D*> BuildAndRegisterAllParticleSystemDefinitionsFromXMLFilesInFolder(const std::string& folderLocation);
};
#endif