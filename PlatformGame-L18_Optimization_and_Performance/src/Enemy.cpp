#include "Enemy.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"
#include "Map.h"
#include "EntityManager.h"
#include "tracy/Tracy.hpp"
#include "window.h"

Enemy::Enemy() : Fighter(EntityType::ENEMY)
{

}

Enemy::~Enemy() {
	delete pathfinding;
}

bool Enemy::Awake() {
	return true;
}

bool Enemy::Start() {

	
	//initilize textures
	texture = Engine::GetInstance().textures.get()->Load(parameters.attribute("texture").as_string());
	frontTexture = Engine::GetInstance().textures.get()->Load(parameters.attribute("frontTexture").as_string());
	position = OGPosition;
	texW = parameters.attribute("w").as_int();
	texH = parameters.attribute("h").as_int();
	speed = parameters.attribute("speed").as_float();

	for (int i = 0; i<ItemsInEnemy.size(); i++)
	{
		if (ItemsInEnemy[i] == "Eye")
		{
			speed = 4.0f; // Eye enemies are faster
		}
	}

	HelpDistance.setX(parameters.attribute("HelpDistance").as_int());
	HelpDistance.setY(parameters.attribute("HelpDistance").as_int());
	//Load animations
	idle.LoadAnimations(parameters.child("animations").child("idle"));
	walk.LoadAnimations(parameters.child("animations").child("walk"));
	currentAnimation = &idle;
	
	//Add a physics to an item - initialize the physics body
	pbody = Engine::GetInstance().physics.get()->CreateCircle((int)position.getX() - 16, (int)position.getY() -16, 16, bodyType::DYNAMIC);

	//Assign collider type
	pbody->ctype = ColliderType::ENEMY;

	pbody->listener = this;

	// Set the gravity of the body
	pbody->body->SetGravityScale(0);

	//Combat Stats
	combatStats = new CombatStats;
	combatStats->attackPoints = parameters.child("combat").attribute("attackPoints").as_int();
	combatStats->health = parameters.child("combat").attribute("health").as_int();
	combatStats->maxHealth = parameters.child("combat").attribute("maxHealth").as_int();
	combatStats->defensePoints = parameters.child("combat").attribute("defensePoints").as_int();

	// Initialize pathfinding
	pathfinding = new Pathfinding();
	Engine::GetInstance().render.get()->DrawTexture(texture, (int)position.getX(), (int)position.getY(), &currentAnimation->GetCurrentFrame());
	ResetPath();

	//Put the items on the enemy
	SetItemsInEnemy();

	return true;
}

bool Enemy::Update(float dt)
{

	return true;
}

bool Enemy::Killed() {

	LOG("Enemy killed");
	return true;
}

bool Enemy::CleanUp()
{
	Engine::GetInstance().physics.get()->DeletePhysBody(pbody);
	return true;
}

void Enemy::SetPosition(Vector2D pos) {
	pos.setX(pos.getX() + (texW / 2));
	pos.setY(pos.getY() + (texH / 2));
	b2Vec2 bodyPos = b2Vec2(PIXEL_TO_METERS(pos.getX()), PIXEL_TO_METERS(pos.getY()));
	pbody->body->SetTransform(bodyPos, 0);
}

Vector2D Enemy::GetPosition() {
	b2Vec2 bodyPos = pbody->body->GetTransform().p;
	Vector2D pos = Vector2D(METERS_TO_PIXELS(bodyPos.x), METERS_TO_PIXELS(bodyPos.y));
	return pos;
}

void Enemy::ResetPath() {
	Vector2D pos = GetPosition();
	Vector2D tilePos = Engine::GetInstance().map.get()->WorldToMap(pos.getX(), pos.getY());
	pathfinding->ResetPath(tilePos);
}

void Enemy::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
	case ColliderType::PLAYER:

		isInCombat = true;
		if (CombatantsFound == true)
		{
			break;
		}
		else
		{
			//for (int i = 0; i < Engine::GetInstance().scene.get()->enemyList.size(); i++)
			//{
			//	Vector2D AllyPos = Engine::GetInstance().scene.get()->enemyList[i]->GetPosition();
			//	if (CheckDistance(AllyPos) < HelpDistance.magnitude())
			//	{
			//		LOG("Combatant found");
			//		Combatants.push_back(Engine::GetInstance().scene.get()->enemyList[i]);
			//	}
			//}
			//CombatantsFound = true;

			//ERROR bc the "function" finds the enemy you just killed and is therefor null, to fix it remove killed enemies from list
		}


		break;
	}
}

void Enemy::OnCollisionEnd(PhysBody* physA, PhysBody* physB)
{
	switch (physB->ctype)
	{
	case ColliderType::PLAYER:
		LOG("Collision player");
		break;
	}
}

bool Enemy::IsInVision()
{
	return distance.getX() <= visionLimit.getX() && distance.getY() <= visionLimit.getY();
}

float Enemy::CheckDistance(Vector2D pos)
{
	Vector2D target = pos;
	Vector2D distance;
	distance.setX(abs(target.getX() - abs(GetPosition().getX())));
	distance.setY(abs(target.getY() - abs(GetPosition().getY())));
	return distance.magnitude();
}

void Enemy::SetItemsInEnemy()
{
	int NumOfItems = Engine::GetInstance().scene.get()->RandomValue(1, 4);
	int num = Engine::GetInstance().scene.get()->RandomValue(0, 10);

	for (int i = 0; i < NumOfItems; i++)
	{
		if (num <= 5)
		{
			ItemsInEnemy.push_back("MixedMeat");
		}
		else if (num < 8 && num > 5)
		{
			ItemsInEnemy.push_back("AnimalSkin");
		}
		else if (num <= 8 && num > 9)
		{
			ItemsInEnemy.push_back("HumanFlesh");
		}
		else
		{
			ItemsInEnemy.push_back("Sizors");
			ItemsInEnemy.push_back("Lighter");
		}
	}	
}

void Enemy::DrawInCombat()
{
	// Draw enemy in combat
	Engine::GetInstance().render.get()->DrawUIimage(frontTexture, (Engine::GetInstance().window.get()->width/2)-((ENEMY_FRONT_TEXTURE_SIZE*4)/2), 0, 4);
}