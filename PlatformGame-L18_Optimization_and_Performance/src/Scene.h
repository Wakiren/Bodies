#pragma once

#include "Module.h"
#include "Player.h"
#include "Enemy.h"
#include "NPC.h"
#include <vector>
#include "GuiControlButton.h"
#include "Physics.h"
#include "Item.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene();

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Return the player position
	Vector2D GetPlayerPosition();

	//L15 TODO 1: Implement the Load function
	void LoadState();
	//L15 TODO 2: Implement the Save function
	void SaveState();

	// Handles multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control);

	float Slower(float ogPos, float goalPos, float time);

	void StartDialogue(NPC &npc);

	Item CreateItem(string name, Vector2D pos);

	void HandleAudio();

public:
	// Get tilePosDebug value
	std::string GetTilePosDebug() {
		return tilePosDebug;

	}

	int RandomValue(int min, int max);

	int WWidth;
	int WHeight;

	std::vector<Enemy*> enemyList;
	std::vector<Item*> itemList;
	Player* player;

private:
	SDL_Texture* mouseTileTex = nullptr;
	std::string tilePosDebug = "[0,0]";
	bool once = true;

// L16: TODO 2: Declare a GUI Control Button 
	GuiControlButton* guiBt;
	int dialogueID;
	bool exit;

};