#pragma once
#include "Module.h"
#include "SDL2/SDL.h"
#include "GuiControlButton.h"
#include "GuiControlToggle.h"
#include "GuiControlSlider.h"
#include "GuiControl.h"
#include "GuiManager.h"
#include "Animation.h"

class SettingsMenu : public Module
{
public:

	SettingsMenu();
	~SettingsMenu();
	bool Awake();
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

public:

	enum class SELECTED {
		CONTINUE = 1,
		EXIT,
		NONE
	};
	SELECTED option = SELECTED::NONE;

private:

	// Buttons
	GuiControlButton* backButton = nullptr;
	GuiControlToggle* fullscreenButton = nullptr;
	GuiControlSlider* volumeSlider = nullptr;

	SDL_Texture* background;

	// Texture buttons
	SDL_Texture* backButtonTexture = nullptr;
	SDL_Texture* fullscreenButtonTexture = nullptr;
	SDL_Texture* volumeSliderTexture = nullptr;

	Animation* currentAnimation = nullptr;
	Animation mainMenuAnimation;

	bool haveToChange = false;
	bool haveToBack = false;
	bool haveToFullscreen = false;

	bool startOptions = true;
	int clickSoundUI = 0;
	bool once = true;
};
