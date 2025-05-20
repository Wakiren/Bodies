#include "GuiManager.h"
#include "Engine.h"
#include "Textures.h"

#include "GuiControlButton.h"
#include "GuiControlSlider.h"
#include "GuiControlToggle.h"
#include "Audio.h"
#include "MainMenu.h"
#include "SettingsMenu.h"
#include "PauseMenu.h"
#include "IntroScene.h"
#include "Scene.h"
#include "DialogueSystem.h"

GuiManager::GuiManager() :Module()
{
	name = "guiManager";
}

GuiManager::~GuiManager() {}

bool GuiManager::Start()
{
	Filter = Engine::GetInstance().textures->Load("Assets/UI/Filter.png");
	Trees = Engine::GetInstance().textures->Load("Assets/Maps/Trees.png");
	NoEyeFilter = Engine::GetInstance().textures->Load("Assets/UI/NoEyeFilter.png");

	// Play the sound effect right at the fokin start --------------------------------------------- MUSIC
		// uncomment to play
	Engine::GetInstance().audio->PlayMusic("Assets/Audio/Music/MainTheme.wav", 0.0f);

	return true;
}

// L16: TODO 1: Implement CreateGuiControl function that instantiates a new GUI control and add it to the list of controls
GuiControl* GuiManager::CreateGuiControl(GuiControlType type, int id, const char* text, SDL_Rect bounds, int fontSize, Module* observer,bool IsDialog, SDL_Rect sliderBounds, int min, int max, int value)
{
	GuiControl* guiControl = nullptr;

	//Call the constructor according to the GuiControlType
	switch (type)
	{
	case GuiControlType::BUTTON:
		guiControl = (GuiControl*) new GuiControlButton(id, bounds, text, fontSize);
		break;
	case GuiControlType::SLIDER:
		guiControl = (GuiControl*) new GuiControlSlider(id, bounds, text, fontSize, min, max, value);
		break;
	case GuiControlType::TOGGLE:
		guiControl = (GuiControl*) new GuiControlToggle(id, bounds, text, fontSize);
		break;
	}

	//Set the observer
	guiControl->SetObserver(observer);

	// Created GuiControls are add it to the list of controls
	/*guiControlsList.push_back(guiControl);*/
	if (IsDialog == true)
	{
		guiControlsList_.Add(guiControl);
	}
	else
	{
		guiControlList.Add(guiControl);
	}

	return guiControl;
}

void GuiManager::EnableButton(GuiControl* button)
{
	button->active = true;
}

void GuiManager::DisableButton(GuiControl* button)
{
	button->active = false;
}

bool GuiManager::Update(float dt)
{	
	if (Engine::GetInstance().mainMenu->active == false && Engine::GetInstance().pauseMenu->active == false
		&& Engine::GetInstance().introScene->active == false && Engine::GetInstance().settingsMenu.get()->active == false)
	{
		Engine::GetInstance().render->DrawTexture(Trees, 0, 0);
		Engine::GetInstance().render->DrawUIimage(Filter, 0, 0);

		if (NoEye == true)
		{
			Engine::GetInstance().render->DrawUIimage(NoEyeFilter, 0, 0);
		}
	}

	accumulatedTime += dt;
	if (accumulatedTime >= updateMsCycle) doLogic = true;

	// We control how often the GUI is updated to optimize the performance
	if (doLogic == true)
	{
		ListItem<GuiControl*>* control = guiControlsList_.start;
		ListItem<GuiControl*>* control2 = guiControlList.start;
		while (control != nullptr)
		{
			if (control->data->state != GuiControlState::NONE) { control->data->Update(dt); }
			control = control->next;
		}
		while (control2 != nullptr)
		{
			if (control2->data->state != GuiControlState::NONE) { control2->data->Update(dt); }
			control2 = control2->next;
		}

		accumulatedTime = 0.0f;
		doLogic = false;
	}

	switch (state)
	{
		case UIState::INTRO:
			break;
		case UIState::TITLE:
			filterActive = false;
			//Engine::GetInstance().audio->PlayFx(MainTheme);
			break;
		case UIState::GAME:
			break;
		case UIState::COMBAT:
			break;
		case UIState::PAUSE:
			break;
		case UIState::CREDIT:
			break;
		case UIState::GAMEOVER:
			break;
		default:
			break;
	}

	if (Engine::GetInstance().scene.get()->player->openInventory == true)
	{
		Engine::GetInstance().scene.get()->player->inventory->UpdateInventory(dt);
	}

	return true;
}

bool GuiManager::Draw()
{
	ListItem<GuiControl*>* control_ = guiControlList.start;
	while (control_ != NULL)
	{
		if (control_->data->state != GuiControlState::NONE) {
			if (control_->data->active) {
				control_->data->Draw(Engine::GetInstance().render.get());
			}
		}
		control_ = control_->next;
	}

	return true;
}

bool GuiManager::DrawDialog()
{
	ListItem<GuiControl*>* control = guiControlsList_.start;
	while (control != NULL)
	{
		if (control->data->state != GuiControlState::NONE) {
			if (control->data->active) {
				control->data->Draw(Engine::GetInstance().render.get());
			}
		}
		control = control->next;
	}
	return true;
}


bool GuiManager::CleanUp()
{
	ListItem<GuiControl*>* control = guiControlsList_.start;

	/*while (control != nullptr)
	{
		RELEASE(control);
	}*/

	guiControlsList_.Clear();

	return true;
}



