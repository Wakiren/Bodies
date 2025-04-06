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

	enum class CombatInput
	{
		ATTACK,
		GUARD,
		SKILL,
		FLEE,
		NONE
	};

	CombatInput combatInput = CombatInput::NONE;

private:

	// Textures
	SDL_Texture* background;
	SDL_Texture* buttons;

	// Buttons
	SDL_Rect button1Rect = {0,0,224,64};
	SDL_Rect button2Rect = {0,64,224,52 };
	SDL_Rect button3Rect = { 0,122,224,52 };
	SDL_Rect button4Rect = { 0,182,224,52 };

	GuiControlButton* button1;

};
