//#include "GuiControlSlider.h"
//#include "GuiControlButton.h"
//#include "Engine.h"
//#include "GuiManager.h"
//#include "Window.h"
//
//GuiControlSlider::GuiControlSlider(int id, SDL_Rect bounds, const char* text, int fontSize, int min, int max, int value) : GuiControl(GuiControlType::SLIDER, id)
//{
//	this->bounds = bounds;
//	this->text = text;
//	this->minValue = min;
//	this->maxValue = max;
//	this->value = value * 2;
//	this->lastKey = -1;
//}
//
//GuiControlSlider::~GuiControlSlider()
//{
//	
//}
//
//bool GuiControlSlider::Update(float dt)
//{
//	if (state != GuiControlState::DISABLED)
//	{
//		Vector2D mousePos = Engine::GetInstance().input->GetMousePosition() * Engine::GetInstance().window.get()->GetScale();
//		int mouseX = mousePos.getX();
//		int mouseY = mousePos.getY();
//
//		// Check collision between mouse and button bounds
//		if (mousePos.getX() > bounds.x && mousePos.getX() < bounds.x + bounds.w && mousePos.getY() > bounds.y && mousePos.getY() < bounds.y + bounds.h) {
//		{
//			state = GuiControlState::FOCUSED;
//
//			/*if ((mouseX > bounds.x) && (mouseX < bounds.x + 46) && Engine::GetInstance().input.get()->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
//			{
//				state = GuiControlState::PRESSED;
//				SubstractValue();
//				NotifyObserver();
//			}
//			else if ((mouseX < bounds.x + bounds.w) && (mouseX > bounds.x + bounds.w - 46) && Engine::GetInstance().input.get()->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
//			{
//				state = GuiControlState::PRESSED;
//				AddValue();
//				NotifyObserver();
//			}*/
//			if (Engine::GetInstance().input.get()->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT || Engine::GetInstance().input.get()->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN)
//			{
//				state = GuiControlState::PRESSED;
//				if (Engine::GetInstance().input.get()->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_REPEAT)
//				{
//					CalculeValue();
//					NotifyObserver();
//				}
//				//else if (app->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_DOWN)	app->audio->PlayFx(channel, clickFx);
//			}
//		}
//		}
//		else
//		{
//			state = GuiControlState::NORMAL;
//		}
//	}
//
//	return false;
//}
//
//bool GuiControlSlider::Draw(Render* render)
//{
//	if (this->value > 221) this->value = 220;
//	else if (this->value < 0) this->value = 0;
//	slider = { section.x + 59, section.y + section.h + 5, this->value, 32 };
//
//	Vector2D mousePos = Engine::GetInstance().input->GetMousePosition() * Engine::GetInstance().window.get()->GetScale();
//	int mouseX, mouseY;
//	mouseX = mousePos.getX();
//	mouseY = mousePos.getY();
//	
//	// Draw the right button depending on state
//	switch (state)
//	{
//	case GuiControlState::DISABLED:
//		render->DrawRectangle({ bounds.x,bounds.y,bounds.w,bounds.h }, 180, 120, 120, 0, true, false);
//		render->DrawRectangle({ bounds.x,bounds.y,bounds.w,bounds.h }, 0, 0, 0, 0, true, false);
//		break;
//
//	case GuiControlState::NORMAL:
//		render->DrawRectangle({ bounds.x,bounds.y,bounds.w,bounds.h }, 180, 120, 120, 180, true, false);
//		render->DrawRectangle({ bounds.x,bounds.y,bounds.w,bounds.h }, 255, 255, 0, 120, true, false);
//		break;
//
//	case GuiControlState::FOCUSED:
//		render->DrawRectangle(bounds, 180, 120, 120, 180, true, false);
//		{
//			SDL_Rect r = { bounds.x, bounds.y, 46,46 };
//			if ((mouseX > bounds.x) && (mouseX < bounds.x + 46)) render->DrawRectangle(r, 255, 255, 0, 120, true, false);
//			else if (lastKey == 0 || lastKey == -1) render->DrawRectangle(r, 255, 255, 0, 120, true, false);
//
//			r.x = bounds.x + bounds.w - 46;
//			if ((mouseX < bounds.x + bounds.w) && (mouseX > bounds.x + bounds.w - 46)) render->DrawRectangle(r, 255, 255, 0, 120, true, false);
//			else if (lastKey == 1) render->DrawRectangle(r, 255, 255, 0, 120, true, false);
//
//		}
//		break;
//
//	case GuiControlState::PRESSED:
//		render->DrawRectangle(bounds, 225, 110, 110, 180, true, false);
//
//		//if ((mouseX > bounds.x) && (mouseX < bounds.x + 46)) render->DrawTexture(texture, bounds.x, bounds.y, &sectionFocused, false);
//		//else if ((mouseX < bounds.x + bounds.w) && (mouseX > bounds.x + bounds.w - 46)) render->DrawTexture(texture, bounds.x + bounds.w - 46, bounds.y, &sectionFocused, false);
//
//		{
//			SDL_Rect r = { bounds.x, bounds.y, 46,46 };
//			if (lastKey == 0) render->DrawRectangle(r, 0, 255, 255, 120, true, false);
//
//			r.x = bounds.x + bounds.w - 46;
//			if (lastKey == 1) render->DrawRectangle(r, 0, 255, 255, 120, true, false);
//		}
//
//
//		break;
//
//	//case GuiControlState::SELECTED:
//	//	render->DrawTexture(texture, bounds.x, bounds.y, &section, false);
//	//	render->DrawTexture(texture, bounds.x + 59, bounds.y + 6, &slider, false);
//	//	break;
//
//	}
//
//	return false;
//}
//
//int GuiControlSlider::GetValue() const
//{
//	int auxValue = this->value / 2;
//	return auxValue;
//}
//
//void GuiControlSlider::SetValue(int value)
//{
//	this->value = value * 2;
//	NotifyObserver();
//}
//
//void GuiControlSlider::AddValue()
//{
//	if (value < 221) this->value += 2;
//}
//
//void GuiControlSlider::SubstractValue()
//{
//	if (value >= 2) this->value -= 2;
//}
//
//void GuiControlSlider::CalculeValue()
//{
//	Vector2D mousePos = Engine::GetInstance().input->GetMousePosition() * Engine::GetInstance().window.get()->GetScale();
//	int mouseX = mousePos.getX();
//	int mouseY = mousePos.getY();
//
//	this->value = mouseX - bounds.x - 62;
//}
