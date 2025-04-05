#pragma once
#include "FadeManager.h"
#include "Log.h"
#include "Engine.h"
#include "Render.h"
#include "Window.h"

FadeManager::FadeManager() : Module()
{
	name = "fademanager";
}

// Destructor
FadeManager::~FadeManager()
{
}

// Called before render is available
bool FadeManager::Awake()
{
	LOG("Loading fade manager");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool FadeManager::Start()
{
	SDL_SetRenderDrawBlendMode(Engine::GetInstance().render.get()->renderer, SDL_BLENDMODE_BLEND);
	fadeRect = { 0, 0, Engine::GetInstance().window.get()->width, Engine::GetInstance().window.get()->height };
	return true;
}

// Called each loop iteration
bool FadeManager::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool FadeManager::Update(float dt)
{
	if (isFading)
	{
		cooldownCounter++;
		if (currentFadeType == FadeType::FADE_OUT)
		{
			if (cooldownCounter <= cooldown)
			{
				fadeAlpha += fadeSpeed;
				if (fadeAlpha >= 255.0f)
				{
					fadeAlpha = 255.0f;
					currentFadeType = FadeType::FADE_IN;
					cooldownCounter = 0;
				}
			}
		}
		else if (currentFadeType == FadeType::FADE_IN)
		{
			if (cooldownCounter <= cooldown)
			{
				fadeAlpha -= fadeSpeed;
				if (fadeAlpha <= 0.0f)
				{
					fadeAlpha = 0.0f;
					isFading = false;
				}
			}
		}
	}

	return true;
}

// Called each loop iteration
bool FadeManager::PostUpdate()
{
	if (isFading)
	{
		SDL_SetRenderDrawColor(Engine::GetInstance().render.get()->renderer, 0, 0, 0, (Uint8)(fadeAlpha));
		SDL_RenderFillRect(Engine::GetInstance().render.get()->renderer, &fadeRect);
	}

	return true;
}

// Called before quitting
bool FadeManager::CleanUp()
{
	LOG("Freeing fade manager");

	return true;
}

void FadeManager::Fade(float speed, int cooldown)
{
	this->cooldown = cooldown;
	fadeSpeed = speed;
	fadeAlpha = 0.0f;
	currentFadeType = FadeType::FADE_OUT;
	isFading = true;
	cooldownCounter = 0;
}
