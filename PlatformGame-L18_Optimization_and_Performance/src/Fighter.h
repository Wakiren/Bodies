#pragma once
#include "Entity.h"
#include "CombatStats.h"
#include "Engine.h"
#include <iostream>

class Fighter: public Entity
{
	public:
	Fighter(EntityType _type);
	~Fighter();

	void Attack(Fighter* target);
	void Guard();
	bool isAlive();
	void TakeDamage(int amount);
	void Heal(int amount);

	CombatStats* combatStats;
	int damage;
};