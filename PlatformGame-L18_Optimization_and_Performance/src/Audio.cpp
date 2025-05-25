#include "Audio.h"
#include "Log.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"

Audio::Audio() : Module()
{
	music = NULL;
	name = "audio";
}

// Destructor
Audio::~Audio()
{}

// Called before render is available
bool Audio::Awake()
{
	LOG("Loading Audio Mixer");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_AUDIO) < 0)
	{
		LOG("SDL_INIT_AUDIO could not initialize! SDL_Error: %s\n", SDL_GetError());
		active = false;
		ret = true;
	}

	// Load support for the JPG and PNG image formats
	int flags = MIX_INIT_OGG;
	int init = Mix_Init(flags);

	if((init & flags) != flags)
	{
		LOG("Could not initialize Mixer lib. Mix_Init: %s", Mix_GetError());
		active = false;
		ret = true;
	}

	// Initialize SDL_mixer
	if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		LOG("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		active = false;
		ret = true;
	}


	return ret;
}

// Called before quitting
bool Audio::CleanUp()
{
	if(!active)
		return true;

	LOG("Freeing sound FX, closing Mixer and Audio subsystem");

	if(music != NULL)
	{
		Mix_FreeMusic(music);
	}

	for (const auto& fxItem : fx) {
		Mix_FreeChunk(fxItem->sound);
	}
	fx.clear();

	Mix_CloseAudio();
	Mix_Quit();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);

	return true;
}

// Play a music file
bool Audio::PlayMusic(const char* path, float fadeTime)
{
	bool ret = true;

	if(!active)
		return false;

	if(music != NULL)
	{
		if(fadeTime > 0.0f)
		{
			Mix_FadeOutMusic(int(fadeTime * 1000.0f));
		}
		else
		{
			Mix_HaltMusic();
		}

		// this call blocks until fade out is done
		Mix_FreeMusic(music);
	}

	music = Mix_LoadMUS(path);

	if(music == NULL)
	{
		LOG("Cannot load music %s. Mix_GetError(): %s\n", path, Mix_GetError());
		ret = false;
	}
	else
	{
		if(fadeTime > 0.0f)
		{
			if(Mix_FadeInMusic(music, -1, (int) (fadeTime * 1000.0f)) < 0)
			{
				LOG("Cannot fade in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
		else
		{
			if(Mix_PlayMusic(music, -1) < 0)
			{
				LOG("Cannot play in music %s. Mix_GetError(): %s", path, Mix_GetError());
				ret = false;
			}
		}
	}

	LOG("Successfully playing %s", path);
	return ret;
}

// Load WAV
int Audio::LoadFx(const char* path, Effects type)
{
	int ret = 0;

	if(!active)
		return 0;

	Mix_Chunk* chunk = Mix_LoadWAV(path);

	if(chunk == NULL)
	{
		LOG("Cannot load wav %s. Mix_GetError(): %s", path, Mix_GetError());
	}
	else
	{
		SFX* sfx = new SFX;
		sfx->sound = chunk;
		sfx->typeFx = type;

		fx.push_back(sfx);

		ret = (int)fx.size();
	}

	return ret;
}

// Play WAV
bool Audio::PlayFx(Effects type, int channel, int repeat)
{
	bool ret = false;

	if(!active)
		return false;


		for (const auto& fxItem : fx) {
			if (fxItem->typeFx == type) {
				LOG("Playing sound effect %d", type);
				if (Mix_Playing(channel) == true)
				{
					ret = true;
					break;
				}

				Mix_PlayChannel(channel, fxItem->sound, repeat);
				ret = true;
				break;
			}
		}

	return ret;
}

bool Audio::StopFx(Effects type, int channel)
{
	bool ret = false;

	if (!active)
		return false;

	for (const auto& fxItem : fx) {
		if (fxItem->typeFx == type) {
			if (Mix_Playing(channel) == true)
			{
				Mix_HaltChannel(channel);
				ret = true;
				break;
			}
		}
	}

	return ret;
}

void Audio::PlayRandFx(Effects type1, Effects type2, Effects type3, int channel)
{
	int val = rand() % 3;

	if (val == 0 && type1 != Effects::UNKNOWN)
	{
		PlayFx(type1, channel, 0);
	}
	else if (val == 1 && type2 != Effects::UNKNOWN)
	{
		PlayFx(type2, channel, 0);
	}
	else if (val == 2 && type3 != Effects::UNKNOWN)
	{
		PlayFx(type3, channel, 0);
	}
}

bool Audio::StopMusic()
{
	if (!active)
		return false;

	Mix_HaltMusic();
	return true;
}
