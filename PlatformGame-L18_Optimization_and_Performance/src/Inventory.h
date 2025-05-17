#pragma once

#include "Player.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Item.h"
#include "GuiControlButton.h"
#include <vector>
#include <string>

#define MAX_ITEMS 117
#define ITEM_SCALE 4
#define ITEM_SIZE 16
#define INVE_POSITION_X 657
#define INVE_POSITION_Y 97

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
	void ResetInventory();
	bool EquipToggle(Item* item);

	vector<Item*> items;
	bool isFull;

public:

	SDL_Texture* texture;

	GuiControlButton* Use = nullptr;
	GuiControlButton* Drop = nullptr;

	
	//Diferent Organitzation Methods
	vector<string> PriorityList1 = {
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

	vector<string> PriorityList2 = {
		"Torch",
		"Sizors",
		"Eye",
		"Alpha",
		"Beta",
		"Omega",
		"Sigma",
		"MixedMeat",
		"HumanFlesh",
		"AnimalSkin",
		"CookedMixedMeat",
		"CookedAnimalSkin",
		"CookedHumanFlesh",
		"Sedative"
	};

	vector<string> PriorityList3 = {
		"Alpha",
		"Beta",
		"Omega",
		"Sigma",
		"MixedMeat",
		"HumanFlesh",
		"AnimalSkin",
		"CookedMixedMeat",
		"CookedAnimalSkin",
		"CookedHumanFlesh",
		"Sedative",
		"Torch",
		"Sizors",
		"Eye"
	};

	int listChange = 0;
	vector<vector<string>> PriorityList = {
		PriorityList1,
		PriorityList2,
		PriorityList3
	};

private:

	int rows = 0;
	//No item is selected
	int currentItem = -1;

	string health;
	string attack;
	string defense;
};