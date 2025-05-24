#pragma once

#include "Fighter.h"
#include "SDL2/SDL.h"
#include "Box2D/Box2D.h"
#include "Animation.h"
#include "CombatStats.h"
#include "PuzzleManager.h"
#include "Inventory.h"
#include "Enemy.h"
#include <vector>
#include <string>
#include "Timer.h"

struct CombatStats;
struct SDL_Texture;
class Inventory;

#define SIGHT_DISTANCE 400

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

	void MoveToMousePos();

	Vector2D GetPosition();

	void OnPause() override;

	void EnterCombatWith(Enemy* enemy_);

public:


	//Declare player parameters
	float speed = 3.0f;
	SDL_Texture* texture = NULL;
	int texW, texH;

	//Audio fx
	int pickCoinFxId;


	PhysBody* pbody;
	PhysBody* sight;
	float jumpForce = 2.5f; // The force to apply when jumping
	bool isJumping = false; // Flag to check if the player is currently jumping
	bool isInCombat = false;

	pugi::xml_node parameters;
	Animation* currentAnimation = nullptr;
	Animation idle;
	Animation walk;

	Puzzle PlayerPuzzle = Puzzle::TREE_PUZZLE;
	PhysBody* EnemyInCombat = nullptr;

	Inventory* inventory = nullptr;
	bool openInventory = false;
	double sightAngle;
	bool canInteract = false;

private:

	const Vector2D vecZero = {0,0};
	Vector2D destination;
	Vector2D movementVector = {0,0};
	Timer* time = nullptr;
	double spriteAngle;
};