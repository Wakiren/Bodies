#include "DialogueSystem.h"
#include "DialogueTree.h"
#include "Engine.h"
#include "Textures.h"
#include "Render.h"
#include "Scene.h"

DialogueSystem::DialogueSystem()
{
	name = "dialogueSystem";
}

DialogueSystem::~DialogueSystem()
{
}

bool DialogueSystem::Awake()
{
	LOG("Loading Dialogue System");
	textBox_path = configParameters.attribute("textBoxBg").as_string();
	return true;
}

bool DialogueSystem::Start()
{
	textBox_tex = Engine::GetInstance().textures.get()->Load(textBox_path);
	return true;
}

bool DialogueSystem::Update(float dt)
{
	if (activeTree != nullptr && inDialog == true)
	{
		//Text box
		activeTree->npc->DrawDialog();
		Vector2D pos = { 0, (float)(Engine::GetInstance().window.get()->height )};
		Engine::GetInstance().render.get()->DrawUIimage(textBox_tex, DIALOGUE_W, (Engine::GetInstance().window.get()->height - DIALOGUE_H));
		activeTree->UpdateTree(dt, Engine::GetInstance().dialogueSystem.get(), pos);
		Engine::GetInstance().guiManager.get()->DrawDialog();

		
	}

	return true;
}

bool DialogueSystem::OnGuiMouseClickEvent(GuiControl* control)
{
	LOG("Event by %d ", control->id);

	// TODO 4: Buttons ID match the choice ID. Use it to access to its attributes
	playerInput = activeTree->activeNode->choicesList[control->id];
	activeTree->whaitDialog.Start();

	// TODO 4: Save important choices
	if (playerInput->eventReturn == DIALOGUE_SAVE)
	{
		activeTree->activeNode->playerAnswer = control->id;
		SaveDialogueState();
	}

	// Check if last node
	if (playerInput->nextNode != -1)
	{
		// TODO 4: If not last node, set active node next node
		activeTree->activeNode = activeTree->nodeList[playerInput->nextNode];
		activeTree->updateOptions = false;
	}
	else // If choice leads to the end of the conversation, change active node to last node
	{
		activeTree->npc->NextID = activeTree->activeNode->choicesList[control->id]->path;
		activeTree->npc->ObjectToGive = activeTree->activeNode->choicesList[control->id]->object;
		activeTree->npc->bloked = activeTree->activeNode->choicesList[control->id]->bloked;
		
		activeTree->activeNode = activeTree->nodeList.at(activeTree->nodeList.size() - 1);
		inDialog = false;
		Engine::GetInstance().scene.get()->player->canInteract = false;

		for (int i = 0; i < Engine::GetInstance().scene.get()->player->inventory->items.size(); i++)
		{
			if (Engine::GetInstance().scene.get()->player->inventory->items[i]->name == "Eye")
			{
				Engine::GetInstance().scene.get()->player->inventory->RemoveItem(Engine::GetInstance().scene.get()->player->inventory->items[i]);
			}
		}
	}

	Engine::GetInstance().guiManager.get()->CleanUp();

	return true;
}

bool DialogueSystem::CleanUp()
{
	// TODO 5: Clean Up 
	if (activeTree != nullptr)
	{
		activeTree->nodeList.clear();
		delete activeTree;
		activeTree = nullptr;
	}

	Engine::GetInstance().input.get()->getInput = false;
	Engine::GetInstance().input.get()->nameEntered = false;
	Engine::GetInstance().input.get()->playerName.clear();

	Engine::GetInstance().guiManager.get()->CleanUp();

	return true;
}


int DialogueSystem::LoadDialogue(const char* file, NPC &npc)
{
	pugi::xml_parse_result result = dialogues.load_file(file);

	DialogueTree* tree = new DialogueTree(false);

	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file, result.description());
		return -1;
	}
	else
	{
		// TODO 1: Load the dialogue tree
		pugi::xml_node pugiNode = dialogues.first_child().first_child();

		while (pugiNode != NULL)
		{
			if (pugiNode.attribute("NPC").as_int() == (int)npc.type && pugiNode.attribute("ID").as_int() == npc.NextID)
			{
				tree->treeID = pugiNode.attribute("ID").as_int();
				tree->npc = &npc;
				tree->activeNode = LoadNodes(pugiNode, tree);
				activeTree = tree;
				break;
			}
			else
			{
				pugiNode = pugiNode.next_sibling("dialogueTree");
			}
		}
	}

	return npc.NextID;
}

