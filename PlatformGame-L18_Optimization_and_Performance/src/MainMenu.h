#ifndef __MAINMENU_H__
#define __MAINMENU_H__

#include "Module.h"
#include "SDL2/SDL.h"
#include "GuiControlButton.h"

class MainMenu : public Module
{
public:

	MainMenu();

	// Destructor
	virtual ~MainMenu();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	bool CleanUp();

public:
	enum class SELECTED {
		START = 1,
		CONTINUE,
		OPTIONS,
		EXIT,
		BACK,
		NONE
	};

	SELECTED option = SELECTED::NONE;

	SDL_Texture* buttons;

private:
	
	SDL_Texture* background;

	SDL_Rect Button1 = { 0,0,106,38 };
	SDL_Rect Button2 = { 0,111,106,38 };

	GuiControlButton* startButton = nullptr;
	GuiControlButton* exitButton = nullptr;

	bool haveToChange = false;
	bool haveToStart = false;
	bool haveToExit = false;
};

#endif // __MAINMENU_H__
