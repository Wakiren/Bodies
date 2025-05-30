//#include "SettingsMenu.h"
//#include "Engine.h"
//#include "Textures.h"
//#include "Render.h"
//#include "MainMenu.h"
//#include "GuiManager.h"
//#include "FadeManager.h"
//#include "Window.h"
//#include "Audio.h"
//
//SettingsMenu::SettingsMenu()
//{
//	name = "settingsmenu";
//}
//
//SettingsMenu::~SettingsMenu()
//{
//
//}
//
//bool SettingsMenu::Awake()
//{
//	return true;
//}
//
//bool SettingsMenu::Start()
//{
//	// Load Textures
//	background = Engine::GetInstance().textures.get()->Load("Assets/Textures/MainMenuAnimations.png");
//	backButtonTexture = Engine::GetInstance().textures.get()->Load("Assets/UI/PauseMenuExit.png");
//	fullscreenButtonTexture = Engine::GetInstance().textures.get()->Load("Assets/UI/SettingsFullscreen.png");
//	volumeSliderTexture = Engine::GetInstance().textures.get()->Load("Assets/UI/PauseMenuOptions.png");
//	backButton = (GuiControlButton*)Engine::GetInstance().guiManager.get()->CreateGuiControl(GuiControlType::BUTTON, 4, "Back", { 848, 500, 106, 38 }, 50, this, false,backButtonTexture);
//	fullscreenButton = (GuiControlToggle*)Engine::GetInstance().guiManager.get()->CreateGuiControl(GuiControlType::TOGGLE, 5, "Fullscreen", { 848, 650, 100, 20}, 50, this, false,fullscreenButtonTexture);
//	volumeSlider = (GuiControlSlider*)Engine::GetInstance().guiManager.get()->CreateGuiControl(GuiControlType::SLIDER, 6, "Volume", { 848,800, 675, 50 }, 50, this, false,volumeSliderTexture,{ (int)Engine::GetInstance().window.get()->width / 2 - 100 / 2, (int)(Engine::GetInstance().window.get()->height / 3 * 2.5) + 50, 100, 20 }, 0, 220, 110);
//
//	// Load the fx sound
//	clickSoundUI = Engine::GetInstance().audio.get()->LoadFx("Assets/Audio/Fx/clickSoundUI.ogg", Effects::UICLICK);
//
//	mainMenuAnimation.LoadAnimations(configParameters.child("animations").child("particle"));
//	currentAnimation = &mainMenuAnimation;
//
//	// Set the initial state of the buttons
//	backButton->active = false;
//	fullscreenButton->active = false;
//	volumeSlider->active = false;
//
//	return true;
//}
//
//bool SettingsMenu::PreUpdate()
//{
//	return true;
//}
//
//bool SettingsMenu::Update(float dt)
//{
//	bool ret = true;
//
//	if (startOptions)
//	{
//		Engine::GetInstance().guiManager.get()->EnableButton(backButton);
//		Engine::GetInstance().guiManager.get()->EnableButton(fullscreenButton);
//		Engine::GetInstance().guiManager.get()->EnableButton(volumeSlider);
//		startOptions = false;
//	}
//
//	if (once) {
//		fullscreenButton->state = GuiControlState::NORMAL;
//		once = false;
//	}
//
//	// Draw the background
//	//Section to draw the background (window size)
//	Engine::GetInstance().render.get()->DrawUIimage(background, 0, 0, 1, 0, &currentAnimation->GetCurrentFrame());
//	currentAnimation->Update();
//
//	Engine::GetInstance().render.get()->camera.x = 0;
//	Engine::GetInstance().render.get()->camera.y = 0;
//
//	// Active the scene and the entity manager, and deactivate the main menu
//	if (haveToChange && Engine::GetInstance().fadeManager.get()->GetCurrentFadeType() == FadeType::FADE_IN) {
//		if (haveToBack) {
//			Engine::GetInstance().settingsMenu.get()->active = false;
//			Engine::GetInstance().mainMenu.get()->active = true;
//			Engine::GetInstance().guiManager.get()->DisableButton(backButton);
//			Engine::GetInstance().guiManager.get()->DisableButton(fullscreenButton);
//			Engine::GetInstance().guiManager.get()->DisableButton(volumeSlider);
//			startOptions = true;
//			haveToBack = false;
//			once = true;
//		}
//	}
//	if (haveToChange && haveToFullscreen) {
//		if (fullscreenButton->IsToggled()) {
//			Uint32 flags = SDL_GetWindowFlags(Engine::GetInstance().window->window);
//
//			if (flags & SDL_WINDOW_FULLSCREEN_DESKTOP) {
//				SDL_SetWindowFullscreen(Engine::GetInstance().window->window, 0);
//			}
//			else {
//				SDL_SetWindowFullscreen(Engine::GetInstance().window->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
//			}
//		}
//		else if (!fullscreenButton->IsToggled()) {
//			Uint32 flags = SDL_GetWindowFlags(Engine::GetInstance().window->window);
//
//			if (flags & SDL_WINDOW_FULLSCREEN_DESKTOP) {
//				SDL_SetWindowFullscreen(Engine::GetInstance().window->window, 0);
//			}
//			else {
//				SDL_SetWindowFullscreen(Engine::GetInstance().window->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
//			}
//		}
//		haveToFullscreen = false;
//		haveToChange = false;
//	}
//
//	// Function to detect the mouse click
//	if (Engine::GetInstance().input.get()->GetMouseButtonDown(SDL_BUTTON_LEFT) == KeyState::KEY_UP) {
//		if (backButton->state == GuiControlState::PRESSED) {
//			Engine::GetInstance().audio.get()->PlayFx(Effects::UICLICK, 0);
//			haveToChange = true;
//			haveToBack = true;
//			Engine::GetInstance().fadeManager.get()->Fade(3.0f, 300);
//		}
//		if (fullscreenButton->state == GuiControlState::PRESSED) {
//			Engine::GetInstance().audio.get()->PlayFx(Effects::UICLICK, 1);
//			haveToFullscreen = true;
//			haveToChange = true;
//			/*Engine::GetInstance().window.get()->ToggleFullscreen();
//			fullscreenButton->state = GuiControlState::NORMAL;*/
//		}
//	}
//
//	backButton->Draw(Engine::GetInstance().render.get());
//	fullscreenButton->Draw(Engine::GetInstance().render.get());
//	volumeSlider->Draw(Engine::GetInstance().render.get());
//
//	return ret;
//}
//
//bool SettingsMenu::PostUpdate()
//{
//	return true;
//}
//
//bool SettingsMenu::CleanUp()
//{
//	return true;
//}
//



