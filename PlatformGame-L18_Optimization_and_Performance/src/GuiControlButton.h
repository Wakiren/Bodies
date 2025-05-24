#pragma once

#include "GuiControl.h"
#include "Vector2D.h"

class GuiControlButton : public GuiControl
{

public:

	//GuiControlButton(int id, SDL_Rect bounds, const char* text = "", int fontSize = FONT_SIZE);
	GuiControlButton(int id, SDL_Rect bounds, const char* text = "", int fontSize = FONT_SIZE, SDL_Texture*textureButton = nullptr);
	virtual ~GuiControlButton();

	// Called each loop iteration
	bool Update(float dt);
	bool Draw(Render* render);

	SDL_Texture* textureButton = nullptr;


private:

	bool canClick = true;
	bool drawBasic = false;
	bool hasTexture = false;
	int Spacing = 10;
	bool dialogueButton = false;
};

#pragma once