#pragma once

#include "Entity.h"
#include "SDL2/SDL.h"
#include "Animation.h"
#include "SString.h"
#include <vector>

struct SDL_Texture;
using namespace std;

//The parameters of the item while its showing on the inventory
#define ITEM_POS_X 1550
#define ITEM_POS_Y 300
#define PREVIEW_SCALE 10

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

	bool CheckMouseHover(Vector2D pos, int scale);

	void SplitDesc(SString text, int fontSize_, int max_chars_line_);

public:

	bool isPicked = false;
	PhysBody* pbody;

	//Atributes
	string type;
	int amount;
	vector<SString> desc;
	bool equiped = false;

	//Inventory
	bool InventorySelected = false;
	SDL_Texture* texture;
	Animation* currentAnimation = nullptr;

private:

	SDL_Texture* Hover;
	SDL_Texture* Selected;
	const char* texturePath;
	int texW, texH;
	pugi::xml_node parameters;
	Animation idle;

	//L08 TODO 4: Add a physics to an item
	int Vel = 30; //Velocity at wich the object gets expeld
	int angle = 0; //Angle at which the object gets expelled
};
