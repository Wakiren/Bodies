#pragma once

#include "Module.h"
#include "Entity.h"
#include <list>

using namespace std;

class EntityManager : public Module
{
public:

	EntityManager();

	// Destructor
	virtual ~EntityManager();

	// Called before render is available
	bool Awake();

	// Called after Awake
	bool Start();

	// Called every frame
	bool Update(float dt);

	// Called before quitting
	bool CleanUp();

	// Additional methods
	Entity* CreateEntity(EntityType type, string name);

	void DestroyEntity(Entity* entity);

	void AddEntity(Entity* entity);

public:

	std::list<Entity*> entities;
};
