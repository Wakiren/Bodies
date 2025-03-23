#include "GuiManager.h"
#include "Engine.h"
#include "Textures.h"

#include "GuiControlButton.h"
#include "Audio.h"

GuiManager::GuiManager() :Module()
{
	name = "guiManager";
}

GuiManager::~GuiManager() {}

bool GuiManager::Start()
{
	return true;
}

// L16: TODO 1: Implement CreateGuiControl function that instantiates a new GUI control and add it to the list of controls
GuiControl* GuiManager::CreateGuiControl(GuiControlType type, int id, const char* text, SDL_Rect bounds, int fontSize, Module* observer, SDL_Rect sliderBounds)
{
	GuiControl* guiControl = nullptr;

	//Call the constructor according to the GuiControlType
	switch (type)
	{
	case GuiControlType::BUTTON:
		guiControl = (GuiControl*) new GuiControlButton(id, bounds, text, fontSize);
		break;
	}

	//Set the observer
	guiControl->SetObserver(observer);

	// Created GuiControls are add it to the list of controls
	/*guiControlsList.push_back(guiControl);*/
	guiControlsList_.Add(guiControl);

	return guiControl;
}

bool GuiManager::Update(float dt)
{	
	accumulatedTime += dt;
	if (accumulatedTime >= updateMsCycle) doLogic = true;

	// We control how often the GUI is updated to optimize the performance
	if (doLogic == true)
	{
		ListItem<GuiControl*>* control = guiControlsList_.start;
		while (control != nullptr)
		{
			if (control->data->state != GuiControlState::NONE) { control->data->Update(dt); }
			control = control->next;
		}

		accumulatedTime = 0.0f;
		doLogic = false;
	}

	switch (state)
	{
		case GuiManager::INTRO:
			break;
		case GuiManager::TITLE:
			break;
		case GuiManager::GAME:
			break;
		case GuiManager::COMBAT:
			break;
		case GuiManager::PAUSE:
			break;
		case GuiManager::CREDIT:
			break;
		case GuiManager::GAMEOVER:
			break;
		default:
			break;
	}



	return true;
}

bool GuiManager::Draw()
{
	ListItem<GuiControl*>* control_ = guiControlsList_.start;
	while (control_ != NULL)
	{
		if (control_->data->state != GuiControlState::NONE) {
			control_->data->Draw(Engine::GetInstance().render.get());
		}
		control_ = control_->next;
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



