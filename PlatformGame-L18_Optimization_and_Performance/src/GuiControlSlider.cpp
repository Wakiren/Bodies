#include "GuiControlSlider.h"
#include "GuiControlButton.h"
#include "Engine.h"
#include "GuiManager.h"
#include "Window.h"

GuiControlSlider::GuiControlSlider(int id, SDL_Rect bounds, const char* text, int fontSize) : GuiControl(GuiControlType::SLIDER, id)
{
	this->bounds = bounds;
	this->text = text;
	this->fontSize = fontSize;
	SetObserver(observer);

	Vector2D thumb_size(20, 20);
	SDL_Rect thumbPos = { bounds.x, (int)(bounds.y + bounds.h / 2 - thumb_size.getY() / 2), (int)thumb_size.getX(), (int)thumb_size.getY() };
	thumb = (GuiControlButton*)Engine::GetInstance().guiManager->CreateGuiControl(GuiControlType::BUTTON, id + 1, "", thumbPos, fontSize, observer);

	thumb->bounds.x = bounds.x + 0.5 * (bounds.w - thumb->bounds.w);
	
	// Initialize slider properties
	// this->sliderBounds = sliderBounds;
	// this->minValue = minValue;
	// this->maxValue = maxValue;
	// this->currentValue = currentValue;
	// this->stepValue = stepValue;
	// this->isDragging = false;
	// this->dragOffsetX = 0;
	// this->dragOffsetY = 0;
}

GuiControlSlider::~GuiControlSlider()
{
	delete thumb;
}

bool GuiControlSlider::Update(float dt)
{
	if (state != GuiControlState::DISABLED)
	{
		thumb->Update(dt);

		if (thumbClicked)
		{
			Vector2D mousePos = Engine::GetInstance().input->GetMousePosition();
			float posX = mousePos.getX() - thumb->bounds.w / 2;

			if (posX < bounds.x)
			{
				posX = (float)bounds.x;
			}
			if (posX > bounds.x + bounds.w - thumb->bounds.w) 
			{
				posX = (float)(bounds.x + bounds.w - thumb->bounds.w);
			}

			if (thumb->bounds.x != posX)
			{
				thumb->bounds.x = (int)posX;
				float average = (posX - bounds.x) / (bounds.w - thumb->bounds.w);

				if (average < 0.0f)
				{
					average = 0.0f;
				}
				else if (average > 1.0f)
				{
					average = 1.0f;
				}

				/*value = average;*/
				NotifyObserver();

				if (thumb->state != GuiControlState::PRESSED)
				{
					thumbClicked = false;
					state = GuiControlState::NORMAL;
				}
			}

			else if (thumb->state == GuiControlState::PRESSED)
			{
				thumbClicked = true;
				state = GuiControlState::PRESSED;
			}
		}

		Draw(Engine::GetInstance().render.get());
	}
	return false;
}

bool GuiControlSlider::Draw(Render* render)
{
	int boundX = bounds.x - Engine::GetInstance().render->camera.x;
	int boundY = bounds.y - Engine::GetInstance().render->camera.y;
	SDL_Rect boundsXY = { boundX, boundY, bounds.w, bounds.h };

	render->DrawRectangle(boundsXY, 107, 124, 132, 106, true, true);

	switch (state)
	{
	
	case GuiControlState::DISABLED:
		
		render->DrawRectangle(thumb->bounds, 200, 200, 200, 255, true, false);
		
		break;
	
	case GuiControlState::NORMAL:
		
		render->DrawRectangle(thumb->bounds, 107, 124, 132, 106, true, false);
		
		break;
	
	case GuiControlState::FOCUSED:
		
		render->DrawRectangle(thumb->bounds, 107, 124, 132, 168, true, false);
		
		break;
	
	case GuiControlState::PRESSED:
		
		render->DrawRectangle(thumb->bounds, 200, 206, 209, 168, true, false);
		
		break;
	}

	return false;
}
