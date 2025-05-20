#include "GuiControlToggle.h"
#include "Engine.h"
#include "Window.h"

GuiControlToggle::GuiControlToggle(int id, SDL_Rect bounds, const char* text, int fontSize) : GuiControl(GuiControlType::TOGGLE, id)
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
    
    inToggle = false;

    fromColor = { 113, 0, 14, 255 }; // Red
    toColor = { 7, 107, 0, 255 };   // Green
    toggleTime = 0.5f;
    currentTime = 0.0f;
}

bool GuiControlToggle::Update(float dt)
{
    if (state != GuiControlState::DISABLED)
    {
        Vector2D mousePos = Engine::GetInstance().input->GetMousePosition() * Engine::GetInstance().window.get()->GetScale();

        if (mousePos.getX() > bounds.x && mousePos.getX() < bounds.x + bounds.w && mousePos.getY() > bounds.y && mousePos.getY() < bounds.y + bounds.h) {

			state = GuiControlState::FOCUSED;

            if (Engine::GetInstance().input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT) {
				
                state = GuiControlState::PRESSED;
            }

            if (Engine::GetInstance().input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN) {

				state = GuiControlState::PRESSED;
                isToggled = !isToggled;
            }

            if (Engine::GetInstance().input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP) {
                NotifyObserver();
            }
		}
		else {
			state = GuiControlState::NORMAL;
		}
        
        //Toggle(dt);
    }

    return false;
}

SDL_Color GuiControlToggle::Toggle(float dt)
{
    currentTime += dt;
    if (currentTime > toggleTime)
    {
        currentTime = toggleTime;
        inToggle = false;
    }

    float t = currentTime / toggleTime;

    fromColor.r = static_cast<Uint8>(fromColor.r + t * (toColor.r - fromColor.r));
    fromColor.g = static_cast<Uint8>(fromColor.g + t * (toColor.g - fromColor.g));
    fromColor.b = static_cast<Uint8>(fromColor.b + t * (toColor.b - fromColor.b));
    fromColor.a = static_cast<Uint8>(fromColor.a + t * (toColor.a - fromColor.a));

	return fromColor;
}

bool GuiControlToggle::Draw(Render* render)
{
    switch (state)
    {

    case GuiControlState::DISABLED:
    {
        LOG("DISABLED");
        render->DrawRectangle({ bounds.x, bounds.y, bounds.w, bounds.h }, 0, 0, 0, 0, true, false);

    } break;

    case GuiControlState::NORMAL:
    {
        if (isToggled)
        {
            render->DrawRectangle({ bounds.x -  Spacing, bounds.y - Spacing, bounds.w + Spacing, bounds.h + Spacing }, toColor.r, toColor.g, toColor.b, toColor.a, true, false);
        }
        else if (!isToggled)
        {
            render->DrawRectangle({ bounds.x - Spacing, bounds.y - Spacing, bounds.w + Spacing, bounds.h + Spacing }, fromColor.r, fromColor.g, fromColor.b, fromColor.a, true, false);
        }

    }break;

    case GuiControlState::FOCUSED:
    {
        LOG("FOCUSED");
        render->DrawRectangle({ bounds.x - 3, bounds.y - 3, bounds.w + 6, bounds.h + 6 }, 250, 248, 246, 255, true, false);
        render->DrawRectangle({ bounds.x - Spacing, bounds.y - Spacing, bounds.w + Spacing, bounds.h + Spacing }, 71, 75, 78, fromColor.a, true, false);

    } break;

    case GuiControlState::PRESSED:
    {
        render->DrawRectangle({ bounds.x - Spacing, bounds.y - Spacing, bounds.w + Spacing, bounds.h + Spacing }, fromColor.r, fromColor.g, fromColor.b, fromColor.a, true, false);

    } break;

    case GuiControlState::SELECTED:
        break;
    }

    int size = fontSize;
    int x = bounds.w / size * 0.5;
    int y = bounds.h - size / 4;

    Engine::GetInstance().render.get()->DrawText(text.GetString(), bounds.x + x, bounds.y, size, { 255,255,255 });
    return false;
}
