#ifndef __MAINMENU_H__
#define __MAINMENU_H__

#include "Module.h"
#include "SDL2/SDL.h"
#include "GuiControlButton.h"
#include "Animation.h"

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
	SDL_Texture* background2;
	SDL_Texture* background3;
	SDL_Texture* background4;

	bool background1_ = true;
	bool background2_ = false;
	bool background3_ = false;
	bool background4_ = false;

	SDL_Rect Button1 = { 0,0,106,38 };
	SDL_Rect Button2 = { 0,111,106,38 };

	GuiControlButton* startButton = nullptr;
	GuiControlButton* optionsButton = nullptr;
	GuiControlButton* exitButton = nullptr;

	SDL_Texture* StartButtonTexture = nullptr;
	SDL_Texture* OptionsButtonTexture = nullptr;
	SDL_Texture* ExitButtonTexture = nullptr;

	Animation* currentAnimation = nullptr;
	Animation mainMenuAnimation;

	bool haveToChange = false;
	bool haveToStart = false;
	bool haveToOptions = false;
	bool haveToExit = false;

	bool startMenu = true;

	int clickSoundUI = 0;
};

#endif // __MAINMENU_H__
