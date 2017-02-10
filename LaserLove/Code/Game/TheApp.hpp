#pragma once
#include "Game/TheGame.hpp"
#include "Engine/Core/Performance/Profiler.hpp"

#ifndef THEAPP_HPP
#define THEAPP_HPP
//-----------------------------------------------------
class TheApp;

//-----------------------------------------------------
extern TheApp* g_TheApp;

//-----------------------------------------------------
class TheApp
{
public:
	TheApp();
	~TheApp();
	void Update(const float& deltaSeconds);
	void Render() const;
	const bool isQuitting() const;
	void OnGainedFocus() { m_focus = true; };
	void OnLostFocus() { m_focus = false; };
	void SetUpFrameBuffer();


	Font* m_font;

private:
	bool m_focus;
	bool isTildeHeldDown;
	bool m_useFrameBuffer;
	FrameBuffer* m_frameBuffer = nullptr;
	ShaderProgram* m_ScreenPassThrough = nullptr;
	Mesh* myMesh = nullptr;
	Material* myMat = nullptr;
	AudioSystem* m_audio;
	AudioChannelHandle m_handle;
	static const char* s_music;
	SoundID m_musicID;
};

#endif // !THEAPP_HPP


