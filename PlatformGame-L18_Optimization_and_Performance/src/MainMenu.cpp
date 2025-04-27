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

	startButton = (GuiControlButton*)Engine::GetInstance().guiManager.get()->CreateGuiControl(GuiControlType::BUTTON, 1,"Start", { 200, 150, 106, 38 }, 50, this);
	exitButton = (GuiControlButton*)Engine::GetInstance().guiManager.get()->CreateGuiControl(GuiControlType::BUTTON, 2, "Exit", { 200, 200, 106, 38 }, 50, this);

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
	Engine::GetInstance().render.get()->DrawUIimage(background, 0, 0, 1, &backgroundRect);

	Engine::GetInstance().render.get()->camera.x = 0;
	Engine::GetInstance().render.get()->camera.y = 0;

	// Active the scene and the entity manager, and deactivate the main menu
	if (haveToChange && Engine::GetInstance().fadeManager.get()->GetCurrentFadeType() == FadeType::FADE_IN) {
		if (startButton->state == GuiControlState::PRESSED) {
			Engine::GetInstance().physics.get()->active = true;
			Engine::GetInstance().map.get()->active = true;
			Engine::GetInstance().scene.get()->active = true;
			Engine::GetInstance().entityManager.get()->active = true;
			Engine::GetInstance().puzzleManager.get()->active = true;
			Engine::GetInstance().mainMenu.get()->active = false;
			haveToChange = false;
		}
		else if (exitButton->state == GuiControlState::PRESSED) {
			ret = false;
		}
	}

	// Function to detect the mouse click
	if (Engine::GetInstance().input.get()->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP) {
		if (startButton->state == GuiControlState::PRESSED) {
			haveToChange = true;
			Engine::GetInstance().fadeManager.get()->Fade(3.0f,300);
		}
		if (exitButton->state == GuiControlState::PRESSED) {
			haveToChange = true;
			Engine::GetInstance().fadeManager.get()->Fade(3.0f, 300);
		}
	}

	//Button textures
	Engine::GetInstance().render.get()->DrawTexture(buttons, 200, 150, &Button1);
	Engine::GetInstance().render.get()->DrawTexture(buttons, 200, 200, &Button2);

	// Draw the buttons
	startButton->Draw(Engine::GetInstance().render.get());
	exitButton->Draw(Engine::GetInstance().render.get());

	int x, y;
	
	Vector2D mousePos = Engine::GetInstance().input.get()->GetMousePosition();
	
	//LOG("X: %f Y: %f", mousePos.getX(), mousePos.getY());

	x = mousePos.getX();
	y = mousePos.getY();
	

	if (x >= 200 && x <= 270 && y >= 150 && y <= 180) {

		option = SELECTED::START;
		
	}
	else if (x >= 200 && x <= 260 && y >= 200 && y <= 220) {

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
