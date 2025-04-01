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
	background = Engine::GetInstance().textures.get()->Load("Assets/Textures/MainMenu.png");

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

	// Draw the background
	//Section to draw the background (window size)
	SDL_Rect backgroundRect = { 0, 0, 1920, 1080 };
	Engine::GetInstance().render.get()->DrawUIimage(background, 0, 0,1, &backgroundRect);

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
	Engine::GetInstance().render.get()->DrawTexture(buttons, 360, 300, &Button1);
	Engine::GetInstance().render.get()->DrawTexture(buttons, 360, 400, &Button2);

	int x, y;
	
	Vector2D mousePos = Engine::GetInstance().input.get()->GetMousePosition();
	
	x = mousePos.getX();
	y = mousePos.getY();
	
	//Check wher is the mouse
	LOG("X: %d Y: %d", x, y);

	if (x >= 360 && x <= 600 && y >= 300 && y <= 360) {

		option = SELECTED::START;
		
	}
	else if (x >= 360 && x <= 600 && y >= 400 && y <= 480) {

		option = SELECTED::EXIT;
	}
	else {
		option = SELECTED::NONE;
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
