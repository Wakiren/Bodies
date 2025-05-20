#pragma once

#include "Module.h"
#include "GuiControl.h"

#include <list>
#include "List.h"
#include "SString.h"

enum UIState
{
	INTRO,
	TITLE,
	GAME,
	COMBAT,
	PAUSE,
	CREDIT,
	GAMEOVER,
	INVENTORY
};

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

	bool DrawDialog();

	void DrawFilter();

	// Called before quitting
	bool CleanUp();

	// Additional methods
	GuiControl* CreateGuiControl(GuiControlType type, int id, const char* text, SDL_Rect bounds, int fontSize, Module* observer, bool IsDialog, SDL_Rect sliderBounds = { 0,0,0,0 }, int min = 0, int max = 0, int value = 0);
	void EnableButton(GuiControl* button);
	void DisableButton(GuiControl* button);

public:

	//std::list<GuiControl*> guiControlsList;
	List<GuiControl*> guiControlsList_;
	List<GuiControl*> guiControlList;
	SDL_Texture* texture;


	float accumulatedTime = 0.0f;
	float updateMsCycle = 0.0f;
	bool doLogic = false;
	bool filterActive = false;
	bool NoEye = true;

	SDL_Texture* Filter = nullptr;
	SDL_Texture* NoEyeFilter = nullptr;
	SDL_Texture* Trees = nullptr;


	bool once = true;

	UIState state = UIState::INTRO;

};

