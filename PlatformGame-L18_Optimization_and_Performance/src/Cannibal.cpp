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

	visionLimit = Engine::GetInstance().map.get()->MapToWorld(2, 2);

	if (IsInVision() && !isInCombat)
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