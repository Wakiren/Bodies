#pragma once
#include "Enemy.h"
class Narcissist : public Enemy
{
public:

	Narcissist();

	bool Update(float dt) override;

	void MoveToNextPoint();

	Vector2D GetClosestPoint();

	vector<Vector2D> GetPath();

	EnemyType GetEnemyType()
	{
		return enemyType;
	}

private:

	EnemyType enemyType = EnemyType::NARCISSIST;

	int NextPoint;
	vector<Vector2D> path;
	
	bool BackToPath = true;
};