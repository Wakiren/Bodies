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

Enemy::Enemy() : Entity(EntityType::ENEMY)
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
	position = OGPosition;
	texW = parameters.attribute("w").as_int();
	texH = parameters.attribute("h").as_int();
	speed = parameters.attribute("speed").as_float();
	//Load animations
	idle.LoadAnimations(parameters.child("animations").child("idle"));
	currentAnimation = &idle;
	
	//Add a physics to an item - initialize the physics body
	pbody = Engine::GetInstance().physics.get()->CreateCircle((int)position.getX() + texH / 2, (int)position.getY() + texH / 2, texH / 2, bodyType::DYNAMIC);

	//Assign collider type
	pbody->ctype = ColliderType::ENEMY;

	pbody->listener = this;

	// Set the gravity of the body
	pbody->body->SetGravityScale(0);

	//Combat Stats
	combatStats->attackPoints = parameters.child("combat").attribute("attackPoints").as_int();
	combatStats->health = parameters.child("combat").attribute("health").as_int();
	combatStats->maxHealth = parameters.child("combat").attribute("maxHealth").as_int();
	combatStats->defensePoints = parameters.child("combat").attribute("defensePoints").as_int();

	// Initialize pathfinding
	pathfinding = new Pathfinding();
	Engine::GetInstance().render.get()->DrawTexture(texture, (int)position.getX(), (int)position.getY(), &currentAnimation->GetCurrentFrame());
	ResetPath();

	return true;
}

bool Enemy::Update(float dt)
{
	ZoneScoped;

	if (pbody->body == nullptr)
	{
		return true;
	}


	Vector2D target = Engine::GetInstance().scene.get()->GetPlayerPosition();

	distance.setX(abs(target.getX() - GetPosition().getX()));
	distance.setY(abs(target.getY() - GetPosition().getY()));

	visionLimit = Engine::GetInstance().map.get()->MapToWorld(2, 2);

	if (IsInVision())
	{
		if (check < 20)
		{
			pathfinding->PropagateAStar(SQUARED);
			check += 1;
		}
		else {
			Vector2D tilePos = Engine::GetInstance().map.get()->WorldToMap(GetPosition().getX(), GetPosition().getY());
			pathfinding->ResetPath(tilePos);
			check = 0;
		}
		if (pathfinding->pathTiles.size() > 0) {
			Vector2D nextTile = pathfinding->pathTiles.front();
			Vector2D nextPos = Engine::GetInstance().map->MapToWorld(nextTile.getX(), nextTile.getY());
			Vector2D direction = nextPos - GetPosition();
			direction.normalized();
			eVelocity = b2Vec2(direction.getX() * speed, direction.getY() * speed);
			



			pbody->body->SetLinearVelocity(eVelocity);
		}
		else {
			pbody->body->SetLinearVelocity(b2Vec2_zero);
		}
	}
	else
	{
		pbody->body->SetLinearVelocity(b2Vec2_zero);
	}

	// L08 TODO 4: Add a physics to an item - update the position of the object from the physics.  
	b2Transform pbodyPos = pbody->body->GetTransform();
	position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2);
	position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2);


	Engine::GetInstance().render.get()->DrawTexture(texture, (int)position.getX(), (int)position.getY() - 2, &currentAnimation->GetCurrentFrame());
	currentAnimation->Update();

	if (Engine::GetInstance().physics.get()->debug)
	{
		// Draw pathfinding 
		pathfinding->DrawPath();
	}
	
	return true;
}

bool Enemy::CleanUp()
{
	Engine::GetInstance().physics.get()->DeletePhysBody(pbody);
	return true;
}

void Enemy::SetPosition(Vector2D pos) {
	pos.setX(pos.getX() + texW / 2);
	pos.setY(pos.getY() + texH / 2);
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
		LOG("Collided with player - DESTROY");
		//Engine::GetInstance().entityManager.get()->DestroyEntity(this);
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