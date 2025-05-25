#include "Inventory.h"
#include "Engine.h"
#include "Scene.h"
#include "Textures.h"
#include "Render.h"
#include "GuiControlButton.h"
#include "GuiManager.h"
#include "DialogueSystem.h"

Inventory::Inventory()
{
	texture = Engine::GetInstance().textures->Load("Assets/UI/InventoryPlaceHolder.png");
	isFull = false;

	Use = (GuiControlButton*)Engine::GetInstance().guiManager.get()->CreateGuiControl(GuiControlType::BUTTON,0, "Use", { ITEM_POS_X + 50, 512 , 100, 50 }, 50, Engine::GetInstance().scene.get(), false);
	Drop = (GuiControlButton*)Engine::GetInstance().guiManager.get()->CreateGuiControl(GuiControlType::BUTTON, 1, "Drop", { ITEM_POS_X, 512 + Use->bounds.h + 30 , 100, 50 }, 50, Engine::GetInstance().scene.get(), false);

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
		Engine::GetInstance().audio.get()->PlayRandFx(Effects::EAT1, Effects::EAT1, Effects::EAT2,1);
	}
	else if (item->type == "Sedative")
	{
		// Heal the player Madness meter
		RemoveItem(item);
	}
	else if (item->type == "Weapon")
	{
		// Equip the weapon
		if (EquipToggle(item) == true)
		{
			player->combatStats->attackPoints += item->amount;
		}
		else
		{
			player->combatStats->attackPoints -= item->amount;
		}
		Engine::GetInstance().audio.get()->PlayFx(Effects::EQUIP,2);
	}
	else if (item->type == "Armor")
	{
		// Equip the armor
		if (EquipToggle(item) == true)
		{
			player->combatStats->defensePoints += item->amount;
		}
		else
		{
			player->combatStats->defensePoints -= item->amount;
		}
		Engine::GetInstance().audio.get()->PlayFx(Effects::EQUIP,2);
	}
	else if (item->type == "Equipable")
	{
		EquipToggle(item);
		Engine::GetInstance().audio.get()->PlayFx(Effects::EQUIP,2);
	}
}

void Inventory::UpdateInventory(float dt)
{
	if (Engine::GetInstance().dialogueSystem.get()->inDialog == true)
	{
		return;
	}
	// Draw the inventory UI
	Engine::GetInstance().render->DrawUIimage(texture, 0, 0, 1);

	//Draw the inventory UI
	for (size_t i = 0; i < items.size(); ++i)
	{
		rows = i / 9;
		items[i]->DrawInInventory({ (float)INVE_POSITION_X + ((i - (9*rows)) * (ITEM_SCALE * ITEM_SIZE)), (float)INVE_POSITION_Y + (rows * (ITEM_SCALE * ITEM_SIZE))}, ITEM_SCALE);

		if (items[i]->InventorySelected == true)
		{
			currentItem = i;
		}
	}
	rows = 0;



	//Update Current Item
	if (currentItem != -1)
	{
		//Update the buttons
		Use->Update(dt);
		Drop->Update(dt);

		//Change the text of the buttons if nessessari
		if (items[currentItem]->equiped == false)
		{
			Use->text = "Use";
			Use->bounds.x = ITEM_POS_X;
		}
		else
		{
			Use->text = "Unequip";
			Use->bounds.x = ITEM_POS_X -60;
		}

		//Check if the buttons are pressed
		if (Use->state == GuiControlState::PRESSED)
		{
			items[currentItem]->InventorySelected = false;
			UseItem(items[currentItem], Engine::GetInstance().scene.get()->player);
			currentItem = -1;
		}
		else if (Drop->state == GuiControlState::PRESSED)
		{
			items[currentItem]->InventorySelected = false;
			RemoveItem(items[currentItem]);
			currentItem = -1;
		}

		Use->Draw(Engine::GetInstance().render.get());
		Drop->Draw(Engine::GetInstance().render.get());
	}

	//Update stats

	if (Engine::GetInstance().scene.get()->player != nullptr)
	{
		health = to_string( Engine::GetInstance().scene.get()->player->combatStats->health);
		defense = to_string(Engine::GetInstance().scene.get()->player->combatStats->defensePoints);
		attack = to_string(Engine::GetInstance().scene.get()->player->combatStats->attackPoints);

		// Draw the stats
		Engine::GetInstance().render->DrawText("Health", 100, 100, 50, { 255,255,255 });
		Engine::GetInstance().render->DrawText(health.c_str(), 100, 200, 35, { 255,255,255 });
		Engine::GetInstance().render->DrawText("Defense" , 100, 300, 50, { 255,255,255 });
		Engine::GetInstance().render->DrawText(defense.c_str(), 100, 400, 35, { 255,255,255 });
		Engine::GetInstance().render->DrawText("Attack" , 100, 500, 50, { 255,255,255 });
		Engine::GetInstance().render->DrawText(attack.c_str(), 100, 600, 35, { 255,255,255 });
	}

}

void Inventory::OrganizeInventory()
{
	if (listChange >= PriorityList.size())
	{
		listChange = 0;
	}
	vector<Item*> listItems;
	// Organize the inventory items
	for (size_t i = 0; i < PriorityList[listChange].size(); ++i)
	{
		for (size_t j = 0; j < items.size(); ++j)
		{
			LOG("%d", i);
			if (items[j]->name == PriorityList[listChange][i])
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
	listChange++;
}

void Inventory::ResetInventory()
{
	currentItem = -1;
	for (size_t i = 0; i < items.size(); ++i)
	{
		items[i]->InventorySelected = false;
	}
}

bool Inventory::EquipToggle(Item* item)
{
	if (item->equiped == false)
	{
		item->equiped = true;
		return true;
	}
	else
	{
		item->equiped = false;
		return false;
	}
}