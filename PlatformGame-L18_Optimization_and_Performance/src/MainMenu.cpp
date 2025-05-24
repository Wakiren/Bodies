#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "MainMenu.h"
#include "SettingsMenu.h"
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
	background = Engine::GetInstance().textures.get()->Load("Assets/Textures/MainMenuAnimations.png");

	LOG("background");
	buttons = Engine::GetInstance().textures.get()->Load("Assets/Textures/Buttons.png");

	mainMenuAnimation.LoadAnimations(configParameters.child("animations").child("particle"));
	currentAnimation = &mainMenuAnimation;
	// Load the button textures
	StartButtonTexture = Engine::GetInstance().textures.get()->Load("Assets/UI/PauseManuContinue.png");
	OptionsButtonTexture = Engine::GetInstance().textures.get()->Load("Assets/UI/PauseMenuOptions.png");
	ExitButtonTexture = Engine::GetInstance().textures.get()->Load("Assets/UI/PauseMenuExit.png");

	startButton = (GuiControlButton*)Engine::GetInstance().guiManager.get()->CreateGuiControl(GuiControlType::BUTTON, 1,"Start", { 848, 550, 106, 38 }, 70, this, false, StartButtonTexture);
	optionsButton = (GuiControlButton*)Engine::GetInstance().guiManager.get()->CreateGuiControl(GuiControlType::BUTTON, 2, "Options", { 848, 700, 106, 38 }, 70, this, false, OptionsButtonTexture);
	exitButton = (GuiControlButton*)Engine::GetInstance().guiManager.get()->CreateGuiControl(GuiControlType::BUTTON, 3, "Exit", { 848, 850, 106, 38 }, 70, this, false, ExitButtonTexture);

	clickSoundUI = Engine::GetInstance().audio.get()->LoadFx("Assets/Audio/Fx/clickSoundUI.ogg");

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

	if (startMenu)
	{
		Engine::GetInstance().guiManager.get()->EnableButton(startButton);
		Engine::GetInstance().guiManager.get()->EnableButton(optionsButton);
		Engine::GetInstance().guiManager.get()->EnableButton(exitButton);
		startMenu = false;
	}

	// Draw the background
	//Section to draw the background (window size)
	//SDL_Rect backgroundRect = { 0, 0, 1920, 1080 };
	Engine::GetInstance().render.get()->DrawUIimage(background, 0, 0, 1, 0, &currentAnimation->GetCurrentFrame());
	currentAnimation->Update();

	Engine::GetInstance().render.get()->camera.x = 0;
	Engine::GetInstance().render.get()->camera.y = 0;

	// Active the scene and the entity manager, and deactivate the main menu
	if (haveToChange && Engine::GetInstance().fadeManager.get()->GetCurrentFadeType() == FadeType::FADE_IN) {
		if (haveToStart) {
			haveToStart = false;
			Engine::GetInstance().physics.get()->active = true;
			Engine::GetInstance().map.get()->active = true;
			Engine::GetInstance().scene.get()->active = true;
			Engine::GetInstance().entityManager.get()->active = true;
			Engine::GetInstance().puzzleManager.get()->active = true;
			Engine::GetInstance().mainMenu.get()->active = false;
			Engine::GetInstance().guiManager.get()->DisableButton(startButton);
			Engine::GetInstance().guiManager.get()->DisableButton(optionsButton);
			Engine::GetInstance().guiManager.get()->DisableButton(exitButton);
			startMenu = true;
			haveToChange = false;
			Engine::GetInstance().audio.get()->StopMusic();
			Engine::GetInstance().audio.get()->PlayMusic("Assets/Audio/Music/Ambient_Forest_BODIES.wav", 2.0f);
		}
		else if (haveToOptions) {
			haveToOptions = false;
			Engine::GetInstance().settingsMenu.get()->active = true;
			Engine::GetInstance().mainMenu.get()->active = false;
			Engine::GetInstance().guiManager.get()->DisableButton(startButton);
			Engine::GetInstance().guiManager.get()->DisableButton(optionsButton);
			Engine::GetInstance().guiManager.get()->DisableButton(exitButton);
			startMenu = true;
			haveToChange = false;
		}
		else if (haveToExit) {
			haveToExit = false;
			Engine::GetInstance().guiManager.get()->DisableButton(startButton);
			Engine::GetInstance().guiManager.get()->DisableButton(optionsButton);
			Engine::GetInstance().guiManager.get()->DisableButton(exitButton);
			startMenu = true;
			haveToChange = false;
			ret = false;
		}
	}

	// Function to detect the mouse click
	if (Engine::GetInstance().input.get()->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP) {
		if (startButton->state == GuiControlState::PRESSED) {
			Engine::GetInstance().audio.get()->PlayFx(clickSoundUI, 0);
			haveToChange = true;
			haveToStart = true;
			Engine::GetInstance().fadeManager.get()->Fade(3.0f,300);
		}
		if (optionsButton->state == GuiControlState::PRESSED) {
			Engine::GetInstance().audio.get()->PlayFx(clickSoundUI, 0);
			haveToChange = true;
			haveToOptions = true;
			Engine::GetInstance().fadeManager.get()->Fade(3.0f, 300);
		}
		if (exitButton->state == GuiControlState::PRESSED) {
			Engine::GetInstance().audio.get()->PlayFx(clickSoundUI, 0);
			haveToChange = true;
			haveToExit = true;
			Engine::GetInstance().fadeManager.get()->Fade(3.0f, 300);
		}
		
	}

	// Draw the buttons
	startButton->Draw(Engine::GetInstance().render.get());
	exitButton->Draw(Engine::GetInstance().render.get());
	optionsButton->Draw(Engine::GetInstance().render.get());

	////Button textures
	//Engine::GetInstance().render.get()->DrawUIimage(StartButtonTexture, startButton->bounds.x, startButton->bounds.y, 1, &(startButton->bounds));
	//Engine::GetInstance().render.get()->DrawUIimage(OptionsButtonTexture, optionsButton->bounds.x, optionsButton->bounds.y, 1, &(optionsButton->bounds));
	//Engine::GetInstance().render.get()->DrawUIimage(ExitButtonTexture, exitButton->bounds.x, exitButton->bounds.y, 1, &(exitButton->bounds));

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
