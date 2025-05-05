#pragma once

#include "Entity.h"
#include "SDL2/SDL.h"
#include "Animation.h"

struct SDL_Texture;
using namespace std;

class Item : public Entity
{
public:

	Item();
	virtual ~Item();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void SetParameters(pugi::xml_node parameters) {
		this->parameters = parameters;
	}

	void SpawnFromEnemy();

	void SetPosition(Vector2D pos);

	void Stabilize();

	void DrawInInventory(Vector2D pos, int scale);

public:

	bool isPicked = false;
	PhysBody* pbody;

	//Atributes
	string type;
	int amount;
	bool InventorySelected = false;

private:

	SDL_Texture* texture;
	SDL_Texture* Hover;
	SDL_Texture* Selected;
	const char* texturePath;
	int texW, texH;
	pugi::xml_node parameters;
	Animation* currentAnimation = nullptr;
	Animation idle;

	//L08 TODO 4: Add a physics to an item
	int Vel = 30; //Velocity at wich the object gets expeld
	int angle = 0; //Angle at which the object gets expelled
};
