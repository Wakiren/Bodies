#include <list>
#include <iostream>
using namespace std;

struct CombatStats {

	int health;
	int maxHealth;
	int attackPoints;
	int defensePoints;
	bool isGuarding;
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