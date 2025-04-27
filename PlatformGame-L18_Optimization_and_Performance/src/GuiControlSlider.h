#pragma once

#include "GuiControl.h"
#include "GuiControlButton.h"
#include "Vector2D.h"

class GuiControlSlider : public GuiControl
{

public:

	GuiControlSlider(int id, SDL_Rect bounds, const char* text = "", int fontSize = FONT_SIZE);
	virtual ~GuiControlSlider();

	// Called each loop iteration
	bool Update(float dt);
	bool Draw(Render* render);

private:
	GuiControlButton* thumb = nullptr;
	bool thumbClicked = false;
	bool canClick = true;
	bool drawBasic = false;
	float value;
};