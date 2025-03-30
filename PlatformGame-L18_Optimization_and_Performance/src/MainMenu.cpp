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


#include "Defs.h"
#include "Log.h"

MainMenu::MainMenu() : Module()
{
	name = "mainmenu";
}

// Destructor
MainMenu::~MainMenu()
{}

// Called before render is available
bool MainMenu::Awake()
{
	// Deactivate the scene and the entity manager

	LOG("Loading main menu");
	bool ret = true;

	return ret;
}

// Called before the first frame
bool MainMenu::Start()
{
	//background = Engine::GetInstance().textures.get()->Load("Assets/Textures/Background.png");

	buttons = Engine::GetInstance().textures.get()->Load("Assets/Textures/Buttons.png");

	return true;
}

// Called each loop iteration
bool MainMenu::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool MainMenu::Update(float dt)
{
	bool ret = true;

	Engine::GetInstance().render.get()->camera.x = 0;
	Engine::GetInstance().render.get()->camera.y = 0;

	// Active the scene and the entity manager, and deactivate the main menu
	if (haveToChange) {
		Engine::GetInstance().physics.get()->active = true;
		Engine::GetInstance().map.get()->active = true;
		Engine::GetInstance().scene.get()->active = true;
		Engine::GetInstance().entityManager.get()->active = true;
		Engine::GetInstance().mainMenu.get()->active = false;
	}

	// Function to detect the mouse click
	if (Engine::GetInstance().input.get()->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP) {
		if (option == SELECTED::START) {
			haveToChange = true;
		}
		if (option == SELECTED::EXIT) {
			ret = false;
		}
	}

	//Button textures
	Engine::GetInstance().render.get()->DrawTexture(buttons, 300, 140, &Button1);
	Engine::GetInstance().render.get()->DrawTexture(buttons, 300, 300, &Button2);

	int x, y;
	
	Vector2D mousePos = Engine::GetInstance().input.get()->GetMousePosition();
	
	x = mousePos.getX();
	y = mousePos.getY();
	
	if (x >= 300 && x <= 556 && y >= 140 && y <= 178) {

		option = SELECTED::START;
	}
	else if (x >= 300 && x <= 556 && y >= 300 && y <= 338) {

		option = SELECTED::EXIT;
	}

	return ret;
}

// Called each loop iteration
bool MainMenu::PostUpdate()
{
	bool ret = true;
	return ret;
}

// Called before quitting
bool MainMenu::CleanUp()
{
	LOG("Freeing main menu");

	return true;
}
