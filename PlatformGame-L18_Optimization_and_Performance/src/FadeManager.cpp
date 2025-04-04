#pragma once
#include "FadeManager.h"
#include "Log.h"
#include "Engine.h"
#include "Render.h"

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
				fadeAlpha -= fadeSpeed;
				if (fadeAlpha <= 0.0f)
				{
					fadeAlpha = 0.0f;
					currentFadeType = FadeType::FADE_IN;
					cooldownCounter = 0;
				}
			}
		}
		else if (currentFadeType == FadeType::FADE_IN)
		{
			if (cooldownCounter >= cooldown)
			{
				fadeAlpha += fadeSpeed;
				if (fadeAlpha >= 255.0f)
				{
					fadeAlpha = 255.0f;
					isFading = false;
				}
			}
		}
	}

	if (isFading)
	{
		Engine::GetInstance().render.get()->DrawRectangle(fadeRect, 0, 0, 0, (Uint8)(fadeAlpha));
	}

	return true;
}

// Called each loop iteration
bool FadeManager::PostUpdate()
{
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
	fadeAlpha = 255.0f;
	currentFadeType = FadeType::FADE_OUT;
	isFading = true;
	cooldownCounter = 0;
}
