#include "PauseMenu.h"
#include "Engine.h"
#include "Textures.h"
#include "Render.h"
#include "Scene.h"
#include "EntityManager.h"
#include "MainMenu.h"
#include "FadeManager.h"

PauseMenu::PauseMenu()
{
	name = "pauseMenu";
}

PauseMenu::~PauseMenu()
{

}

bool PauseMenu::Awake()
{
	return true;
}

bool PauseMenu::Start()
{
	// Load Textures
	background = Engine::GetInstance().textures.get()->Load("Assets/Textures/PauseMenu.png");
	
	buttons = Engine::GetInstance().textures.get()->Load("Assets/Textures/Buttons_.png");

	return true;
}

bool PauseMenu::PreUpdate()
{
	return true;
}

bool PauseMenu::Update(float dt)
{
	if (once)
	{	
		// Deactivate the modules of scene and entity manager
		Engine::GetInstance().scene.get()->active = false;
		Engine::GetInstance().entityManager.get()->active = false;
		Engine::GetInstance().physics.get()->active = false;
		Engine::GetInstance().map.get()->active = false;
		once = false;
	}
	bool ret = true;

	// Draw the background
	//Section to draw the background (window size)
	SDL_Rect backgroundRect = { 0, 0, 1920, 1080 };
	Engine::GetInstance().render.get()->DrawUIimage(background, 0, 0, 1, &backgroundRect);

	Engine::GetInstance().render.get()->camera.x = 0;
	Engine::GetInstance().render.get()->camera.y = 0;

	/*Engine::GetInstance().render.get()->camera.x = 0;
	Engine::GetInstance().render.get()->camera.y = 0;*/

	// Active the scene and the entity manager, and deactivate the main menu
	if (haveToChange && Engine::GetInstance().fadeManager.get()->GetCurrentFadeType() == FadeType::FADE_IN) {
		if (option == SELECTED::EXIT) {
			Engine::GetInstance().pauseMenu.get()->active = false;
			Engine::GetInstance().map.get()->active = false;
			Engine::GetInstance().mainMenu.get()->active = true;


		}
		else if (option == SELECTED::CONTINUE){
			Engine::GetInstance().pauseMenu.get()->active = false;
			Engine::GetInstance().scene.get()->active = true;
			Engine::GetInstance().entityManager.get()->active = true;
			Engine::GetInstance().physics.get()->active = true;
			Engine::GetInstance().map.get()->active = true;
		}
		haveToChange = false;
	}

	// Function to detect the mouse click
	if (Engine::GetInstance().input.get()->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP) {
		if (option == SELECTED::CONTINUE) {
			haveToChange = true;
			Engine::GetInstance().fadeManager.get()->Fade(3.0f, 300);
		}
		if (option == SELECTED::EXIT) {
			haveToChange = true;
			Engine::GetInstance().fadeManager.get()->Fade(3.0f, 300);
		}
	}

	//Button textures
	Engine::GetInstance().render.get()->DrawTexture(buttons, 200, 150, &Button1);
	Engine::GetInstance().render.get()->DrawTexture(buttons, 200, 200, &Button2);

	int x, y;

	Vector2D mousePos = Engine::GetInstance().input.get()->GetMousePosition();

	//LOG("X: %f Y: %f", mousePos.getX(), mousePos.getY());

	x = mousePos.getX();
	y = mousePos.getY();


	if (x >= 200 && x <= 270 && y >= 150 && y <= 180) {

		option = SELECTED::CONTINUE;
	}
	else if (x >= 200 && x <= 260 && y >= 200 && y <= 220) {

		option = SELECTED::EXIT;
	}
	else {
		option = SELECTED::NONE;
	}

	return ret;
}

bool PauseMenu::PostUpdate()
{
	return true;
}

bool PauseMenu::CleanUp()
{
	return true;
}



