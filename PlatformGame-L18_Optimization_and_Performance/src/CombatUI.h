#pragma once
#include "Module.h"
#include "GuiControlButton.h"
#include "SDL2/SDL.h"


class CombatUI : public Module
{
public:

	CombatUI();
	~CombatUI();
	bool Awake();
	bool Start();
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();

	bool active = false;

private:

	// Textures
	SDL_Texture* background;
	SDL_Texture* buttons;

	// Buttons
	SDL_Rect button1Rect = {0,0,224,64};

	GuiControlButton* button1;

};
