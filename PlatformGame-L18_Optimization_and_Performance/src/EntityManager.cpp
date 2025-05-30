#include "EntityManager.h"
#include "Player.h"
#include "Engine.h"
#include "Textures.h"
#include "Scene.h"
#include "Log.h"
#include "Item.h"
#include "Enemy.h"
#include "Cannibal.h"
#include "Narcissist.h"
#include "TreeBoss.h"
#include "NPC.h"
#include "tracy/Tracy.hpp"

EntityManager::EntityManager() : Module()
{
	name = "entitymanager";
}

// Destructor
EntityManager::~EntityManager()
{}

// Called before render is available
bool EntityManager::Awake()
{
	LOG("Loading Entity Manager");
	bool ret = true;

	//Iterates over the entities and calls the Awake
	for(const auto entity : entities)
	{
		if (entity->active == false) continue;
		ret = entity->Awake();
	}

	return ret;

}

bool EntityManager::Start() {

	bool ret = true; 

	//Iterates over the entities and calls Start
	for(const auto entity : entities)
	{
		if (entity->active == false) continue;
		ret = entity->Start();
	}

	return ret;
}

// Called before quitting
bool EntityManager::CleanUp()
{
	bool ret = true;

	for(const auto entity : entities)
	{
		if (entity->active == false) continue;
		ret = entity->CleanUp();
	}

	entities.clear();

	return ret;
}

Entity* EntityManager::CreateEntity(EntityType type, string name)
{
	Entity* entity = nullptr; 

	//L04: TODO 3a: Instantiate entity according to the type and add the new entity to the list of Entities
	switch (type)
	{
	case EntityType::PLAYER:
		entity = new Player();
		break;
	case EntityType::ITEM:
		entity = new Item();
		break;
	case EntityType::ENEMY:

		if (name == "Narcissist")
		{
			entity = new Narcissist();
		}
		else if (name == "Cannibal")
		{
			entity = new Cannibal();
		}
		else if (name == "TreeBoss")
		{
			entity = new TreeBoss();
		}

	case EntityType::NPC:

		if (name == "Sally")
			entity = new NPC(NPCType::SALLY);
		else if (name == "Henry")
			entity = new NPC(NPCType::HENRY);
		else if (name == "Billy")
			entity = new NPC(NPCType::BILLY);

		break;
	default:
		break;
	}

	entities.push_back(entity);

	return entity;
}

void EntityManager::DestroyEntity(Entity* entity)
{
	for (auto it = entities.begin(); it != entities.end(); ++it)
	{
		if (*it == entity) {
			(*it)->CleanUp();
			delete* it; // Free the allocated memory
			entities.erase(it); // Remove the entity from the list
			break; // Exit the loop after removing the entity
		}
	}
}

void EntityManager::AddEntity(Entity* entity)
{
	if ( entity != nullptr) entities.push_back(entity);
}

void EntityManager::PauseEntities()
{
	pauseEntities = true;
}

void EntityManager::UnPauseEntities()
{
	pauseEntities = false;
}

bool EntityManager::Update(float dt)
{
	ZoneScoped;
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
	{
		PauseEntities();
	}
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_U) == KEY_DOWN)
	{
		UnPauseEntities();
	}
	bool ret = true;
	
	if (pauseEntities) 
	{
		for (const auto entity : entities)
		{
			entity->OnPause();
			entity->active = false;
		}
	}
	else
	{
		for (const auto entity : entities)
		{
			entity->active = true;
			if (Engine::GetInstance().scene.get()->player->sight->Contains(entity->position.getX(), entity->position.getY()))
			{
				if (entity->type == EntityType::ENEMY)
				{
					if (Engine::GetInstance().scene.get()->player->isInCombat == false)
					{
						Engine::GetInstance().audio.get()->PlayFx(Effects::CHASE, 4);
					}
				}
				entity->InSight = true;
			}
			else
			{
				entity->InSight = false;
			}
		}
	}

	if (Engine::GetInstance().scene.get()->player->isInCombat == true)
	{
		Engine::GetInstance().audio.get()->StopFx(Effects::CHASE, 4);
	}
	
	for(const auto entity : entities)
	{
		if (entity->active == false) continue;
		ret = entity->Update(dt);
	}
	return ret;
}