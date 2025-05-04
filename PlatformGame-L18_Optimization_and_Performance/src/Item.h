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

public:

	bool isPicked = false;
	PhysBody* pbody;

private:

	SDL_Texture* texture;
	const char* texturePath;
	int texW, texH;
	pugi::xml_node parameters;
	Animation* currentAnimation = nullptr;
	Animation idle;

	//Atributes
	string type;
	int amount;

	//L08 TODO 4: Add a physics to an item
	int Vel = 30; //Velocity at wich the object gets expeld
	int angle = 0; //Angle at which the object gets expelled
};
