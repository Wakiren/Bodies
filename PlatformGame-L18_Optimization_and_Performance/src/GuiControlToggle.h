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
	SDL_Color Toggle(float dt);
	bool Draw(Render* render);
	bool IsToggled() const { return isToggled; }

private:
	
	bool canClick = true;
	bool drawBasic = false;

	bool inToggle = true;   
	bool once = true;

	SDL_Color fromColor = { 200, 200, 200, 255 };
	SDL_Color toColor = { 10, 110, 0, 255 };
	bool isToggled = true;
	float toggleTime;    
	float currentTime;       
};