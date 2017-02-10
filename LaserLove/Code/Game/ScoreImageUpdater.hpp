#pragma once

#ifndef SCOREIMAGEUPDATER_HPP
#define SCOREIMAGEUPDATER_HPP
#include "Engine/2D/Sprite.hpp"

class ScoreImageUpdater
{
private:
	bool m_visible;
	size_t m_digits;
	std::vector<const SpriteResource*> m_digitResources;
	std::vector<Sprite*> m_sprites;
protected:
public:
	ScoreImageUpdater(size_t digits = 10);
	virtual ~ScoreImageUpdater();

	void Update(size_t playerScore);
	void ToggleVisible(const bool& visible);
};
#endif