#include "Fighter.h"
#include "CombatUI.h"
#include "CombatSystem.h"
#include "Engine.h"

Fighter::Fighter(EntityType _type):Entity(_type)
{
   
}

Fighter::~Fighter()
{
}

void Fighter::Attack(Fighter* target)
{
    damage = combatStats->attackPoints - target->combatStats->defensePoints;
    if (damage < 0)
    {
        damage = 0;
    }

	if (target->combatStats->isGuarding != true) 
	{
		target->TakeDamage(damage);
	}
	else 
	{
		
		//Engine::GetInstance().combatSystem.get()->displayMessageAfterDelay("Target was guarding, damage evaded", 2);
		cout << "Target was guarding, damage evaded " << endl;
	}
 
}

void Fighter::Guard()
{
	combatStats->isGuarding = true;
}

bool Fighter::isAlive()
{ 
	if (combatStats == nullptr) 
	{
		return false;
	}
    return combatStats->health > 0; 
}

void Fighter::TakeDamage(int amount) {
	combatStats->health -= amount;
	if (combatStats->health < 0)
	{
		combatStats->health = 0;
	}
}

void Fighter::Heal( int amount) {
	combatStats->health += amount;
	if (combatStats->health > combatStats->maxHealth)
	{
		combatStats->health = combatStats->maxHealth;
	}
}