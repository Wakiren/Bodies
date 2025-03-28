#pragma once

#include "Module.h"
#include "GuiControl.h"

#include <list>
#include "List.h"
#include "SString.h"

class GuiManager : public Module
{
public:

	// Constructor
	GuiManager();

	// Destructor
	virtual ~GuiManager();

	// Called before the first frame
	 bool Start();

	 // Called each loop iteration
	 bool Update(float dt);

	bool Draw();

	// Called before quitting
	bool CleanUp();

	// Additional methods
	GuiControl* CreateGuiControl(GuiControlType type, int id, const char* text, SDL_Rect bounds, int fontSize, Module* observer, SDL_Rect sliderBounds = { 0,0,0,0 });

public:

	//std::list<GuiControl*> guiControlsList;
	List<GuiControl*> guiControlsList_;
	SDL_Texture* texture;

	float accumulatedTime = 0.0f;
	float updateMsCycle = 0.0f;
	bool doLogic = false;



	enum UIState
	{
		INTRO,
		TITLE,
		GAME,
		COMBAT,
		PAUSE,
		CREDIT,
		GAMEOVER
	};

	UIState state = INTRO;

};

