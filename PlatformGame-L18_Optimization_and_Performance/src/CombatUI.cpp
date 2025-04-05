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

		SDL_Rect backgroundRect = { 0, 0, 720, 480 };
		Engine::GetInstance().render.get()->DrawUIimage(background, Engine::GetInstance().render.get()->camera.w / 3.25,
		Engine::GetInstance().render.get()->camera.h / 2, 1, &backgroundRect);

		Engine::GetInstance().render.get()->DrawTexture(buttons, 0, 0, &button1Rect);
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
