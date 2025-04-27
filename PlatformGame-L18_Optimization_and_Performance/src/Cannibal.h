#pragma once
#include "Enemy.h"
class Cannibal : public Enemy 
{
public:

	Cannibal();

	bool Update(float dt) override;

	bool Killed() override;

	EnemyType GetEnemyType()
	{
		return enemyType;
	}

private:
	
	EnemyType enemyType = EnemyType::CANNIBAL;

};