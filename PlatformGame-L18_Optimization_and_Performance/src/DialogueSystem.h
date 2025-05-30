#ifndef __DIALOGSYSTEM_H__
#define __DIALOGSYSTEM_H__

#include "DialogueSystem.h"
#include "DialogueTree.h"

#include "Engine.h"
#include "GuiManager.h"
#include "Module.h"
#include "GuiControlButton.h"


class DialogueSystem : public Module
{
public:
	DialogueSystem();
	~DialogueSystem();

	// Called before render is available
	bool Awake();
	bool Start();
	bool Update(float dt);
	bool OnGuiMouseClickEvent(GuiControl* control);
	bool CleanUp();


	int LoadDialogue(const char* file, NPC &npc);
	DialogueNode* LoadNodes(pugi::xml_node& xml_trees, DialogueTree* tree);
	void LoadChoices(pugi::xml_node& xml_node, DialogueNode* node);

	bool LoadDialogueState();
	bool SaveDialogueState();

	friend DialogueTree;

public:

	DialogueChoice* playerInput;
	DialogueTree* activeTree = nullptr;

	bool inDialog = false;

	SDL_Texture* textBox_tex;
	const char* textBox_path;

private:
	pugi::xml_document dialogues;
};

#endif // __DIALOGSYSTEM_H__
