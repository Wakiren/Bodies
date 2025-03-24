#include "CombatSystem.h"

CombatSystem::CombatSystem()
{
}

CombatSystem::~CombatSystem()
{
}

bool CombatSystem::Awake()
{
	return false;
}

bool CombatSystem::Start()
{
	return false;
}

bool CombatSystem::Update(float dt)
{
	MainLoop();
	return false;
}

bool CombatSystem::CleanUp()
{
	return false;
}

void CombatSystem::MainLoop()
{
    if (!isCombatOver(player, enemy)) 
    {
        PlayerTurn();
        if (isCombatOver(player, enemy)) 
        {
            //PlayerAttack(player, enemy);
            //END COMBAT
            cout << "COMBAT OVER" << endl;
        }

        EnemyTurn();
        if (isCombatOver(player, enemy)) 
        {
            //END COMBAT
            cout << "COMBAT OVER" << endl;
        }

        round++;
    }
}

void CombatSystem::EnemyTurn()
{
    //TO IMPLEMENT: ENEMY IA/ BEHAVIOUR
}

void CombatSystem::PlayerTurn()
{
    //PLAYER CHOOSES WHAT TO DO
}

bool CombatSystem::isCombatOver(Player* player, Enemy* enemy)
{
    if (!player->combatStats->isAlive()) {
        cout << "Player defeated!\n";
        return true;
    }
    if (!enemy->combatStats->isAlive()) {
        cout << "Enemy defeated!\n";
        return true;
    }
    return false;

}
