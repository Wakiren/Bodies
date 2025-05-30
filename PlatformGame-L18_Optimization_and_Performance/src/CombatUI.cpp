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
#include "CombatSystem.h"
#include <string>

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
	background1 = Engine::GetInstance().textures.get()->Load("Assets/Combat/CombatUI.png");
	background = Engine::GetInstance().textures.get()->Load("Assets/Combat/CombatUI2.png");
	CursorIdle = Engine::GetInstance().textures.get()->Load("Assets/UI/CursorIdle.png");
	CursorPress = Engine::GetInstance().textures.get()->Load("Assets/UI/CursorPress.png");
	DeathScreen = Engine::GetInstance().textures.get()->Load("Assets/Textures/DeathScreen.png");

	//Create Buttons
	attackButton = (GuiControlButton*)Engine::GetInstance().guiManager.get()->CreateGuiControl
	(GuiControlType::BUTTON, 1, "Attack", {350,720,224,64} , 50, this, false);
	guardButton = (GuiControlButton*)Engine::GetInstance().guiManager.get()->CreateGuiControl
	(GuiControlType::BUTTON, 1, "Guard", { 350,64 + 720,224,64 }, 50, this, false);
	skillButton = (GuiControlButton*)Engine::GetInstance().guiManager.get()->CreateGuiControl
	(GuiControlType::BUTTON, 1, "Skill", { 350,128 + 720,224,64 }, 50, this, false);
	fleeButton = (GuiControlButton*)Engine::GetInstance().guiManager.get()->CreateGuiControl
	(GuiControlType::BUTTON, 1, "Flee", { 350,192 + 720,224,64 }, 50, this, false);

	return true;
}

bool CombatUI::PreUpdate()
{
	return true;
}

bool CombatUI::Update(float dt)
{

	
	//COMBAT UI RENDER //
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_1) == KEY_DOWN) 
	{
		active = !active;
	}
	if(active) 
	{

		//Background
		SDL_Rect backgroundRect = { 0, 0, 738, 246 };
		Engine::GetInstance().render.get()->DrawUIimage(background, Engine::GetInstance().render.get()->camera.w / 2.75,
		Engine::GetInstance().render.get()->camera.h / 1.6, 1.4f,0, &backgroundRect);

		SDL_Rect background1Rect = { 0, 0, 246, 246 };
		Engine::GetInstance().render.get()->DrawUIimage(background1, Engine::GetInstance().render.get()->camera.w / 6.5,
		Engine::GetInstance().render.get()->camera.h / 1.6, 1.4f,0, &background1Rect);

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
		
		Engine::GetInstance().render.get()->DrawText(text.c_str(), 750, 720, textSize, {255,255,255});
	}

	//MOUSE TEXTURE RENDER//
	SDL_ShowCursor(0); // Hide the cursor
	//Set the cursor to the idle texture
	Vector2D mPos = Engine::GetInstance().input->GetMousePosition() * Engine::GetInstance().window.get()->GetScale();

	if (Engine::GetInstance().input.get()->GetMouseButtonDown(1))
	{
		// Set the cursor to the press texture
		Engine::GetInstance().render.get()->DrawUIimage(CursorPress, mPos.getX(), mPos.getY(), 3);
	}
	else
	{
		Engine::GetInstance().render.get()->DrawUIimage(CursorIdle, mPos.getX(), mPos.getY(), 3);
	}

	if (Engine::GetInstance().combatSystem.get()->dead) 
	{

		Engine::GetInstance().render.get()->DrawUIimage(DeathScreen, 0, 0, 4);
		if (Engine::GetInstance().input.get()->GetMouseButtonDown(1))
		{
			Engine::GetInstance().combatSystem.get()->dead = false;
			Engine::GetInstance().scene.get()->LoadState();
		}
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
