#pragma once
#include "GuiControl.h"
#include "Vector2D.h"
#include "GuiControlButton.h"

class GuiControlToggle : public GuiControl
{
public:

	GuiControlToggle(int id, SDL_Rect bounds, const char* text = "", int fontSize = FONT_SIZE);
	virtual ~GuiControlToggle() {};

	// Called each loop iteration
	bool Update(float dt);
	void Toggle(float dt);

private:
	
	bool canClick = true;
	bool drawBasic = false;

	bool inToggle = false;   

	SDL_Color fromColor;   
	SDL_Color toColor;     
	
	float toggleTime;    
	float currentTime;       
};