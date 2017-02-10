#ifndef INCLUDED_AUDIO
#define INCLUDED_AUDIO
#pragma once

//---------------------------------------------------------------------------
#pragma comment( lib, "Thirdparty/fmod/fmodex_vc" ) // Link in the fmodex_vc.lib static library

//---------------------------------------------------------------------------
//#include "../External/fmod/fmod.hpp"
#include "Thirdparty/fmod/fmod.hpp"
#include <string>
#include <vector>
#include <map>


//---------------------------------------------------------------------------
typedef unsigned int SoundID;
typedef FMOD::Channel* AudioChannelHandle;
const unsigned int MISSING_SOUND_ID = 0xffffffff;


/////////////////////////////////////////////////////////////////////////////
class AudioSystem
{
public:
	AudioSystem();
	virtual ~AudioSystem();
	const SoundID CreateOrGetSound(const std::string& soundFileName, const bool& loop = false);
	//---------------------------------------------------------------------------
	void StopChannel(AudioChannelHandle channel);
	const bool GetIsPlaying(AudioChannelHandle channel) const;
	AudioChannelHandle PlaySound(const SoundID& soundID, const float& volumeLevel = 1.f);
	void Update(const float& deltaSeconds); // Must be called at regular intervals (e.g. every frame)

protected:
	void InitializeFMOD();
	void ValidateResult(const FMOD_RESULT& result);

protected:
	FMOD::System*						m_fmodSystem;
	std::map< std::string, SoundID >	m_registeredSoundIDs;
	std::vector< FMOD::Sound* >			m_registeredSounds;
};


//---------------------------------------------------------------------------
void InitializeAudio();


#endif // INCLUDED_AUDIO
