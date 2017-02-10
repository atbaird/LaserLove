#include "Engine/2D/SpriteResourceHandler.hpp"
#include "Engine/Core/StringUtils.hpp"

std::map<std::string, const SpriteResource*>* SpriteResourceHandler::s_Resources = nullptr;
std::map<std::string, SpriteAnimationSequence>* SpriteResourceHandler::s_Sequences = nullptr;

STATIC void SpriteResourceHandler::RegisterSpriteResource(const std::string& name, const SpriteResource* resource)
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

STATIC void SpriteResourceHandler::RegisterSpriteAnimationSequence(const std::string& name, const SpriteAnimationSequence& sequence)
{
	if (SimpleStrCmp(name, "") == true)
	{
		return;
	}
	if (s_Sequences == nullptr)
	{
		s_Sequences = new std::map<std::string, SpriteAnimationSequence>();
	}
	s_Sequences->insert(std::pair<std::string, SpriteAnimationSequence>(name, sequence));
}

STATIC const SpriteResource* SpriteResourceHandler::GetSpriteResourceByName(const std::string& name)
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

STATIC SpriteAnimationSequence* SpriteResourceHandler::GetSpriteAnimationSequenceByName(const std::string& name)
{
	if (s_Sequences == nullptr)
	{
		return nullptr;
	}
	std::map<std::string, SpriteAnimationSequence>::const_iterator it = s_Sequences->find(name);
	if (it == s_Sequences->end())
	{
		return nullptr;
	}
	SpriteAnimationSequence* sequence = const_cast<SpriteAnimationSequence*>(&it->second);
	return sequence;
}

STATIC const size_t SpriteResourceHandler::GetResourceIDByName(const std::string& name, bool& foundIt)
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

STATIC const bool SpriteResourceHandler::GetIfNameAlreadyExistsSpriteResource(std::string& name)
{
	if (s_Resources == nullptr || s_Resources->find(name) == s_Resources->end())
	{
		return false;
	}
	return true;
}

STATIC const bool SpriteResourceHandler::GetIfNameAlreadyExistsSpriteAnimationResource(std::string& name)
{
	if (s_Sequences == nullptr || s_Sequences->find(name) == s_Sequences->end())
	{
		return false;
	}
	return true;
}


STATIC void SpriteResourceHandler::UnregisterSpriteResource(const SpriteResource* resource)
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

STATIC void SpriteResourceHandler::UnregisterSpriteResourceByName(const std::string& name)
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

STATIC void SpriteResourceHandler::UnregisterSpriteAnimationSequence(SpriteAnimationSequence* sequence)
{
	if (s_Sequences == nullptr)
	{
		return;
	}
	for (std::map<std::string, SpriteAnimationSequence>::const_iterator it = s_Sequences->begin(); it != s_Sequences->end(); it++)
	{
		if (&it->second == sequence)
		{
			s_Sequences->erase(it);
			return;
		}
	}
}

STATIC void SpriteResourceHandler::UnregisterSpriteANimationSequenceByName(const std::string& name)
{
	if (s_Sequences == nullptr)
	{
		return;
	}
	std::map<std::string, SpriteAnimationSequence>::const_iterator it = s_Sequences->find(name);
	if (it == s_Sequences->end())
	{
		return;
	}
	s_Sequences->erase(it);
}

STATIC void SpriteResourceHandler::ClearAll()
{
	if (s_Resources != nullptr)
	{
		s_Resources->clear();
		delete s_Resources;
		s_Resources = nullptr;
	}
	if (s_Sequences != nullptr)
	{
		s_Sequences->clear();
		delete s_Sequences;
		s_Sequences = nullptr;
	}
}