#pragma once

#include "Fighter.h"
#include "SDL2/SDL.h"
#include "Box2D/Box2D.h"
#include "Animation.h"
#include "CombatStats.h"


struct CombatStats;
struct SDL_Texture;

class Player : public Fighter
{
public:

	Player();
	
	virtual ~Player();

	bool Awake();

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	// L08 TODO 6: Define OnCollision function for the player. 
	void OnCollision(PhysBody* physA, PhysBody* physB);

	void OnCollisionEnd(PhysBody* physA, PhysBody* physB);

	void SetParameters(pugi::xml_node parameters) {
		this->parameters = parameters;
	}

	void SetPosition(Vector2D pos);

	void MoveToMousePos(float speed);

	Vector2D GetPosition();

public:


	//Declare player parameters
	float speed = 10.0f;
	SDL_Texture* texture = NULL;
	int texW, texH;

	//Audio fx
	int pickCoinFxId;


	PhysBody* pbody;
	float jumpForce = 2.5f; // The force to apply when jumping
	bool isJumping = false; // Flag to check if the player is currently jumping
	bool isInCombat = false;

	pugi::xml_node parameters;
	Animation* currentAnimation = nullptr;
	Animation idle;

private:

	const Vector2D vecZero = {0,0};
	Vector2D destination;
	Vector2D movementVector = {0,0};

	double spriteAngle;


};