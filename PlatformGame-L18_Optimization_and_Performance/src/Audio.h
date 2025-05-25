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

	bool StopMusic();

private:

	_Mix_Music* music;
	std::vector<SFX*> fx;
};