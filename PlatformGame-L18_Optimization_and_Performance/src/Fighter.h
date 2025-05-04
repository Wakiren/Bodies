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

	void Attack(Fighter* attacker, Fighter* target);
	void Guard(Fighter* fighter);
	bool isAlive(Fighter* Fighter);
	void TakeDamage(Fighter* fighter, int amount);
	void Heal(Fighter* fighter, int amount);

	CombatStats* combatStats;

};