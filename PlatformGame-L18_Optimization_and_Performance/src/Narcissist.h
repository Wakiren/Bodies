#pragma once
#include "Enemy.h"
class Narcissist : public Enemy
{
public:

	Narcissist();

	bool Update(float dt) override;

	EnemyType GetEnemyType()
	{
		return enemyType;
	}

private:

	EnemyType enemyType = EnemyType::NARCISSIST;
};