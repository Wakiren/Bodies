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
#include "TreeBoss.h"
#include "EntityManager.h"
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
					sym->position = { mapObject.x , mapObject.y  };
					sym->texture = Engine::GetInstance().textures.get()->Load("Assets/Puzzles/Alpha.png");
					symbols.push_back(sym);
					break;
				case 1:
					sym->Start(collider, "Beta");
					sym->position = { mapObject.x , mapObject.y };
					sym->texture = Engine::GetInstance().textures.get()->Load("Assets/Puzzles/Beta.png");	
					symbols.push_back(sym);
					break;
				case 2:
					sym->Start(collider, "Omega");
					sym->position = { mapObject.x , mapObject.y  };
					sym->texture = Engine::GetInstance().textures.get()->Load("Assets/Puzzles/Omega.png");
					symbols.push_back(sym);
					break;
				case 3:
					sym->Start(collider, "Sigma");
					sym->position = { mapObject.x , mapObject.y  };
					sym->texture = Engine::GetInstance().textures.get()->Load("Assets/Puzzles/Sigma.png");
					symbols.push_back(sym);
					break;
				case 4:
					Altar = collider;
					AltarCompleated = Engine::GetInstance().textures.get()->Load("Assets/Puzzles/Altar.png");
					AltarPos.setX(mapObject.x);
					AltarPos.setY(mapObject.y);
					break;
				case 5:
					TreeBossZone = collider;
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


		if ((*it)->taken == false)
		{
			Engine::GetInstance().render.get()->DrawTexture((*it)->texture, (*it)->position.getX(), (*it)->position.getY());
		}

		if ((*it)->IsInZone() == true && (*it)->taken == false)
		{
			DysplayText();
			if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_E) == KEY_DOWN )
			{
				Item Key = Engine::GetInstance().scene.get()->CreateItem((*it)->name, (*it)->position);
				Key.SpawnFromEnemy();
				(*it)->taken = true;
				
				numKeys++;
			}
		}

		numKeys = 4;

		if (Altar->Contains(Engine::GetInstance().scene.get()->player->GetPosition().getX(),
			Engine::GetInstance().scene.get()->player->GetPosition().getY()))
		{
			DysplayText();
			if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_E) == KEY_DOWN && numKeys == 4)
			{
				Compleated = true;
			}
		}

		if (Compleated)
		{
			Engine::GetInstance().render.get()->DrawTexture(AltarCompleated, AltarPos.getX(), AltarPos.getY());
		}

		if (TreeBossZone->Contains(Engine::GetInstance().scene.get()->player->GetPosition().getX(),
			Engine::GetInstance().scene.get()->player->GetPosition().getY()) && Compleated == true && BossDefeated == false)
		{
			DysplayText();
			if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
			{
				BossDefeated = true;
				pugi::xml_node enemyNode = Engine::GetInstance().scene->configParameters.child("entities").child("enemies").child("Cannibal");
					
				TreeBoss* enemy = (TreeBoss*)Engine::GetInstance().entityManager->CreateEntity(EntityType::ENEMY, enemyNode.name());
				enemy->SetParameters(enemyNode);
				enemy->pbody->listener->type = EntityType::ENEMY;
				//enemy->ItemsInEnemy.push_back("Eye"); //Set Special Items

				Engine::GetInstance().scene->player->EnterCombatWith(enemy);

				Engine::GetInstance().scene->enemyList.push_back(enemy);
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