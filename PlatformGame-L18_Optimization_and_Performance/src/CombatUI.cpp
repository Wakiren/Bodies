#include "CombatUI.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "MainMenu.h"
#include "Entitymanager.h"
#include "Scene.h"
#include "Physics.h"
#include "Engine.h"
#include "FadeManager.h"
#include "GuiManager.h"


#include "Defs.h"
#include "Log.h"

CombatUI::CombatUI()
{
}

CombatUI::~CombatUI()
{
}

bool CombatUI::Awake()
{

	return true;
}

bool CombatUI::Start()
{
	buttons = Engine::GetInstance().textures.get()->Load("Assets/Textures/CombatButtons.png");
	background = Engine::GetInstance().textures.get()->Load("Assets/Textures/combatBackground.png");

	attackButton = (GuiControlButton*)Engine::GetInstance().guiManager.get()->CreateGuiControl
	(GuiControlType::BUTTON, 1, "Attack", {632,600,224,64} , 50, this);
	guardButton = (GuiControlButton*)Engine::GetInstance().guiManager.get()->CreateGuiControl
	(GuiControlType::BUTTON, 1, "Guard", { 632,64 + 600,224,64 }, 50, this);
	skillButton = (GuiControlButton*)Engine::GetInstance().guiManager.get()->CreateGuiControl
	(GuiControlType::BUTTON, 1, "Skill", { 632,128 + 600,224,64 }, 50, this);
	fleeButton = (GuiControlButton*)Engine::GetInstance().guiManager.get()->CreateGuiControl
	(GuiControlType::BUTTON, 1, "Flee", { 632,192 + 600,224,64 }, 50, this);
	return true;
}

bool CombatUI::PreUpdate()
{
	return true;
}

bool CombatUI::Update(float dt)
{
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_1) == KEY_DOWN) 
	{
		active = !active;
	}

	if(active) 
	{
		Vector2D mousePos = Engine::GetInstance().input.get()->GetMousePosition();
		float mouseX = mousePos.getX();
		float mouseY = mousePos.getY();

		//Background
		SDL_Rect backgroundRect = { 0, 0, 720, 480 };
		Engine::GetInstance().render.get()->DrawUIimage(background, Engine::GetInstance().render.get()->camera.w / 3.25,
		Engine::GetInstance().render.get()->camera.h / 2, 1, &backgroundRect);


		if (Engine::GetInstance().input.get()->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP) {
			if (attackButton->state == GuiControlState::FOCUSED) 
			{
				combatInput = CombatInput::ATTACK;
			}
			if (guardButton->state == GuiControlState::FOCUSED) 
			{
				combatInput = CombatInput::GUARD;
			}
			if (skillButton->state == GuiControlState::FOCUSED) 
			{
				combatInput = CombatInput::SKILL;
			}
			if (fleeButton->state == GuiControlState::FOCUSED) 
			{
				combatInput = CombatInput::FLEE;
			}
		}

		attackButton->Draw(Engine::GetInstance().render.get());
		guardButton->Draw(Engine::GetInstance().render.get());
		skillButton->Draw(Engine::GetInstance().render.get());
		fleeButton->Draw(Engine::GetInstance().render.get());

		//Buttons
		//Engine::GetInstance().render.get()->DrawUIimage(buttons, Engine::GetInstance().render.get()->camera.w / 3.1,
		//Engine::GetInstance().render.get()->camera.h / 1.9,1, &button1Rect);


		//Engine::GetInstance().render.get()->DrawUIimage(buttons, Engine::GetInstance().render.get()->camera.w / 3.1,
		//Engine::GetInstance().render.get()->camera.h / 1.9 + 64 * 1.8f, 1, &button2Rect);


		//Engine::GetInstance().render.get()->DrawUIimage(buttons, Engine::GetInstance().render.get()->camera.w / 3.1,
		//Engine::GetInstance().render.get()->camera.h / 1.9 + 128 * 1.8f, 1, &button3Rect);


		//Engine::GetInstance().render.get()->DrawUIimage(buttons, Engine::GetInstance().render.get()->camera.w / 3.1,
		//Engine::GetInstance().render.get()->camera.h / 1.9 + 192 * 1.8f, 1, &button4Rect);


		/*if (Engine::GetInstance().input.get()->GetMouseButtonDown(1) == KEY_UP) 
		{
			if (mouseX >= 150 && mouseX <= 210 && mouseY >= 140 && mouseY <= 160)
			{
				combatInput = CombatInput::ATTACK;
			}

			if (mouseX >= 150 && mouseX <= 210 && mouseY >= 170 && mouseY <= 180)
			{
				combatInput = CombatInput::GUARD;
			}

			if (mouseX >= 150 && mouseX <= 210 && mouseY >= 200 && mouseY <= 210)
			{
				combatInput = CombatInput::SKILL;
			}

			if (mouseX >= 150 && mouseX <= 210 && mouseY >= 225 && mouseY <= 240)
			{
				combatInput = CombatInput::FLEE;
			}
		}*/
	}

	return true;
}

bool CombatUI::PostUpdate()
{
	return true;
}

bool CombatUI::CleanUp()
{
	return true;
}
