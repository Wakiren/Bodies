#pragma once
#include "Module.h"
#include "SDL2/SDL.h"
#include "GuiControlButton.h"
#include "GuiControlToggle.h"
#include "GuiControl.h"
#include "GuiManager.h"

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

	SDL_Texture* background;

	bool haveToChange = false;
	bool haveToBack = false;
	bool haveToFullscreen = false;

	bool once = true;
};
