#include "Cannibal.h"
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

Cannibal::Cannibal() : Enemy()
{

}

bool Cannibal::Update(float dt)
{
	ZoneScoped;

	if (pbody->body == nullptr)
	{
		return true;
	}


	Vector2D target = Engine::GetInstance().scene.get()->GetPlayerPosition();

	distance.setX(abs(target.getX() - GetPosition().getX()));
	distance.setY(abs(target.getY() - GetPosition().getY()));

	visionLimit = Engine::GetInstance().map.get()->MapToWorld(10, 10);

	if (IsInVision() && !isInCombat)
	{

		if (pathfinding->pathTiles.size() > 0 ) {

			Vector2D nextTile = pathfinding->pathTiles.back();
			Vector2D nextPos = Engine::GetInstance().map->MapToWorld(nextTile.getX(), nextTile.getY());
			nextPos.setX(nextPos.getX() + (texW / 2));
			nextPos.setY(nextPos.getY() + (texH / 2));
			Vector2D direction = nextPos - GetPosition();
			direction = direction.normalized();

			spriteAngle = atan2(direction.getX(), direction.getY()) * -180 / b2_pi;

			eVelocity = b2Vec2(direction.getX() * speed, direction.getY() * speed);
			currentAnimation = &walk;

			pbody->body->SetLinearVelocity(eVelocity);
		}
		else
		{
			currentAnimation = &idle;
			pbody->body->SetLinearVelocity(b2Vec2_zero);
		}

		ResetPath();
		while (check < 20)
		{
			pathfinding->PropagateAStar(SQUARED);
			check += 1;
		}
		check = 0;

		pbody->body->SetTransform({ pbody->body->GetPosition().x, pbody->body->GetPosition().y }, spriteAngle);
	}
	else
	{
		pbody->body->SetLinearVelocity(b2Vec2_zero);
		ResetPath();
	}

	// L08 TODO 4: Add a physics to an item - update the position of the object from the physics.  
	b2Transform pbodyPos = pbody->body->GetTransform();
	position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2);
	position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2);

	Engine::GetInstance().render.get()->DrawTexture(texture, (int)position.getX(), (int)position.getY(),
		&currentAnimation->GetCurrentFrame(), 1, spriteAngle);
	currentAnimation->Update();

	if (Engine::GetInstance().physics.get()->debug)
	{
		// Draw pathfinding 
		pathfinding->DrawPath();
	}

	return true;
}

bool Cannibal::Killed()
{
	Engine::GetInstance().scene->CreateItem("MixedMeat", position).SpawnFromEnemy();

	return true;
}
