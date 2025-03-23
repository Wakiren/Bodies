#ifndef __COMBATSTATS_H__
#define __COMBATSTATS_H__

#include <list>
#include <iostream>
using namespace std;

struct CombatStats {

	int health = 0;
	int maxHealth = 0;
	int attackPoints = 0;
	int defensePoints = 0;
	bool isGuarding = false;
	list <int> abilities;

	bool isAlive() const { return health > 0; }

	void TakeDamage(int amount) {
		health -= amount;
		if (health < 0)
		{
			health = 0;
		}
	}

	void Heal(int amount) {
		health += amount;
		if (health > maxHealth)
		{
			health = maxHealth;
		}
	}
};

#endif