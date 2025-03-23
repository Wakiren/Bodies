#include <list>
#include <iostream>
using namespace std;

struct CombatStats {
	int health;
	int maxHealth;
	int attackPoints;
	int defensePoints;
	list <int> abilities;

	bool isAlive() const { return health > 0; }

	void takeDamage(int amount) {
		health -= amount;
		if (health < 0)
		{
			health = 0;
		}
	}
};