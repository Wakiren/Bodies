#include "GuiControlButton.h"
#include "Render.h"
#include "Engine.h"
#include "Window.h"
#include "Audio.h"

GuiControlButton::GuiControlButton(int id, SDL_Rect bounds, const char* text, int fontSize) : GuiControl(GuiControlType::BUTTON, id)
{
	this->bounds = bounds;
	this->text = text;
	this->fontSize = fontSize;
	int length = strlen(text);

	if (length == 0)
	{
		length = 8;
	}
	// The bounds depends on the font size
	this->bounds.w = fontSize * 0.75 * length;
	this->bounds.h = fontSize + 10;

	canClick = true;
	drawBasic = false;
}

GuiControlButton::~GuiControlButton()
{

}

bool GuiControlButton::Update(float dt)
{
	if (state != GuiControlState::DISABLED)
	{
		// L16: TODO 3: Update the state of the GUiButton according to the mouse position
		Vector2D mousePos = Engine::GetInstance().input->GetMousePosition() * Engine::GetInstance().window.get()->GetScale();

		//LOG("X: %f Y: %f", mousePos.getX(), mousePos.getY());
		//LOG("X: %d Y: %d", bounds.x, bounds.y);

		//If the position of the mouse if inside the bounds of the button 
		if (mousePos.getX() > bounds.x && mousePos.getX() < bounds.x + bounds.w && mousePos.getY() > bounds.y && mousePos.getY() < bounds.y + bounds.h) {
		
			state = GuiControlState::FOCUSED;

			if (Engine::GetInstance().input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
				state = GuiControlState::PRESSED;
			}
			
			if (Engine::GetInstance().input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
				NotifyObserver();
			}
		}
		else {
			state = GuiControlState::NORMAL;
		}

		////L16: TODO 4: Draw the button according the GuiControl State
		//switch (state)
		//{
		//case GuiControlState::DISABLED:
		//	Engine::GetInstance().render->DrawRectangle(bounds, 200, 200, 200, 255, true, false);
		//	break;
		//case GuiControlState::NORMAL:
		//	Engine::GetInstance().render->DrawRectangle(bounds, 0, 0, 255, 255, true, false);
		//	break;
		//case GuiControlState::FOCUSED:
		//	Engine::GetInstance().render->DrawRectangle(bounds, 0, 0, 20, 255, true, false);
		//	break;
		//case GuiControlState::PRESSED:
		//	Engine::GetInstance().render->DrawRectangle(bounds, 0, 255, 0, 255, true, false);
		//	break;
		//}

		//Engine::GetInstance().render->DrawText(text.c_str(), bounds.x, bounds.y, bounds.w, bounds.h);

	}

	return false;
}

bool GuiControlButton::Draw(Render* render)
{
	// Draw the right button depending on state
	switch (state)
	{

	case GuiControlState::DISABLED:
	{
		render->DrawRectangle({ bounds.x - Spacing , bounds.y - Spacing, bounds.w + Spacing, bounds.h  + Spacing }, 0, 0, 0, 0, true, false);

	} break;

	case GuiControlState::NORMAL:
	{
		render->DrawRectangle({ bounds.x - Spacing, bounds.y - Spacing, bounds.w + Spacing, bounds.h + Spacing }, 0, 0, 0, 125, true, false);

	}	break;

	case GuiControlState::FOCUSED:
	{
		render->DrawRectangle({ bounds.x - Spacing, bounds.y - Spacing, bounds.w + Spacing, bounds.h + Spacing }, 255, 255, 255, 100, true, false);

	} break;

	case GuiControlState::PRESSED:
	{
		render->DrawRectangle({ bounds.x - Spacing, bounds.y - Spacing, bounds.w + Spacing, bounds.h + Spacing }, 255, 255, 255, 150, true, false);

	} break;

	case GuiControlState::SELECTED:
		break;
	}

	int size = fontSize;
	int x = bounds.w / size * 0.5;
	int y = bounds.h - size / 4;

	Engine::GetInstance().render.get()->DrawText(text.GetString(), bounds.x + x, bounds.y, size, {255,255,255});

	return false;
}

