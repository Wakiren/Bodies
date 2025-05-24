#pragma once

#include "GuiControl.h"
#include "GuiControlButton.h"
#include "Vector2D.h"

class GuiControlSlider : public GuiControl
{

public:

	GuiControlSlider(int id, SDL_Rect bounds, const char* text = "", int fontSize = FONT_SIZE,int min = 0, int max = 0, int value = 0, SDL_Texture*buttonTexture = nullptr);
	virtual ~GuiControlSlider();

	// Called each loop iteration
	bool Update(float dt);
	bool Draw(Render* render);

	int GetValuePercent() const;
	void SetValue(int value);

	SDL_Texture* textureButton = nullptr;

private:
	
	void AddValue();
	void SubstractValue();
	void CalculeValue();

	SDL_Rect slider;
	int value;

	int minValue;
	int maxValue;

	int lastKey; // 0 == left; 1 == right
	bool once = true;

	SDL_Rect auxRect = { 0,0,0,0 };

	SDL_Texture* sliderBarTexture = nullptr;
	SDL_Texture* sliderTexture = nullptr;

	////Fx
	//uint clickFx;
	//uint focusedFx;
	//bool isPlayable;
};