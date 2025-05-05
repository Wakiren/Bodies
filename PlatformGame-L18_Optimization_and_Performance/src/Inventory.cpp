#include "Inventory.h"
#include "Engine.h"
#include "Scene.h"
#include "Textures.h"
#include "Render.h"
#include "GuiControlButton.h"

Inventory::Inventory()
{
	texture = Engine::GetInstance().textures->Load("Assets/UI/InventoryPlaceHolder.png");
	isFull = false;

	Use = new GuiControlButton(0, { 1456, 512 , 100, 50 }, "Use", 100);
	Drop = new GuiControlButton(1, { 1456, 512 + Use->bounds.h , 100, 50 }, "Drop", 100);
	items.clear();
}

Inventory::~Inventory()
{
	// Clean up the buttons
	delete Use;
	delete Drop;
	// Clean up the items
	for (Item* item : items)
	{
		delete item;
	}
	items.clear();
}

void Inventory::AddItem(Item* item)
{
	if (items.size() < MAX_ITEMS)
	{
		items.push_back(item);
	}
	else
	{
		isFull = true;
	}
}

void Inventory::RemoveItem(Item* item)
{
	auto it = std::find(items.begin(), items.end(), item);
	if (it != items.end())
	{
		items.erase(it);
	}
	else
	{
		isFull = false;
	}
}

void Inventory::UseItem(Item* item, Player* player)
{
	if (item == nullptr)
		return;

	if (item->type == "Heal")
	{
		// Heal the player
		if (player->combatStats->health + item->amount > player->combatStats->maxHealth)
			player->combatStats->health = player->combatStats->maxHealth;
		else if (player->combatStats->health + item->amount < player->combatStats->maxHealth)
			player->combatStats->health += item->amount;

		// Remove the item from the inventory
		RemoveItem(item);
	}
	else if (item->type == "Sedative")
	{
		// Heal the player Madness meter
		RemoveItem(item);
	}
	else if (item->type == "Weapon")
	{
		// Equip the weapon
		player->combatStats->attackPoints += item->amount;
	}
	else if (item->type == "Armor")
	{
		// Equip the item
		player->combatStats->defensePoints += item->amount;
	}
	else if (item->type == "Equipable")
	{

	}
}

void Inventory::UpdateInventory(float dt)
{
	// Draw the inventory UI
	Engine::GetInstance().render->DrawUIimage(texture, 0, 0, 1);
	//Draw the inventory UI
	for (size_t i = 0; i < items.size(); ++i)
	{
		if (i % 9 == 0 && i != 0)
		{
			rows++;
		}
		items[i]->DrawInInventory({ (float)ITEM_POSITION_X + (i * (ITEM_SCALE * ITEM_SIZE)), (float)ITEM_POSITION_Y + (rows *(ITEM_SCALE * ITEM_SIZE)) }, ITEM_SCALE);
	}
	

	Use->Update(dt);
	Use->Draw(Engine::GetInstance().render.get());
	Drop->Update(dt);
	Drop->Draw(Engine::GetInstance().render.get());

}

void Inventory::OrganizeInventory()
{
	vector<Item*> listItems;
	// Organize the inventory items
	for (size_t i = 0; i < PriorityList.size(); ++i)
	{
		for (size_t j = 0; j < items.size(); ++j)
		{
			if (items[j]->name == PriorityList[i])
			{
				listItems.push_back(items[j]);
			}
		}
	}
	items.clear();
	for (size_t i = 0; i < listItems.size(); ++i)
	{
		items.push_back(listItems[i]);
	}
}