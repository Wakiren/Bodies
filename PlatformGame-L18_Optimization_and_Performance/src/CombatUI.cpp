#include "CombatUI.h"
#include "Engine.h"
#include "Textures.h"
#include "Render.h"
#include "Scene.h"
#include "EntityManager.h"
#include "FadeManager.h"
#include "CombatSystem.h"
#include "MainMenu.h"

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


		//Backgroudn
		SDL_Rect backgroundRect = { 0, 0, 720, 480 };
		Engine::GetInstance().render.get()->DrawUIimage(background, Engine::GetInstance().render.get()->camera.w / 3.25,
		Engine::GetInstance().render.get()->camera.h / 2, 1, &backgroundRect);


		//Buttons
		Engine::GetInstance().render.get()->DrawUIimage(buttons, Engine::GetInstance().render.get()->camera.w / 3.1,
		Engine::GetInstance().render.get()->camera.h / 1.9,1, &button1Rect);


		Engine::GetInstance().render.get()->DrawUIimage(buttons, Engine::GetInstance().render.get()->camera.w / 3.1,
		Engine::GetInstance().render.get()->camera.h / 1.9 + 64 * 1.8f, 1, &button2Rect);


		Engine::GetInstance().render.get()->DrawUIimage(buttons, Engine::GetInstance().render.get()->camera.w / 3.1,
		Engine::GetInstance().render.get()->camera.h / 1.9 + 128 * 1.8f, 1, &button3Rect);


		Engine::GetInstance().render.get()->DrawUIimage(buttons, Engine::GetInstance().render.get()->camera.w / 3.1,
		Engine::GetInstance().render.get()->camera.h / 1.9 + 192 * 1.8f, 1, &button4Rect);

		if (Engine::GetInstance().input.get()->GetMouseButtonDown(1)) 
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
