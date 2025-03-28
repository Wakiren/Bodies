#pragma once
#include "Entity.h"
#include "CombatStats.h"
#include <iostream>

class Fighter: public Entity
{
	public:
	Fighter(EntityType _type);
	~Fighter();

	void Attack(Fighter* attacker, Fighter* target);
	void Guard(Fighter* fighter);

	CombatStats* combatStats;

};