#pragma once

#include "Input.h"
#include "Render.h"
#include "Module.h"

#include "Log.h"
#include "SString.h"
#include "Vector2D.h"

enum class GuiControlType
{
	BUTTON,
	TOGGLE,
	CHECKBOX,
	SLIDER,
	SLIDERBAR,
	COMBOBOX,
	DROPDOWNBOX,
	INPUTBOX,
	VALUEBOX,
	SPINNER
};

enum class GuiControlState
{
	NONE,
	DISABLED,
	NORMAL,
	FOCUSED,
	PRESSED,
	SELECTED
};

class GuiControl
{
public:

	// Constructor
	GuiControl(GuiControlType type, int id) : type(type), id(id), state(GuiControlState::NORMAL) {}

	// Constructor
	GuiControl(GuiControlType type, SDL_Rect bounds, const char* text, int fontSize) :
		type(type),
		state(GuiControlState::NORMAL),
		bounds(bounds),
		text(text),
		fontSize(fontSize)
	{
		color.r = 255; color.g = 255; color.b = 255;
		texture = NULL;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	virtual bool Draw(Render* render)
	{
		return true;
	}
	// 
	void SetTexture(SDL_Texture* tex)
	{
		texture = tex;
		section = { 0, 0, 0, 0 };
	}

	// 
	void SetObserver(Module* module)
	{
		observer = module;
	}

	// 
	void NotifyObserver()
	{
		observer->OnGuiMouseClickEvent(this);
	}

	void DialogueButton() { dialogueButton = true; }


public:

	int id;
	GuiControlType type;
	GuiControlState state;

	SString text;           // Control text (if required)
	SDL_Rect bounds;        // Position and size
	SDL_Color color;        // Tint color

	SDL_Texture* texture;   // Texture atlas reference
	SDL_Rect section;       // Texture atlas base section
	SDL_Rect sectionFocused; // Texture atlas base section for focused state

	bool active = true; // Control active state

	int fontSize = FONT_SIZE;           // Font size (if required)

	Module* observer;        // Observer 

	bool dialogueButton = false; // Is a dialogue button
};