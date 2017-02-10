#include "Game/Bullets/BulletConfiguration.hpp"
#include "Game/Bullets/Bullet.hpp"
#include "Game/Ships/Ship.hpp"
#include "Game/SpriteLoader/CommonVariables.hpp"
#include "Engine/Core/EngineXMLParser.hpp"
#include "Engine/Core/StringUtils.hpp"
#include "Engine/Core/FileUtils.hpp"
#include "Game/SpriteLoader/SpriteResourceList.hpp"
#include "Engine/2D/SpriteAnimationSequence.hpp"
#include <algorithm>

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
//Struct
BulletPosAngSpeed::BulletPosAngSpeed(const Vector2& pos, const float& ang, const float& sp, const float& angularVel)
	: relPos(pos),
	angle(ang),
	speed(sp),
	angularVelocity(angularVel),
	numTimesExplode(0),
	imageaddedrotation(0.f),
	additiveAngularVelocity(0.f),
	secondsAfterBornToDie(-1.f)
{
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
//Constructors
std::map<std::string, BulletConfiguration*>* BulletConfiguration::s_configurations = nullptr;
const std::string BulletConfiguration::s_bulletConfigurationRootFolder = "Data/XML/BulletConfigurations/";
const char* BulletConfiguration::s_baseBulletConfigurationsNodeName = "BulletConfigurations";
const char* BulletConfiguration::s_bulletConfigurationNodeName = "BulletConfiguration";
BulletConfiguration::BulletConfiguration(const SpriteResource* resource)
	: m_resource(resource),
	m_usesResource(true),
	m_sequence(nullptr),
	m_collisionScale(Vector2::vec2_ones),
	m_time(0.f)
{

}
BulletConfiguration::BulletConfiguration(const SpriteResource* resource, const std::vector<BulletPosAngSpeed>& positions)
	: m_resource(resource),
	m_bulletRels(positions),
	m_usesResource(true),
	m_sequence(nullptr),
	m_collisionScale(Vector2::vec2_ones),
	m_time(0.f)
{

}
BulletConfiguration::BulletConfiguration(SpriteAnimationSequence* sequence, const float& time)
	: m_resource(nullptr),
	m_usesResource(false),
	m_sequence(sequence),
	m_collisionScale(Vector2::vec2_ones),
	m_time(time)
{

}
BulletConfiguration::BulletConfiguration(SpriteAnimationSequence* sequence, const std::vector<BulletPosAngSpeed>& positions,
	const float& time)
	: m_resource(nullptr),
	m_bulletRels(positions),
	m_usesResource(false),
	m_sequence(sequence),
	m_collisionScale(Vector2::vec2_ones),
	m_time(time)
{

}
BulletConfiguration::~BulletConfiguration()
{
	m_resource = nullptr;
	m_sequence = nullptr;
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
//Setters
void BulletConfiguration::SetTime(const float& time)
{
	m_time = time;
}
void BulletConfiguration::SetResource(const SpriteResource* resource)
{
	m_resource = nullptr;
	m_resource = resource;
	m_usesResource = true;
}
void BulletConfiguration::SetSequence(SpriteAnimationSequence* sequence)
{
	m_sequence = nullptr;
	m_sequence = sequence;
	m_usesResource = false;
}
void BulletConfiguration::AddBulletPosition(const BulletPosAngSpeed& pos)
{
	m_bulletRels.push_back(pos);
}
void BulletConfiguration::SetBulletPositions(const std::vector<BulletPosAngSpeed>& positions)
{
	m_bulletRels = positions;
}
void BulletConfiguration::AppendBulletPosition(const std::vector<BulletPosAngSpeed>& positions)
{
	m_bulletRels.insert(m_bulletRels.end(), positions.begin(), positions.end());
}
void BulletConfiguration::SetCollisionScale(const Vector2& colScale)
{
	m_collisionScale = colScale;
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
//Spawners
const std::vector<Bullet*> BulletConfiguration::SpawnBullets(const Ship* instigator, const Vector2& shipCurrentSpeed) const
{
	std::vector<Bullet*> bullets;
	if (instigator == nullptr || m_bulletRels.size() == 0)
	{
		return bullets;
	}

	Controller* instigatorController = instigator->GetEditableController();
	const float rotation = instigator->GetRotation() + 90.f;
	const Vector2 center = instigator->GetPosition();
	const Vector2 forward = instigator->GetForward();
	const Vector2 right = instigator->GetRight();
	const size_t teamID = instigator->GetTeamID();
	const float dotProd = forward.dotProduct(shipCurrentSpeed);


	for (size_t i = 0; i < m_bulletRels.size(); i++)
	{
		BulletPosAngSpeed pos = m_bulletRels.at(i);
		Bullet* bullet = nullptr;
		if (m_usesResource)
		{
			bullet = new Bullet(m_resource);
		}
		else
		{
			bullet = new Bullet(m_sequence, m_time);
		}
		bullet->SetPosition(Vector2(center.x + ((forward.x * pos.relPos.x) - (forward.y * pos.relPos.y)),
			center.y + ((forward.y * pos.relPos.x) + (forward.x * pos.relPos.y))));
		bullet->SetAngularVelocity(pos.angularVelocity);
		bullet->SetSpeed(pos.speed + (dotProd * pos.speed * (3.f/ 4.f)));
		bullet->SetRotation(pos.angle + rotation);
		bullet->SetCollisionRadius(pos.collisionScale);
		bullet->SetNumTimesExplode(pos.numTimesExplode);
		bullet->SetInstigatorController(instigatorController);
		bullet->SetAngularVelocityAddedAfterExplosion(pos.additiveAngularVelocity);
		bullet->SetAddedRotationForImage(pos.imageaddedrotation);
		bullet->SetSecondsBeforeDie(pos.secondsAfterBornToDie);
		bullet->SetTeamID(teamID);
		bullet->ChangeLayers(CommonVariables::s_bulletLayer);
		bullets.push_back(bullet);
		//direction.x * cos - direction.y * sin,
		//	direction.x * sin + direction.y * cos,
	}

	return bullets;
}

//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
//Static Methods
const bool BulletConfiguration::RegisterBulletConfiguration(const std::string& name, BulletConfiguration* configuration)
{
	if (configuration == nullptr)
	{
		return false;
	}
	if (s_configurations == nullptr)
	{
		s_configurations = new std::map<std::string, BulletConfiguration*>();
	}
	std::map<std::string, BulletConfiguration*>::const_iterator it = s_configurations->find(name);
	if (it != s_configurations->end())
	{
		return false;
	}
	s_configurations->insert(std::pair<std::string, BulletConfiguration*>(name, configuration));
	return true;
}
const BulletConfiguration* BulletConfiguration::GetBulletConfiguration(const std::string& name)
{
	if (s_configurations == nullptr)
	{
		return nullptr;
	}
	std::map<std::string, BulletConfiguration*>::const_iterator it = s_configurations->find(name);
	if (it == s_configurations->end())
	{
		return nullptr;
	}
	return it->second;
}
void BulletConfiguration::ClearAllBulletConfigurations()
{
	if (s_configurations == nullptr)
	{
		return;
	}
	for (std::map<std::string, BulletConfiguration*>::iterator it = s_configurations->begin(); it != s_configurations->end(); it++)
	{
		if (it->second != nullptr)
		{
			delete it->second;
			it->second = nullptr;
		}
	}
	s_configurations->clear();
	delete s_configurations;
	s_configurations = nullptr;
}
void BulletConfiguration::LoadAllBulletConfigurations()
{
	std::vector<std::string> fileLocs = FileUtils::EnumerateFilesInDirectory(s_bulletConfigurationRootFolder, "*");
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
		std::string file = fileLocs.at(fileIdx);
		XMLNode root = EngineXMLParser::ParseXMLFile(file, s_baseBulletConfigurationsNodeName, couldNotLoad);
		if (couldNotLoad)
		{
			continue;
		}
		for (int childIdx = 0; childIdx < root.nChildNode(); childIdx++)
		{
			XMLNode child = root.getChildNode(childIdx);
			if (SimpleStrCmp(child.getName(), s_bulletConfigurationNodeName) == false)
			{
				continue;
			}
			std::string name;
			BulletConfiguration* bulletConfig = new BulletConfiguration();
			std::string resourceName = "";
			bool isASequence = false;
			float time = 1.f;
			for (int attrIdx = 0; attrIdx < child.nAttribute(); attrIdx++)
			{
				XMLAttribute attr = child.getAttribute(attrIdx);
				if (SimpleStrCmp(attr.lpszName, "name"))
				{
					name = std::string(attr.lpszValue);
				}
				else if (SimpleStrCmp(attr.lpszName, "resource") == true)
				{
					resourceName = (attr.lpszValue);
				}
				else if (SimpleStrCmp(attr.lpszName, "isASequence"))
				{
					if (SimpleStrCmp(attr.lpszValue, "true")
						|| SimpleStrCmp(attr.lpszValue, "t"))
					{
						isASequence = true;
					}
				}
				else if (SimpleStrCmp(attr.lpszName, "time"))
				{
					time = (float)atof(attr.lpszValue);
				}
			}

			if (SimpleStrCmp(name, "") == true || SimpleStrCmp(resourceName, "") == true)
			{
				delete bulletConfig;
				continue;
			}
			if (isASequence == true)
			{
				bulletConfig->SetSequence(SpriteResourceList::GetSpriteAnimationSequenceByName(resourceName));
			}
			else
			{
				bulletConfig->SetResource(SpriteResourceList::GetSpriteResourceByName(resourceName));
			}
			for (int bulletIdx = 0; bulletIdx < child.nChildNode(); bulletIdx++)
			{
				XMLNode bullet = child.getChildNode(bulletIdx);
				if (SimpleStrCmp(bullet.getName(), "bullet") == true)
				{
					BulletPosAngSpeed pos = BulletPosAngSpeed();
					for (int bulletAttrIdx = 0; bulletAttrIdx < bullet.nAttribute(); bulletAttrIdx++)
					{
						XMLAttribute BulletAttr = bullet.getAttribute(bulletAttrIdx);
						if (SimpleStrCmp(BulletAttr.lpszName, "relativeposition") == true)
						{
							std::string val = ReplaceCharInString(BulletAttr.lpszValue, ',', ' ');
							std::vector<std::string> parsed = ParseString(val);
							if (parsed.size() > 0)
							{
								pos.relPos.y = (float)atof(parsed.at(0).c_str());
							}
							if (parsed.size() > 1)
							{
								pos.relPos.x = (float)atof(parsed.at(1).c_str());
							}
						}
						else if (SimpleStrCmp(BulletAttr.lpszName, "angle") == true)
						{
							pos.angle = (float)atof(BulletAttr.lpszValue);
						}
						else if (SimpleStrCmp(BulletAttr.lpszName, "speed") == true)
						{
							pos.speed = (float)atof(BulletAttr.lpszValue);
						}
						else if (SimpleStrCmp(BulletAttr.lpszName, "angularvelocity") == true)
						{
							pos.angularVelocity = (float)atof(BulletAttr.lpszValue);
						}
						else if (SimpleStrCmp(BulletAttr.lpszName, "collisionscale"))
						{
							Vector2 collisionScale = Vector2::vec2_ones;
							std::string var = ReplaceCharInString(BulletAttr.lpszValue, ',', ' ');
							std::vector<std::string> parsed = ParseString(var);
							if (parsed.size() > 0)
							{
								collisionScale.x = (float)atof(parsed.at(0).c_str());
							}
							if (parsed.size() > 1)
							{
								collisionScale.y = (float)atof(parsed.at(1).c_str());
							}
							pos.collisionScale = (collisionScale);
						}
						else if (SimpleStrCmp(BulletAttr.lpszName, "numtimestoexplode"))
						{
							int numTimesToExplode = atoi(BulletAttr.lpszValue);
							if (numTimesToExplode > 0)
							{
								pos.numTimesExplode = (size_t)numTimesToExplode;
							}
						}
						else if (SimpleStrCmp(BulletAttr.lpszName, "additiveangularvelocityafterexplosion") == true)
						{
							pos.additiveAngularVelocity = (float)atof(BulletAttr.lpszValue);
						}
						else if (SimpleStrCmp(BulletAttr.lpszName, "addedrotationforimage") == true)
						{
							pos.imageaddedrotation = (float)atof(BulletAttr.lpszValue);
						}
						else if (SimpleStrCmp(BulletAttr.lpszName, "secondsafterborntodie") == true)
						{
							pos.secondsAfterBornToDie = (float)atof(BulletAttr.lpszValue);
						}
					}
					bulletConfig->AddBulletPosition(pos);
				}
			}
			if (s_configurations == nullptr)
			{
				s_configurations = new std::map<std::string, BulletConfiguration*>();
			}
			s_configurations->insert(std::pair<std::string, BulletConfiguration*>(name, bulletConfig));
		}
	}
}