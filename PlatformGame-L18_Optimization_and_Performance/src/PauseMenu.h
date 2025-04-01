#pragma once
#include "Module.h"
#include "SDL2/SDL.h"

class PauseMenu : public Module
{
public:

	PauseMenu();
	~PauseMenu();
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

	bool once = false;

private:
	
	// Textures
	SDL_Texture* background;
	SDL_Texture* buttons;

	// Buttons
	SDL_Rect Button1 = { 0,42,106,38 };
	SDL_Rect Button2 = { 0,115,106,38 };

	bool haveToChange = false;
};