DialogueNode* DialogueSystem::LoadNodes(pugi::xml_node& xml_trees, DialogueTree* tree)
{
	DialogueNode* first_node = new DialogueNode;

	// TODO 1: Load tree nodes and save first node. Add all nodes to the list in tree
	for (pugi::xml_node pugiNode = xml_trees.child("node"); pugiNode != NULL; pugiNode = pugiNode.next_sibling("node"))
	{
		DialogueNode* node = new DialogueNode;

		node->nodeID = pugiNode.attribute("id").as_int();
		node->text = pugiNode.attribute("text").as_string();

		LoadChoices(pugiNode, node);

		tree->nodeList.push_back(node);

		if (node->nodeID == 0) { first_node = node; }	// return the first node to set as the active one
	}

	return first_node;
}

void DialogueSystem::LoadChoices(pugi::xml_node& xml_node, DialogueNode* node)
{
	// TODO 1: Load all choices and add them to the list in node
	for (pugi::xml_node choice = xml_node.child("choice"); choice != NULL; choice = choice.next_sibling("choice"))
	{
		DialogueChoice* option = new DialogueChoice;
		option->nextNode = choice.attribute("nextNode").as_int();
		option->text = choice.attribute("option").as_string();
		option->eventReturn = choice.attribute("eventReturn").as_int();

		option->path = choice.attribute("NextID").as_int();
		option->object = choice.attribute("object").as_string();
		option->bloked = choice.attribute("bloked").as_bool();
	
		node->choicesList.push_back(option);
	}
}


bool DialogueSystem::LoadDialogueState()
{
	pugi::xml_document gameStateFile;
	pugi::xml_parse_result result = gameStateFile.load_file("save_dialogue.xml");
	pugi::xml_node node = gameStateFile.child("save_choices");

	bool ret = true;

	if (result == NULL)
	{
		LOG("Could not load xml file save_dialogue.xml. pugi error: %s", result.description());
		ret = false;
	}
	else
	{
		// TODO 7: Load player's name and important choices from xml
		string temp = node.child("player").attribute("player_name").as_string();
		Engine::GetInstance().input.get()->playerName = temp.c_str();
		Engine::GetInstance().input.get()->nameEntered = true;

		for (size_t i = 0; i < activeTree->nodeList.size() && !node.child("node").empty(); i++)
		{
			for (int j = 0; j < activeTree->nodeList[i]->choicesList.size(); j++)
			{
				if (activeTree->nodeList[i]->choicesList[j]->eventReturn == 3)
				{
					activeTree->nodeList[i]->nodeID = node.child("node").attribute("id").as_int();
					activeTree->nodeList[i]->playerAnswer = node.child("node").attribute("answer").as_int();
				}
			}
		}
	}

	return ret;
}

bool DialogueSystem::SaveDialogueState()
{
	bool ret = true;

	// TODO 7: Save player's name and important choices
	pugi::xml_document* saveDoc = new pugi::xml_document();
	pugi::xml_node node = saveDoc->append_child("save_choices");


	pugi::xml_node player = node.append_child("player");

	// save player's name
	player.append_attribute("player_name") = Engine::GetInstance().input.get()->playerName.c_str();

	// save important choices
	for (size_t i = 0; i < activeTree->nodeList.size(); i++)
	{
		for (int j = 0; j < activeTree->nodeList[i]->choicesList.size(); j++)
		{
			if (activeTree->nodeList[i]->playerAnswer > -1 && activeTree->nodeList[i]->choicesList[j]->eventReturn == 3)
			{
				player = node.append_child("node");
				player.append_attribute("id") = activeTree->nodeList[i]->nodeID;
				player.append_attribute("answer") = activeTree->nodeList[i]->playerAnswer;
				player.append_attribute("text") = activeTree->nodeList[i]->choicesList[activeTree->nodeList[i]->playerAnswer]->text.GetString();
				break;
			}
		}
	}

	ret = saveDoc->save_file("save_dialogue.xml");

	return ret;
}
