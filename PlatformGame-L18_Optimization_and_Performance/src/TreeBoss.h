#pragma once
#include "Enemy.h"
class TreeBoss : public Enemy
{
public:

	TreeBoss();

	bool Update(float dt) override;

	EnemyType GetEnemyType()
	{
		return enemyType;
	}

private:

	EnemyType enemyType = EnemyType::TREEBOSS;

}; 
