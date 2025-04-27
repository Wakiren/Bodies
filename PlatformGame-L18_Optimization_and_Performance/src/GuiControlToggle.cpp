#include "GuiControlToggle.h"
#include "Engine.h"

GuiControlToggle::GuiControlToggle(int id, SDL_Rect bounds, const char* text, int fontSize) : GuiControl(GuiControlType::TOGGLE, id)
{
    this->bounds = bounds;
    this->text = text;

    canClick = true;
    drawBasic = false;
    inToggle = false;

    fromColor = { 200, 200, 200, 255 }; // Grey
    toColor = { 10, 110, 0, 255 };   // Green
    toggleTime = 0.5f;
    currentTime = 0.0f;
}

bool GuiControlToggle::Update(float dt)
{
    if (state != GuiControlState::DISABLED)
    {
        Vector2D mousePos = Engine::GetInstance().input->GetMousePosition();

        if (mousePos.getX() > bounds.x && mousePos.getX() < bounds.x + bounds.w && mousePos.getY() > bounds.y && mousePos.getY() < bounds.y + bounds.h) {

            Engine::GetInstance().render->DrawRectangle({ bounds.x - 3, bounds.y - 3, bounds.w + 6, bounds.h + 6 }, 250, 248, 246, 255, true, false);

            if (Engine::GetInstance().input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_DOWN && !inToggle) {
                if (state == GuiControlState::PRESSED)
                {
                    state = GuiControlState::NORMAL;
                    fromColor = { 200, 200, 200, 255 }; // Grey
                    toColor = { 10, 110, 0, 255 };   // Green
                    currentTime = 0.0f;
                    inToggle = true;
                }
                else if (state == GuiControlState::NORMAL)
                {
                    state = GuiControlState::PRESSED;
                    fromColor = { 180, 190, 170, 255 }; // Grey
                    toColor = { 10, 110, 0, 255 };   // Green
                    currentTime = 0.0f;
                    inToggle = true;
                }
            }
        }

        Toggle(dt);

        Engine::GetInstance().render->DrawRectangle(bounds, fromColor.r, fromColor.g, fromColor.b, fromColor.a, true, false);
    }

    return false;
}

void GuiControlToggle::Toggle(float dt)
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
}
