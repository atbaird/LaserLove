#include "Game/SpriteLoader/SpriteLoader.hpp"
#include "Engine/2D/SpriteDatabase.hpp"
#include "Game/SpriteLoader/ResourceNames.hpp"

void SpriteLoader::LoadAllSpritesAsNew()
{
	//SpriteDatabase::g_SpriteDatabase->TryLoadAllSpriteResourcesFromXML(SpriteDatabase::s_CommonSpriteDatabaseSaveLocation);

	//--------------------------------------------------
	//Load in all Textures
	Texture* m_redSmiley = Texture::CreateOrGetTexture("Data/Images/Red Smiley.png");
	Texture* bulletCollection1 = Texture::CreateOrGetTexture("Data/Images/M484BulletCollection1.png");
	Texture* shipCollection1 = Texture::CreateOrGetTexture("Data/Images/ships.png");
	Texture* backgroundTile1 = Texture::CreateOrGetTexture("Data/Images/361-0.png");
	Texture* menuTitle = Texture::CreateOrGetTexture("Data/Images/Text_Laser_Love.png");
	Texture* pressEsc = Texture::CreateOrGetTexture("Data/Images/Text_Press_Esc_To_Quit.png");
	Texture* pressSpace = Texture::CreateOrGetTexture("Data/Images/Text_Press_Space_To_Start.png");
	Texture* whiteSpace = Texture::CreateOrGetTexture("Data/Images/whitespace.png");
	Texture* borderOutline = Texture::CreateOrGetTexture("Data/Images/Outline-Square.png");
	Texture* zero = Texture::CreateOrGetTexture("Data/Images/Numbers/0.png");
	Texture* one = Texture::CreateOrGetTexture("Data/Images/Numbers/1.png");
	Texture* two = Texture::CreateOrGetTexture("Data/Images/Numbers/2.png");
	Texture* three = Texture::CreateOrGetTexture("Data/Images/Numbers/3.png");
	Texture* four = Texture::CreateOrGetTexture("Data/Images/Numbers/4.png");
	Texture* five = Texture::CreateOrGetTexture("Data/Images/Numbers/5.png");
	Texture* six = Texture::CreateOrGetTexture("Data/Images/Numbers/6.png");
	Texture* seven = Texture::CreateOrGetTexture("Data/Images/Numbers/7.png");
	Texture* eight = Texture::CreateOrGetTexture("Data/Images/Numbers/8.png");
	Texture* nine = Texture::CreateOrGetTexture("Data/Images/Numbers/9.png");
	Texture* press = Texture::CreateOrGetTexture("Data/Images/Press.png");
	Texture* through = Texture::CreateOrGetTexture("Data/Images/through.png");
	Texture* toselectAShip = Texture::CreateOrGetTexture("Data/Images/To Select a Ship.png");

	//--------------------------------------------------
	//Menu
	SpriteResourceList::RegisterSpriteResource(ResourceNames::s_MenuPressEsc, LoadInSprite(pressEsc, 0, AABB2::ZERO_TO_ONE, Vector2::vec2_ones));
	SpriteResourceList::RegisterSpriteResource(ResourceNames::s_MenuPressSpace, LoadInSprite(pressSpace, 0, AABB2::ZERO_TO_ONE, Vector2::vec2_ones));
	SpriteResourceList::RegisterSpriteResource(ResourceNames::s_MenuTitle, LoadInSprite(menuTitle, 0, AABB2::ZERO_TO_ONE, Vector2::vec2_ones));
	SpriteResourceList::RegisterSpriteResource(ResourceNames::s_whiteSpace, LoadInSprite(whiteSpace, 0, AABB2::ZERO_TO_ONE, Vector2::vec2_ones));
	

	//--------------------------------------------------
	//Ship Select
	SpriteResourceList::RegisterSpriteResource(ResourceNames::s_whiteOutline, LoadInSprite(borderOutline, 0, AABB2::ZERO_TO_ONE, Vector2::vec2_ones));
	SpriteResourceList::RegisterSpriteResource(ResourceNames::s_zero, LoadInSprite(zero, 0, AABB2::ZERO_TO_ONE, Vector2::vec2_ones));
	SpriteResourceList::RegisterSpriteResource(ResourceNames::s_one, LoadInSprite(one, 0, AABB2::ZERO_TO_ONE, Vector2::vec2_ones));
	SpriteResourceList::RegisterSpriteResource(ResourceNames::s_two, LoadInSprite(two, 0, AABB2::ZERO_TO_ONE, Vector2::vec2_ones));
	SpriteResourceList::RegisterSpriteResource(ResourceNames::s_three, LoadInSprite(three, 0, AABB2::ZERO_TO_ONE, Vector2::vec2_ones));
	SpriteResourceList::RegisterSpriteResource(ResourceNames::s_four, LoadInSprite(four, 0, AABB2::ZERO_TO_ONE, Vector2::vec2_ones));
	SpriteResourceList::RegisterSpriteResource(ResourceNames::s_five, LoadInSprite(five, 0, AABB2::ZERO_TO_ONE, Vector2::vec2_ones));
	SpriteResourceList::RegisterSpriteResource(ResourceNames::s_six, LoadInSprite(six, 0, AABB2::ZERO_TO_ONE, Vector2::vec2_ones));
	SpriteResourceList::RegisterSpriteResource(ResourceNames::s_seven, LoadInSprite(seven, 0, AABB2::ZERO_TO_ONE, Vector2::vec2_ones));
	SpriteResourceList::RegisterSpriteResource(ResourceNames::s_eight, LoadInSprite(eight, 0, AABB2::ZERO_TO_ONE, Vector2::vec2_ones));
	SpriteResourceList::RegisterSpriteResource(ResourceNames::s_nine, LoadInSprite(nine, 0, AABB2::ZERO_TO_ONE, Vector2::vec2_ones));
	SpriteResourceList::RegisterSpriteResource(ResourceNames::s_Press, LoadInSprite(press, 0, AABB2::ZERO_TO_ONE, Vector2::vec2_ones));
	SpriteResourceList::RegisterSpriteResource(ResourceNames::s_through, LoadInSprite(through, 0, AABB2::ZERO_TO_ONE, Vector2::vec2_ones));
	SpriteResourceList::RegisterSpriteResource(ResourceNames::s_ToSelectAShip, LoadInSprite(toselectAShip, 0, AABB2::ZERO_TO_ONE, Vector2::vec2_ones));

	//--------------------------------------------------
	//RedSmiley
	SpriteResourceList::RegisterSpriteResource(ResourceNames::s_redSmiley, LoadInSprite(m_redSmiley, 0, AABB2::ZERO_TO_ONE, Vector2::vec2_ones));
	
	//--------------------------------------------------
	//Bullet Collection 1
	SpriteResourceList::RegisterSpriteResource(ResourceNames::s_wholeBulletCollection1, LoadInSprite(bulletCollection1, 0, AABB2::ZERO_TO_ONE, Vector2::vec2_ones));

	//!-----------
	//defaultBullet
	const SpriteResource* bullet1Sprite1 = LoadInSprite(bulletCollection1, 1, AABB2(Vector2(0.02307f, 0.03047f), Vector2(0.03653f, 0.0609f)), Vector2(0.2f, 0.2f));
	const SpriteResource* bullet1Sprite2 = LoadInSprite(bulletCollection1, 2, AABB2(Vector2(0.08269f, 0.03047f), Vector2(0.10192f, 0.0609f)), Vector2(0.2f, 0.2f));
	const SpriteResource* bullet1Sprite3 = LoadInSprite(bulletCollection1, 3, AABB2(Vector2(0.03846f, 0.03047f), Vector2(0.05961f, 0.0609f)), Vector2(0.2f, 0.2f));
	const SpriteResource* bullet1Sprite4 = LoadInSprite(bulletCollection1, 4, AABB2(Vector2(0.06153f, 0.03047f), Vector2(0.08076f, 0.0609f)), Vector2(0.2f, 0.2f));
	SpriteAnimationSequence* bullet1Sequence = new SpriteAnimationSequence(MOTION_WRAPMODE_LOOP);
	frame_t bullet1Frame1 = frame_t(bullet1Sprite1, 0.15f);
	frame_t bullet1Frame2 = frame_t(bullet1Sprite2, 0.15f);
	frame_t bullet1Frame3 = frame_t(bullet1Sprite3, 0.15f);
	frame_t bullet1Frame4 = frame_t(bullet1Sprite4, 0.15f);
	bullet1Sequence->PushBackFrame(bullet1Frame1);
	bullet1Sequence->PushBackFrame(bullet1Frame2);
	bullet1Sequence->PushBackFrame(bullet1Frame3);
	bullet1Sequence->PushBackFrame(bullet1Frame4);
	SpriteResourceList::RegisterSpriteAnimationSequence(ResourceNames::s_defaultBullet, bullet1Sequence);

	//!-----------
	//bulletAnim1
	const SpriteResource* bullet2Sprite1 = LoadInSprite(bulletCollection1, 5, AABB2(Vector2(0.0211f, 0.2576f), Vector2(0.02884f, 0.2991f)), Vector2(0.2f, 0.2f));
	const SpriteResource* bullet2Sprite2 = LoadInSprite(bulletCollection1, 6, AABB2(Vector2(0.03461f, 0.2576f), Vector2(0.04615f, 0.2991f)), Vector2(0.2f, 0.2f));
	const SpriteResource* bullet2Sprite3 = LoadInSprite(bulletCollection1, 7, AABB2(Vector2(0.05f, 0.2576f), Vector2(0.0653f, 0.2991f)), Vector2(0.2f, 0.2f));
	const SpriteResource* bullet2Sprite4 = LoadInSprite(bulletCollection1, 8, AABB2(Vector2(0.0692f, 0.2576f), Vector2(0.0826f, 0.2991f)), Vector2(0.2f, 0.2f));
	const SpriteResource* bullet2Sprite5 = LoadInSprite(bulletCollection1, 9, AABB2(Vector2(0.0884f, 0.2576f), Vector2(0.1057f, 0.2991f)), Vector2(0.2f, 0.2f));
	const SpriteResource* bullet2Sprite6 = LoadInSprite(bulletCollection1, 10, AABB2(Vector2(0.1096f, 0.2576f), Vector2(0.1288f, 0.2991f)), Vector2(0.2f, 0.2f));
	const SpriteResource* bullet2Sprite7 = LoadInSprite(bulletCollection1, 11, AABB2(Vector2(0.1326f, 0.2576f), Vector2(0.1538f, 0.2991f)), Vector2(0.2f, 0.2f));
	const SpriteResource* bullet2Sprite8 = LoadInSprite(bulletCollection1, 12, AABB2(Vector2(0.1576f, 0.2576f), Vector2(0.1807f, 0.2991f)), Vector2(0.2f, 0.2f));
	const SpriteResource* bullet2Sprite9 = LoadInSprite(bulletCollection1, 13, AABB2(Vector2(0.1846f, 0.2576f), Vector2(0.2096f, 0.2991f)), Vector2(0.2f, 0.2f));
	const SpriteResource* bullet2Sprite10 = LoadInSprite(bulletCollection1, 14, AABB2(Vector2(0.2134f, 0.2576f), Vector2(0.2403f, 0.2991f)), Vector2(0.2f, 0.2f));
	const SpriteResource* bullet2Sprite11 = LoadInSprite(bulletCollection1, 15, AABB2(Vector2(0.2442f, 0.2576f), Vector2(0.2730f, 0.2991f)), Vector2(0.2f, 0.2f));
	frame_t bullet2Frame1 = frame_t(bullet2Sprite1, 0.1f);
	frame_t bullet2Frame2 = frame_t(bullet2Sprite2, 0.1f);
	frame_t bullet2Frame3 = frame_t(bullet2Sprite3, 0.1f);
	frame_t bullet2Frame4 = frame_t(bullet2Sprite4, 0.1f);
	frame_t bullet2Frame5 = frame_t(bullet2Sprite5, 0.1f);
	frame_t bullet2Frame6 = frame_t(bullet2Sprite6, 0.1f);
	frame_t bullet2Frame7 = frame_t(bullet2Sprite7, 0.1f);
	frame_t bullet2Frame8 = frame_t(bullet2Sprite8, 0.1f);
	frame_t bullet2Frame9 = frame_t(bullet2Sprite9, 0.1f);
	frame_t bullet2Frame10 = frame_t(bullet2Sprite10, 0.1f);
	frame_t bullet2Frame11 = frame_t(bullet2Sprite11, 0.1f);
	SpriteAnimationSequence* bullet2Sequence = new SpriteAnimationSequence();
	bullet2Sequence->PushBackFrame(bullet2Frame1);
	bullet2Sequence->PushBackFrame(bullet2Frame2);
	bullet2Sequence->PushBackFrame(bullet2Frame3);
	bullet2Sequence->PushBackFrame(bullet2Frame4);
	bullet2Sequence->PushBackFrame(bullet2Frame5);
	bullet2Sequence->PushBackFrame(bullet2Frame6);
	bullet2Sequence->PushBackFrame(bullet2Frame7);
	bullet2Sequence->PushBackFrame(bullet2Frame8);
	bullet2Sequence->PushBackFrame(bullet2Frame9);
	bullet2Sequence->PushBackFrame(bullet2Frame10);
	bullet2Sequence->PushBackFrame(bullet2Frame11);
	SpriteResourceList::RegisterSpriteAnimationSequence(ResourceNames::s_explosionBulletAnim, bullet2Sequence);

	//!------------
	//bulletAnim2
	const SpriteResource* bullet3Sprite1 = LoadInSprite(bulletCollection1, 5, AABB2(Vector2(0.4038f, 0.0166f), Vector2(0.4269f, 0.0498f)), Vector2(0.5f, 0.5f));
	const SpriteResource* bullet3Sprite2 = LoadInSprite(bulletCollection1, 6, AABB2(Vector2(0.4365f, 0.0166f), Vector2(0.4673f, 0.0498f)), Vector2(0.5f, 0.5f));
	const SpriteResource* bullet3Sprite3 = LoadInSprite(bulletCollection1, 7, AABB2(Vector2(0.4769f, 0.0166f), Vector2(0.5134f, 0.0498f)), Vector2(0.5f, 0.5f));
	const SpriteResource* bullet3Sprite4 = LoadInSprite(bulletCollection1, 8, AABB2(Vector2(0.5269f, 0.0166f), Vector2(0.5673f, 0.0498f)), Vector2(0.5f, 0.5f));
	frame_t bullet3Frame1 = frame_t(bullet3Sprite1, 0.1f);
	frame_t bullet3Frame2 = frame_t(bullet3Sprite2, 0.1f);
	frame_t bullet3Frame3 = frame_t(bullet3Sprite3, 0.1f);
	frame_t bullet3Frame4 = frame_t(bullet3Sprite4, 0.1f);
	SpriteAnimationSequence* bullet3Sequence = new SpriteAnimationSequence();
	bullet3Sequence->PushBackFrame(bullet3Frame1);
	bullet3Sequence->PushBackFrame(bullet3Frame2);
	bullet3Sequence->PushBackFrame(bullet3Frame3);
	bullet3Sequence->PushBackFrame(bullet3Frame4);
	SpriteResourceList::RegisterSpriteAnimationSequence(ResourceNames::s_spiralExplosionBulletAnim, bullet3Sequence);

	//--------------------------------------------------
	//ShipCollection
	SpriteResourceList::RegisterSpriteResource(ResourceNames::s_wholeShipCollection1, LoadInSprite(shipCollection1, 0, AABB2::ZERO_TO_ONE, Vector2::vec2_ones));
	SpriteResourceList::RegisterSpriteResource(ResourceNames::s_playerShip, LoadInSprite(shipCollection1, 1, AABB2(Vector2(0.125f, 0.0213f), Vector2(0.375f, 0.0516f)), Vector2(1.f, 0.15f)));
	SpriteResourceList::RegisterSpriteResource(ResourceNames::s_enemyShip1, LoadInSprite(shipCollection1, 2, AABB2(Vector2(0.0585f, 0.3158f), Vector2(0.4394f, 0.3668f)), Vector2(1.f, 0.15f)));
	SpriteResourceList::RegisterSpriteResource(ResourceNames::s_enemyShip2, LoadInSprite(shipCollection1, 3, AABB2(Vector2(0.1523f, 0.7169f), Vector2(0.3476f, 0.74338f)), Vector2(1.f, 0.15f)));
	SpriteResourceList::RegisterSpriteResource(ResourceNames::s_enemyShip3, LoadInSprite(shipCollection1, 4, AABB2(Vector2(0.1210f, 0.0955f), Vector2(0.3769f, 0.1364f)), Vector2(1.f, 0.15f)));

	//--------------------------------------------------
	//Background Tile1
	SpriteResourceList::RegisterSpriteResource(ResourceNames::s_backgroundTile1, LoadInSprite(backgroundTile1, 0, AABB2::ZERO_TO_ONE, Vector2::vec2_ones));
	
	SpriteDatabase::g_SpriteDatabase->SaveOutAllSpriteResourcesToXML();
}

const SpriteResource* SpriteLoader::LoadInSprite(Texture* tex, const size_t& spriteID, const AABB2& spriteCoords, const Vector2& virtualSize)
{
	if (tex == nullptr)
	{
		return nullptr;
	}
	const SpriteResource* toReturn = nullptr;
	bool foundIt = false;
	size_t spriteResourceID = SpriteDatabase::g_SpriteDatabase->GetSpriteResourceIDForSpriteSheetAndSpriteID(tex, spriteID, foundIt);
	if (foundIt)
	{
		toReturn = SpriteDatabase::g_SpriteDatabase->GetSprite(spriteResourceID);
	}
	else
	{
		SpriteResource* redSmileyResource = new SpriteResource(tex, spriteCoords, spriteID, virtualSize);
		SpriteDatabase::g_SpriteDatabase->RegisterNewSprite(tex, spriteID, redSmileyResource);
		toReturn = redSmileyResource;
	}
	return toReturn;
}