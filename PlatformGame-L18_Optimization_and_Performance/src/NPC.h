#pragma once

#include "Entity.h"
#include "SDL2/SDL.h"
#include "Box2D/Box2D.h"
#include "Animation.h"

#define NPC_FRONT_TEXTURE_Y 100

struct SDL_Texture;

using namespace std;

enum class NPCType
{
	SALLY,
	HENRY,
	BILLY
};

class NPC : public Entity
{
public:

	NPC(NPCType type);
	virtual ~NPC();

	bool Awake();

	bool Start();

	virtual bool Update(float dt);

	bool CleanUp();

	void SetParameters(pugi::xml_node parameters) {
		this->parameters = parameters;
	}

	void SetPosition(Vector2D pos);

	Vector2D GetPosition();

	float CheckDistance(Vector2D pos);

	void CheckForProgress();

	void CheckForAction();

	void DrawDialog();

public:

	Vector2D OGPosition;

	SDL_Texture* texture;
	SDL_Texture* frontTexture;

	const char* texturePath;
	int texW, texH, FtexH;
	pugi::xml_node parameters;
	Animation* currentAnimation = nullptr;
	Animation idle;
	PhysBody* pbody;

	//For dialogue
	NPCType type;
	unsigned int NextID = 0;
	bool bloked = false;
	string ObjectToGive;

};