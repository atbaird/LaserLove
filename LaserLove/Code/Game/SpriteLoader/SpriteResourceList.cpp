#include "Game/SpriteLoader/SpriteResourceList.hpp"
#include "Engine/Core/StringUtils.hpp"

std::map<std::string, const SpriteResource*>* SpriteResourceList::s_Resources = nullptr;
std::map<std::string, SpriteAnimationSequence*>* SpriteResourceList::s_Sequences = nullptr;

void SpriteResourceList::RegisterSpriteResource(const std::string& name, const SpriteResource* resource)
{
	if (SimpleStrCmp(name, "") == true || resource == nullptr)
	{
		return;
	}
	if (s_Resources == nullptr)
	{
		s_Resources = new std::map<std::string, const SpriteResource*>();
	}

	s_Resources->insert(std::pair<std::string, const SpriteResource*>(name, resource));
}
void SpriteResourceList::RegisterSpriteAnimationSequence(const std::string& name, SpriteAnimationSequence* sequence)
{
	if (SimpleStrCmp(name, "") == true || sequence == nullptr)
	{
		return;
	}
	if (s_Sequences == nullptr)
	{
		s_Sequences = new std::map<std::string, SpriteAnimationSequence*>();
	}
	s_Sequences->insert(std::pair<std::string, SpriteAnimationSequence*>(name, sequence));
}
const SpriteResource* SpriteResourceList::GetSpriteResourceByName(const std::string& name)
{
	if (s_Resources == nullptr)
	{
		return nullptr;
	}
	std::map<std::string, const SpriteResource*>::const_iterator it = s_Resources->find(name);
	if (it == s_Resources->end())
	{
		return nullptr;
	}
	return it->second;
}
SpriteAnimationSequence* SpriteResourceList::GetSpriteAnimationSequenceByName(const std::string& name)
{
	if (s_Sequences == nullptr)
	{
		return nullptr;
	}
	std::map<std::string, SpriteAnimationSequence*>::const_iterator it = s_Sequences->find(name);
	if (it == s_Sequences->end())
	{
		return nullptr;
	}
	return it->second;
}
const size_t SpriteResourceList::GetResourceIDByName(const std::string& name, bool& foundIt)
{
	foundIt = false;
	if (s_Resources == nullptr)
	{
		return 0;
	}
	std::map<std::string, const SpriteResource*>::const_iterator it = s_Resources->find(name);
	if (it == s_Resources->end())
	{
		return 0;
	}
	foundIt = true;
	return it->second->GetSpriteResourceID();
}
void SpriteResourceList::UnregisterSpriteResource(const SpriteResource* resource)
{
	if (s_Resources == nullptr)
	{
		return;
	}
	for (std::map<std::string, const SpriteResource*>::const_iterator it = s_Resources->begin(); it != s_Resources->end(); it++)
	{
		if (it->second == resource)
		{
			s_Resources->erase(it);
			return;
		}
	}
}
void SpriteResourceList::UnregisterSpriteResourceByName(const std::string& name)
{
	if (s_Resources == nullptr)
	{
		return;
	}
	std::map<std::string, const SpriteResource*>::const_iterator it = s_Resources->find(name);
	if (it == s_Resources->end())
	{
		return;
	}
	s_Resources->erase(it);
}
void SpriteResourceList::UnregisterSpriteAnimationSequence(SpriteAnimationSequence* sequence)
{
	if (s_Sequences == nullptr)
	{
		return;
	}
	for (std::map<std::string, SpriteAnimationSequence*>::const_iterator it = s_Sequences->begin(); it != s_Sequences->end(); it++)
	{
		if (it->second == sequence)
		{
			s_Sequences->erase(it);
			return;
		}
	}
}

void SpriteResourceList::UnregisterSpriteANimationSequenceByName(const std::string& name)
{
	if (s_Sequences == nullptr)
	{
		return;
	}
	std::map<std::string, SpriteAnimationSequence*>::const_iterator it = s_Sequences->find(name);
	if (it == s_Sequences->end())
	{
		return;
	}
	s_Sequences->erase(it);
}
void SpriteResourceList::ClearAll()
{
	if (s_Resources != nullptr)
	{
		s_Resources->clear();
		delete s_Resources;
		s_Resources = nullptr;
	}
	if (s_Sequences != nullptr)
	{
		for (std::map<std::string, SpriteAnimationSequence*>::iterator it = s_Sequences->begin(); it != s_Sequences->end(); it++)
		{
			SpriteAnimationSequence* sequence = it->second;
			if (sequence != nullptr)
			{
				delete sequence;
				sequence = nullptr;
			}
		}
		s_Sequences->clear();
		delete s_Sequences;
		s_Sequences = nullptr;
	}
}