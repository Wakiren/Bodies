#pragma once

#include "Player.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Item.h"
#include "GuiControlButton.h"
#include <vector>
#include <string>

#define MAX_ITEMS 10
#define ITEM_SCALE 4
#define ITEM_SIZE 16
#define ITEM_POSITION_X 657
#define ITEM_POSITION_Y 97

using namespace std;
class Player;

class Inventory
{
public:

	Inventory();
	~Inventory();

	void AddItem(Item* item);
	void RemoveItem(Item* item);
	void UseItem(Item* item, Player* player);
	void UpdateInventory(float dt);
	void OrganizeInventory();

	vector<Item*> items;
	bool isFull;

public:

	SDL_Texture* texture;

	GuiControlButton* Use = nullptr;
	GuiControlButton* Drop = nullptr;

	vector<string> PriorityList = {
		"MixedMeat",
		"HumanFlesh",
		"AnimalSkin",
		"CookedMixedMeat",
		"CookedAnimalSkin",
		"CookedHumanFlesh",
		"Sedative",
		"Torch",
		"Sizors",
		"Eye",
		"Alpha",
		"Beta",
		"Omega",
		"Sigma"
	};

private:

	int rows = 0;
};