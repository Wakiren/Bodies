#include "Fighter.h"

Fighter::Fighter()
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
    target->combatStats->TakeDamage(damage);
}

void Fighter::Guard(Fighter* fighter)
{
    fighter->combatStats->isGuarding = true;
}
