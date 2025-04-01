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

};

#endif