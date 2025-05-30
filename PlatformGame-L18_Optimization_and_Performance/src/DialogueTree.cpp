#include "DialogueTree.h"
#include "Engine.h"

void DialogueNode::SplitText(SString text, int fontSize_, int max_chars_line_)
{
	string line = text.GetString();

	// TODO 6: adapt text to the text box
	if (text.Length() > max_chars_line_)
	{
		int a, b, startIndex = 0;
		for (int j = 0; j <= line.length() / max_chars_line_; j++)	// <= -> in case of decimal, get the round up number 
		{
			a = max_chars_line_ + startIndex;

			b = line.find_first_of(" ", a);
			if (b == std::string::npos) {
				b = line.length();
			}

			// If we reached the end of the word or the end of the input.
			string temp;
			temp.append(line, startIndex, b - startIndex);	// string text to append, int index start, int size of text to append
			texts.push_back(temp.c_str());
			startIndex = b;
		}
	}
	else
	{
		texts.push_back(line.c_str());
	}

	trimmed = true;
}

DialogueTree::DialogueTree(bool a)
{
	active = a;
}

bool DialogueTree::UpdateTree(float dt, Module* mod, Vector2D pos)
{
	max_chars_line = fontSize * 2.5f ;

	// TODO 6: Substitute player's name in text and choices if needed
	if (!Engine::GetInstance().input.get()->playerName.empty())
	{
		activeNode->text.Substitute("%x", Engine::GetInstance().input.get()->playerName.c_str());
	}

	if (!activeNode->trimmed)
	{
		activeNode->SplitText(activeNode->text, fontSize, max_chars_line);
	}

	// TODO 6: Render dialogue in text box
	size_t lines = activeNode->texts.size();
	for (size_t i = 0; i < lines; i++)
	{
		//THE X IN HERE SHOULD BE THE HITBOX OF THE TALKING CHARCATER
		Engine::GetInstance().render.get()->DrawText(activeNode->texts[i].GetString(), DIALOGUE_W + SPACING, (Engine::GetInstance().window.get()->height - DIALOGUE_H) + 50* i + SPACING, fontSize, { 0, 0, 0});

	}

	EventReturn(mod, pos);

	if (!updateOptions)
	{
		if(whaitDialog.ReadSec() > lines)
		{
			updateOptions = UpdateChoices(mod, pos, fontSize);
		}
	}

	return true;
}

bool DialogueTree::UpdateChoices(Module* mod, Vector2D pos, int fontSize)
{
	GuiControl* button;

	// TODO 3: Create buttons for choices
	for (int i = 0; i < activeNode->choicesList.size(); i++)
	{
		const char* ch_option = activeNode->choicesList[i]->text.GetString();	// SString to const char*	
		int w = activeNode->choicesList[i]->text.Length() * fontSize * 0.75;
		int h = fontSize + fontSize / 4;
		SDL_Rect bounds = { Engine::GetInstance().window.get()->width - w, pos.getY() - (h + fontSize) * (i + 1) - DIALOGUE_H , w, h};

		button = (GuiControlButton*)Engine::GetInstance().guiManager.get()->CreateGuiControl(GuiControlType::BUTTON, i, ch_option, bounds, fontSize, mod, true);
		button->state = GuiControlState::NORMAL;
		listDialogueButtons.Add(button);
	}

	return true;
}

bool DialogueTree::EventReturn(Module* mod, Vector2D pos)
{
	for (int i = 0; i < activeNode->choicesList.size(); i++)
	{
		switch (activeNode->choicesList[i]->eventReturn)
		{
		case DIALOGUE_INPUT:

			if (!Engine::GetInstance().input.get()->getInput)
			{
				// Get player's input
				Engine::GetInstance().input.get()->getInput = true;
				updateOptions = true;

				if (Engine::GetInstance().input.get()->nameEntered)
				{
					activeNode->choicesList[i]->text.Substitute("%x", Engine::GetInstance().input.get()->playerName.c_str());	// Change %x for player's name - (const char* current word, const char* new word)
					updateOptions = UpdateChoices(mod, pos, fontSize);
				}
			}

			Engine::GetInstance().render.get()->DrawText("Player's name:", Engine::GetInstance().window.get()->width / 3, 100, fontSize, { 255, 255, 255 });
			if (!Engine::GetInstance().input.get()->playerName.empty())
			{
				const char* ch_name = Engine::GetInstance().input.get()->playerName.c_str();	// SString to const char*	
				Engine::GetInstance().render.get()->DrawText(ch_name, Engine::GetInstance().window.get()->width / 3 + fontSize * 7, 100, fontSize, { 255, 255, 255 });
			}

			break;

		case DIALOGUE_SAVE:
			// see on DialogueSystem::OnGuiMouseClickEvent();
			break;
		case DIALOGUE_IF:


			break;
		default:
			return false;
			break;
		}
	}

	return true;
}

void DialogueTree::CleanUp()
{
	// TODO 5: Clean Up
	for (int j = 0; j < nodeList.size(); j++)
	{
		nodeList[j]->CleanUp();
		delete nodeList[j];
	}

	nodeList.clear();
}
