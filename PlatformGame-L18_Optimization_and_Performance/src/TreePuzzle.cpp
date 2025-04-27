#include "TreePuzzle.h"
#include "Player.h"
#include "Engine.h"
#include "Textures.h"
#include "Scene.h"
#include "Log.h"
#include "Item.h"
#include "tracy/Tracy.hpp"
#include "Physics.h"
#include "Entity.h"
#include "Window.h"
#include <list>

TreePuzzle::TreePuzzle() 
{

}

// Destructor
TreePuzzle::~TreePuzzle()
{}

// Called before render is available
bool TreePuzzle::Awake()
{
	LOG("Loading PuzzleManager");


	return true;

}

bool TreePuzzle::Start() {

	bool ret = true;

	int i = 0;

	for (const auto& mapObjectGroup : Engine::GetInstance().map.get()->mapData.objectLayers) {

		if (mapObjectGroup->properties.GetProperty("Interactable") != NULL && mapObjectGroup->properties.GetProperty("Interactable")->value == true) {
			for (const auto& mapObject : mapObjectGroup->object_list) {
				/*LOG("pos.x = %d, pos.y = %d, width = %d, height = %d", mapObject._x, mapObject._y, mapObject.width, mapObject.height);*/
				PhysBody* collider = Engine::GetInstance().physics.get()->CreateRectangleSensor(mapObject.x + mapObject.width / 2, mapObject.y + mapObject.height / 2, mapObject.width - 1, mapObject.height, STATIC);
				collider->ctype = ColliderType::INTERACTABLE;
				Vector2D position = { mapObject.x + mapObject.width / 2, mapObject.y + mapObject.height / 2 };

				Symbol* sym = new Symbol();

				switch (i)
				{
				case 0:
					sym->Start(collider,"Alpha");
					sym->position = { mapObject.x + mapObject.width / 2, mapObject.y + mapObject.height / 2 };
					symbols.push_back(sym);
					break;
				case 1:
					sym->Start(collider, "Beta");
					sym->position = { mapObject.x + mapObject.width / 2, mapObject.y + mapObject.height / 2 };
					symbols.push_back(sym);
					break;
				case 2:
					sym->Start(collider, "Omega");
					sym->position = { mapObject.x + mapObject.width / 2, mapObject.y + mapObject.height / 2 };
					symbols.push_back(sym);
					break;
				case 3:
					sym->Start(collider, "Sigma");
					sym->position = { mapObject.x + mapObject.width / 2, mapObject.y + mapObject.height / 2 };
					symbols.push_back(sym);
					break;
				case 4:
					Altar = collider;
					break;
				}
				i++;
			}
		}
	}

	return ret;
}

// Called before quitting
bool TreePuzzle::CleanUp()
{
	bool ret = true;

	return ret;
}

bool TreePuzzle::Update(float dt)
{
	bool ret = true;


	for (auto it = symbols.begin(); it != symbols.end(); ++it) {


		if ((*it)->IsInZone() == true)
		{
			DysplayText();
			if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_E) == KEY_DOWN && (*it)->taken == false)
			{
				Item Key = Engine::GetInstance().scene.get()->CreateItem((*it)->name, (*it)->position);
				Key.SpawnFromEnemy();
				(*it)->taken = true;
			}
		}
	}

	return ret;
}

Symbol::Symbol() : Entity(EntityType::PUZZLE)
{
	LOG("Loading Symbol");
}

Symbol::~Symbol()
{
	LOG("Unloading Symbol");
}

bool Symbol::IsInZone()
{
	return symbolBody->Contains(Engine::GetInstance().scene.get()->player->GetPosition().getX(),
								Engine::GetInstance().scene.get()->player->GetPosition().getY());
}


void TreePuzzle::DysplayText()
{
	Engine::GetInstance().render.get()->DrawText("Press E to interact", Engine::GetInstance().window.get()->width / 2, (Engine::GetInstance().window.get()->height / 2) - 32, 25, { 255,255,255 });
}