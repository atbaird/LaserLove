#pragma once


#ifndef SPRITELOADER_HPP
#define SPRITELOADER_HPP
#include "Engine/2D/SpriteResourceHandler.hpp"
#include "Engine/Core/EngineXMLParser.hpp"

#define STATIC 
class SpriteLoader
{
public:
	static const char* s_SpriteResourceRootXMLNode;
	static const char* s_SpriteResourceChildXMLNode;

	static const char* s_SpriteAnimationRootXMLNode;
	static const char* s_SpriteAnimationChildXMLNode;

	static const char* s_resourceName;
	static const char* s_textureLocation;
	static const char* s_spriteID;
	static const char* s_spriteBounds;
	static const char* s_virtualSize;

	static const char* s_SpriteAnimationName;
	static const char* s_defaultTimeIntervalBetweenFrames;
	static const char* s_MotionMode;
	static const char* s_ReverseDirection;
	static const char* s_SpriteResource;
	static const char* s_currentSpriteTimeInterval;
	/*

			//load in spriteResource
			//load in duration if applicable.
	*/
	/*

		//load default interval for frames.
		//load motionMode.
		//load whether should be reading in reverse direction.
	*/

	//Public Static methods
	static void LoadAllSpritesResourcesFromFolderAsNew(const char* spriteResourcesXMLfolderLoc, bool recurseChildren = false);
	static void LoadAllSpriteAnimationsFromFolderAsNew(const char* spriteAnimationXMLfolderLoc, bool recurseChildren = false);
	static void LoadAllSpritesResourcesAsNew(const char* spriteResourcesXMLfileLoc);
	static void LoadAllSpriteAnimationsAsNew(const char* spriteAnimationXMLfileLoc);
	static const SpriteResource* LoadInSprite(Texture* tex, const size_t& spriteID, const AABB2& spriteCoords, const Vector2& virtualSize);

private:
	//TryLoadInSpriteResource
	static bool TryLoadInSpriteResource(XMLNode& child);
	static void TryLoadInSpriteResourceReadAttributes(XMLNode& child, bool& resourceNameSet, bool& textureLocationSet,
		std::string& resourceName, std::string& textureLocation, size_t& spriteID, AABB2& spriteBounds, Vector2& virtualSize);
	static bool TryLoadInSpriteResourceTestValid(bool& resourceNameSet, bool& textureLocationSet, bool& spriteIDIsAvailable,
		std::string& resourceName, std::string& textureLocation, size_t& spriteID, AABB2& spriteBounds, Vector2& virtualSize);

	static bool TryLoadInSpriteAnimation(XMLNode& child);
	static void TryLoadInSpriteAnimationReadAttributes(XMLNode& child, std::string& name, std::vector<frame_t>& frames,
		Motion_WrapModes& mode, float& end_time, bool& flipped);
	static bool TryLoadInSpriteAnimationTestValid(std::string& name, std::vector<frame_t>& frames, Motion_WrapModes& mode,
		float& end_time, bool& flipped);
};
#endif