#include "Narcissist.h"
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

Narcissist::Narcissist() : Enemy()
{

}

bool Narcissist::Update(float dt)
{
	ZoneScoped;

	if (pbody->body == nullptr)
	{
		return true;
	}

	if (BackToPath == true)
	{
		GetPath();
		GetClosestPoint();
		BackToPath = false;

	}

	Vector2D target = Engine::GetInstance().scene.get()->GetPlayerPosition();

	distance.setX(abs(target.getX() - GetPosition().getX()));
	distance.setY(abs(target.getY() - GetPosition().getY()));

	visionLimit = Engine::GetInstance().map.get()->MapToWorld(15, 15);

	//IF THE PLAYER IS DETECTED
	if (IsInVision() && !isInCombat)
	{
		if (pathfinding->pathTiles.size() > 0) {

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
		else {
			currentAnimation = &idle;
			pbody->body->SetLinearVelocity(b2Vec2_zero);
			BackToPath = true;
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
	//SCOUTING AND TRAVERSING
	else
	{
		MoveToNextPoint();
		currentAnimation = &walk;
		pbody->body->SetTransform({ pbody->body->GetPosition().x, pbody->body->GetPosition().y }, spriteAngle);
	}

	// L08 TODO 4: Add a physics to an item - update the position of the object from the physics.  
	b2Transform pbodyPos = pbody->body->GetTransform();
	position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2);
	position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2);

	if(InSight == true)
	{
		Engine::GetInstance().render.get()->DrawTexture(texture, (int)position.getX(), (int)position.getY(),
			&currentAnimation->GetCurrentFrame(), 1, spriteAngle);
	}
	currentAnimation->Update();

	if (Engine::GetInstance().physics.get()->debug)
	{
		// Draw pathfinding 
		pathfinding->DrawPath();
	}

	return true;
}

void Narcissist::MoveToNextPoint()
{
	ZoneScoped;
	if (path.empty()) {
		GetPath();
		return; // Or handle the empty path case appropriately
	}
	if (NextPoint >= path.size() - 1)
	{
		NextPoint = 0;
	}

	if (CheckDistance(path[NextPoint])<5.0f)
	{
		NextPoint++;
		spriteAngle = atan2(path[NextPoint].getX() - position.getX(), path[NextPoint].getY() - position.getY()) * -180 / b2_pi;
	}

	Vector2D direction = path[NextPoint] - GetPosition();
	eVelocity = b2Vec2(direction.normalized().getX() * speed, direction.normalized().getY() * speed);

	pbody->body->SetLinearVelocity(eVelocity);
}

Vector2D Narcissist::GetClosestPoint()
{
	ZoneScoped;
	Vector2D closestPoint = path[0];
	float distance = CheckDistance(path[0]);
	for (int i = 0; i < path.size(); i++)
	{
		if (CheckDistance(path[i]) < distance)
		{
			closestPoint = path[i];
			distance = CheckDistance(path[i]);
			NextPoint = i;
		}
	}
	return closestPoint;
}

vector<Vector2D> Narcissist::GetPath()
{
	ZoneScoped;

	path = Engine::GetInstance().map.get()->GetNarPath()->points;

	return path;
}
