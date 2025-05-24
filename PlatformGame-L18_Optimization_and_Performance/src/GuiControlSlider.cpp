#include "GuiControlSlider.h"
#include "GuiControlButton.h"
#include "Engine.h"
#include "GuiManager.h"
#include "Textures.h"
#include "Window.h"

GuiControlSlider::GuiControlSlider(int id, SDL_Rect bounds, const char* text, int fontSize, int min, int max, int value,SDL_Texture*buttonTexture) : GuiControl(GuiControlType::SLIDER, id)
{
	this->bounds = bounds;
	this->auxRect = bounds;
	this->textureButton = buttonTexture;
	this->text = text;
	this->minValue = min;
	this->maxValue = max;
	this->value = value * 2;
	this->lastKey = -1;

	sliderBarTexture = Engine::GetInstance().textures->Load("Assets/UI/Barra.png");
	sliderTexture = Engine::GetInstance().textures->Load("Assets/UI/Slider.png");
}

GuiControlSlider::~GuiControlSlider()
{
	
}

bool GuiControlSlider::Update(float dt)
{
	if (state != GuiControlState::DISABLED)
	{
		Vector2D mousePos = Engine::GetInstance().input->GetMousePosition() * Engine::GetInstance().window.get()->GetScale();
		int mouseX = mousePos.getX();
		int mouseY = mousePos.getY();

		// Check collision between mouse and button bounds
		if (mousePos.getX() > bounds.x && mousePos.getX() < bounds.x + bounds.w && mousePos.getY() > bounds.y && mousePos.getY() < bounds.y + bounds.h) {
		{
			state = GuiControlState::FOCUSED;

			/*if ((mouseX > bounds.x) && (mouseX < bounds.x + 46) && Engine::GetInstance().input.get()->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
			{
				state = GuiControlState::PRESSED;
				SubstractValue();
				NotifyObserver();
			}
			else if ((mouseX < bounds.x + bounds.w) && (mouseX > bounds.x + bounds.w - 46) && Engine::GetInstance().input.get()->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
			{
				state = GuiControlState::PRESSED;
				AddValue();
				NotifyObserver();
			}*/
			if ((Engine::GetInstance().input.get()->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT || Engine::GetInstance().input.get()->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN)
				&& (mouseX > bounds.x) && (mouseX < (bounds.x + bounds.w)))
			{
				state = GuiControlState::PRESSED;
				if (Engine::GetInstance().input.get()->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
				{
					CalculeValue();
					NotifyObserver();
				}
				//else if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN)	app->audio->PlayFx(channel, clickFx);
			}
		}
		}
		else
		{
			state = GuiControlState::NORMAL;
		}
	}

	//LOG("Value: %d", this->value);

	return false;
}

bool GuiControlSlider::Draw(Render* render)
{
	if (this->value > 221) this->value = 220;
	else if (this->value < 0) this->value = 0;

	Vector2D mousePos = Engine::GetInstance().input->GetMousePosition() * Engine::GetInstance().window.get()->GetScale();
	int mouseX, mouseY;
	mouseX = mousePos.getX();
	mouseY = mousePos.getY();
	
	// Draw the right button depending on state
	switch (state)
	{
	case GuiControlState::DISABLED:
		render->DrawRectangle(bounds, 180, 120, 120, 0, true, false);
		render->DrawRectangle(auxRect, 0, 0, 0, 0, true, false);
		break;

	case GuiControlState::NORMAL:
		render->DrawUIimage(sliderBarTexture, bounds.x, bounds.y, 4,0);
		render->DrawUIimage(sliderTexture, auxRect.x, bounds.y - 20, 4, 0);
		//	render->DrawRectangle(bounds, 180, 120, 120, 255, true, false);
		//	// Draw the slider in the center of the slider as a square depending of the bounds
		//	render->DrawRectangle({ auxRect.x, bounds.y - 20, bounds.w / 2 - 124, bounds.h + 40 }, 255, 255, 255, 255, true, false);
		break;

	case GuiControlState::FOCUSED:
		render->DrawUIimage(sliderBarTexture, bounds.x, bounds.y, 4, 0);
		render->DrawUIimage(sliderTexture, auxRect.x, bounds.y - 20, 4.5, 0);
		/*render->DrawRectangle(bounds, 180, 120, 120, 255, true, false);
		render->DrawRectangle({ auxRect.x, bounds.y - 20, bounds.w / 2 - 124, bounds.h + 40 }, 207, 240, 230, 255, true, false);*/

		break;

	case GuiControlState::PRESSED:
		auxRect.x = bounds.x + bounds.w * value / maxValue;
		if (auxRect.x > (bounds.x + bounds.w) - (bounds.w / 2 - 124))
		{
			auxRect.x = bounds.x + bounds.w - (bounds.w / 2 - 124);
		}
		render->DrawUIimage(sliderBarTexture, bounds.x, bounds.y, 4, 0);
		render->DrawUIimage(sliderTexture, auxRect.x, bounds.y - 20, 4.25, 0);
		/*render->DrawRectangle(bounds, 180, 120, 120, 255, true, false);*/
		//render->DrawRectangle({ auxRect.x, bounds.y - 20, bounds.w / 2 - 124, bounds.h + 40 }, 207, 240, 230, 255, true, false);
		break;
	}

	//LOG("MouseX: %d MouseY: %d", mouseX, mouseY);

	return false;
}

int GuiControlSlider::GetValuePercent() const
{
	int auxValue = (this->value / this->maxValue);
	return auxValue;
}

void GuiControlSlider::SetValue(int value)
{
	this->value = value * 2;
	NotifyObserver();
}

void GuiControlSlider::AddValue()
{
	if (value < 221) this->value += 2;
}

void GuiControlSlider::SubstractValue()
{
	if (value >= 2) this->value -= 2;
}

void GuiControlSlider::CalculeValue()
{
	Vector2D mousePos = Engine::GetInstance().input->GetMousePosition() * Engine::GetInstance().window.get()->GetScale();
	int mouseX = mousePos.getX();
	int mouseY = mousePos.getY();

	this->value = mouseX - bounds.x - 62;
}
