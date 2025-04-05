#pragma once
#include "Module.h"
#include "SDL2/SDL.h"


enum class FadeType
{
	FADE_IN,
	FADE_OUT,
	FADE_IN_OUT
};

class FadeManager : public Module
{
public:
	FadeManager();
	~FadeManager();
	bool Awake();
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	void Fade(float speed, int cooldown);
	bool IsFading() const { return isFading; }
	FadeType GetCurrentFadeType() const { return currentFadeType; }

private:
	SDL_Rect fadeRect = { 0, 0, 1280, 720 };
	SDL_Color fadeColor = { 0, 0, 0, 255 };
	
	float fadeAlpha = 255.0f;
	float fadeSpeed = 1.0f;
	FadeType currentFadeType = FadeType::FADE_IN;

	int cooldown = 0;
	int cooldownCounter = 0;
	bool isFading = false;
};
