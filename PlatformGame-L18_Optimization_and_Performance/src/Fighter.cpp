#include "Fighter.h"

Fighter::Fighter(EntityType _type):Entity(_type)
{
   
}

Fighter::~Fighter()
{
}

void Fighter::Attack(Fighter* attacker, Fighter* target)
{
    int damage = attacker->combatStats->attackPoints - target->combatStats->defensePoints;
    if (damage < 0)
    {
        damage = 0;
    }
    target->TakeDamage(target, damage);
}

void Fighter::Guard(Fighter* fighter)
{
    fighter->combatStats->isGuarding = true;
}

bool Fighter::isAlive(Fighter* fighter)
{ 
    return fighter->combatStats->health > 0; 
}

void Fighter::TakeDamage(Fighter* fighter, int amount) {
	fighter->combatStats->health -= amount;
	if (fighter->combatStats->health < 0)
	{
		fighter->combatStats->health = 0;
	}
}

void Fighter::Heal(Fighter* fighter, int amount) {
	fighter->combatStats->health += amount;
	if (fighter->combatStats->health > fighter->combatStats->maxHealth)
	{
		fighter->combatStats->health = fighter->combatStats->maxHealth;
	}
}