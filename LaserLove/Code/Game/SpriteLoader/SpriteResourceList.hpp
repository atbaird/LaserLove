#pragma once

#ifndef SPRITERESOURCELIST_HPP
#define SPRITERESOURCELIST_HPP
#include "Engine/2D/SpriteResource.hpp"
#include "Engine/2D/SpriteAnimationSequence.hpp"
#include "Game/SpriteLoader/ResourceNames.hpp"

class SpriteResourceList
{
	static std::map<std::string, const SpriteResource*>* s_Resources;
	static std::map<std::string, SpriteAnimationSequence*>* s_Sequences;
public:
	static void RegisterSpriteResource(const std::string& name, const SpriteResource* resource);
	static void RegisterSpriteAnimationSequence(const std::string& name, SpriteAnimationSequence* sequence);
	static const SpriteResource* GetSpriteResourceByName(const std::string& name);
	static SpriteAnimationSequence* GetSpriteAnimationSequenceByName(const std::string& name);
	static const size_t GetResourceIDByName(const std::string& name, bool& foundIt);
	static void UnregisterSpriteResource(const SpriteResource* resource);
	static void UnregisterSpriteResourceByName(const std::string& name);
	static void UnregisterSpriteAnimationSequence(SpriteAnimationSequence* sequence);
	static void UnregisterSpriteANimationSequenceByName(const std::string& name);
	static void ClearAll();
};
#endif