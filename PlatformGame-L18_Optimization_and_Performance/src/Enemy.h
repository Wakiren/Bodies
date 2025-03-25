#pragma once

#include "Entity.h"
#include "SDL2/SDL.h"
#include "Box2D/Box2D.h"
#include "Animation.h"
#include "Pathfinding.h"
#include "CombatStats.h"
#include "Fighter.h"

struct CombatStats;
struct SDL_Texture;

class Enemy : public Fighter
{
public:

	Enemy();
	virtual ~Enemy();

	bool Awake();

	bool Start();

	virtual bool Update(float dt);

	bool CleanUp();

	void SetParameters(pugi::xml_node parameters) {
		this->parameters = parameters;
	}

	void SetPosition(Vector2D pos);

	Vector2D GetPosition();

	void ResetPath();

	virtual void OnCollision(PhysBody* physA, PhysBody* physB);

	void OnCollisionEnd(PhysBody* physA, PhysBody* physB);

	bool IsInVision();

	float CheckDistance(Vector2D pos);

	std::vector<Enemy*> GetNumOfCombatants()
	{
		return Combatants;
	}

public:

	CombatStats* combatStats = new CombatStats;
	Vector2D OGPosition;

	SDL_Texture* texture;
	const char* texturePath;
	int texW, texH;
	pugi::xml_node parameters;
	Animation* currentAnimation = nullptr;
	Animation idle;
	PhysBody* pbody;
	Pathfinding* pathfinding;

	bool propagatePath = false;

	b2Vec2 eVelocity;
	Vector2D distance;
	Vector2D visionLimit;
	Vector2D HelpDistance;
	float speed;

	int check = 0;

	enum class EnemyType
	{
		CANNIBAL,
		NARCISSIST
	};

	//Search for combatants
	std::vector<Enemy*> Combatants;
	bool CombatantsFound = false;
};
