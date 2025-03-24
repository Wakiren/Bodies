#pragma once
#include "Enemy.h"
#include "Player.h"
#include "CombatStats.h"
#include <iostream>

class Fighter 
{
	public:
	Fighter();
	~Fighter();

	void Attack(Fighter attacker, Fighter Target);

	string name;
	CombatStats* combatStats;
};