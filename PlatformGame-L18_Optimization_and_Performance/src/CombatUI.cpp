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
	background = Engine::GetInstance().textures.get()->Load("Assets/Textures/combatBackground.png");

	//Create Buttons
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

		//Background
		SDL_Rect backgroundRect = { 0, 0, 720, 480 };
		Engine::GetInstance().render.get()->DrawUIimage(background, Engine::GetInstance().render.get()->camera.w / 3.25,
		Engine::GetInstance().render.get()->camera.h / 2, 1, &backgroundRect);

		//Assign a result to each button
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

		
		Engine::GetInstance().render.get()->DrawText(text.GetString(), 900, 600, textSize, { 255,255,255 });
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
