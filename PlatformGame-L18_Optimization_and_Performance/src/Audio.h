#pragma once

#include "Module.h"
#include "SDL2/SDL_mixer.h"
#include <vector>

#define DEFAULT_MUSIC_FADE_TIME 2.0f

struct _Mix_Music;
enum class Effects
{
	CHASE,
	COUGHF1,
	COUGHF2,
	COUGHF3,
	EAT1,
	EAT2,
	ENEMY_HIT,
	EQUIP,
	HIT1,
	HIT2,
	HIT3,
	PICKUP1,
	PICKUP2,
	PICKUP3,
	SAM_EXHAUST,
	WLAKING,
	UICLICK,
	UNKNOWN // Default value for uninitialized effects
};

class SFX
{
public:

	Mix_Chunk* sound = nullptr;
	Effects typeFx;

};

class Audio : public Module
{
public:

	Audio();

	// Destructor
	virtual ~Audio();

	// Called before render is available
	bool Awake();

	bool Start();

	// Called before quitting
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fadeTime = DEFAULT_MUSIC_FADE_TIME);

	// Load a WAV in memory
	int LoadFx(const char* path, Effects type);

	// Play a previously loaded WAV
	bool PlayFx(Effects type, int channel = -1, int repeat = 0);
	void PlayRandFx(Effects type1 = Effects::UNKNOWN, Effects type2 = Effects::UNKNOWN, Effects type3 = Effects::UNKNOWN, int channel = -1);

	bool StopFx(Effects type, int channel);

	// Function to reduce volume with percentatges
	void ChangeGlobalVolume(float percentage);
	void ChangeGeneralVolume(float percentage);
	void ChangeSfxVolume(float percentage);
	void ChangeMusicVolume(float percentage);

	// Getters
	float GetGeneralVolume() const { return general; }
	float GetSfxVolume() const { return sfxVolume; }
	float GetMusicVolume() const { return musicVolume; }

	bool StopMusic();

private:

	_Mix_Music* music;
	std::vector<SFX*> fx;

	float general = 1.0f; // General volume multiplier
	float sfxVolume = 1.0f; // Default volume for SFX
	float musicVolume = 1.0f; // Default volume for music
};