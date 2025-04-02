#ifndef __DIALOGTREE_H__
#define __DIALOGTREE_H__

#include "GuiManager.h"
#include "Module.h"
#include "GuiControlButton.h"
#include "Window.h"

#include "List.h"
#include "Log.h"
#include "Vector2D.h"
#include "SString.h"
#include <vector>
#include "Textures.h"
#include "NPC.h"


#define DIALOGUE_INPUT 2
#define DIALOGUE_SAVE 3
#define DIALOGUE_IF 4

#define DIALOGUE_H 400
#define DIALOGUE_W 575
#define SPACING 175
#define MaxCharsInLine 15

using namespace std;

// Tree -> nodes -> choices

class DialogueChoice
{
public:
	DialogueChoice() {};
	~DialogueChoice() {};

public:
	// DONE: Add choices attributes
	int choiceID;
	SString text;
	int nextNode;
	int eventReturn;
	int path;
};


class DialogueNode
{
public:
	DialogueNode() {};
	~DialogueNode() {};

	void SplitText(SString text, int fontSize_, int max_chars_line_);

	void CleanUp()
	{
		choicesList.clear();
	}

public:
	// DONE: Add nodes attributes 
	SString text;
	int nodeID;
	vector <DialogueChoice*> choicesList;
	int playerAnswer = -1;

	bool trimmed = false;
	vector<SString> texts;
};


class DialogueTree
{
public:
	DialogueTree();
	DialogueTree(bool a);
	~DialogueTree() {};

	bool UpdateTree(float dt, Module* mod, Vector2D pos);
	bool UpdateChoices(Module* mod, Vector2D pos, int fontSize);
	bool EventReturn(Module* mod, Vector2D pos);
	void CleanUp();

public:
	// DONE: Create tree structure
	int treeID;
	NPC* npc = nullptr;
	bool FinalDialog = false;
	bool active = false;
	vector <DialogueNode*> nodeList;
	DialogueNode* activeNode;

	List <GuiControl*> listDialogueButtons;

	bool updateOptions;
	int fontSize = FONT_SIZE;
	int max_chars_line;
};

#endif //__DIALOGTREE_H__
