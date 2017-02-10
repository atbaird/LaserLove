#pragma once

#ifndef HUD_HPP
#define HUD_HPP
#include "Game/RealGameState.hpp"
#include "Engine/Math/Vector2.hpp"
#include "Engine/2D/SpriteResource.hpp"
#include <vector>

class ShaderProgram;
class Sprite;

class HUD
{
private:
	ShaderProgram* m_prog;
	std::vector<Sprite*> m_widgets;
	const SpriteResource* m_resource;
	static const Vector2 s_healthBoxSize;
	static const Vector2 s_foregroundHealthBoxDecrease;
	static const Vector2 s_localShiftOver;
	static const Vector2 s_healthBoxStart;
	Vector2 m_overallHealthBoxPercentage;
	Vector2 m_blHealthBoxForegroundStart;
	bool m_loaded;
public:
	//Constructors
	HUD(ShaderProgram* prog = nullptr);
	virtual ~HUD();

	//Update and Render
	void Update(const float& deltaSeconds, const gameState& currentState);

	//Setters
	void SetWidgets(const std::vector<Sprite*>& widgets, const bool& setShaderProgram = true);
	void AddWidget(Sprite* widget, const bool& setShaderProgram = true);
	void SetShaderProgram(ShaderProgram* prog);
	void LoadSprites();
	void ClearSprites();

	//Getters
	const std::vector<Sprite*> GetWidgets() const;
	const ShaderProgram* GetShaderProgram() const;
	ShaderProgram* GetEditableShaderProgram() const;
};
#endif