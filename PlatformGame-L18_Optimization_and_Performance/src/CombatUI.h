#pragma once
#include "Module.h"
#include "GuiControlButton.h"
#include "SDL2/SDL.h"
#include <string>

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
  
	enum CombatInput
	{
		EMPTY,
		ATTACK,
		GUARD,
		SKILL,
		FLEE,

	};

	CombatInput combatInput = CombatInput::EMPTY;

	std::string text;
	int textSize = 50;

private:

	// Textures
	SDL_Texture* background;
	SDL_Texture* background1;
	SDL_Texture* buttons;

	SDL_Texture* CursorIdle;
	SDL_Texture* CursorPress;
	SDL_Texture* DeathScreen;

	GuiControlButton* attackButton = nullptr;
	GuiControlButton* guardButton = nullptr;
	GuiControlButton* skillButton = nullptr;
	GuiControlButton* fleeButton = nullptr;
};
