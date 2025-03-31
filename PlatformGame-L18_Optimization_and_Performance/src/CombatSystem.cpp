#include "CombatSystem.h"

CombatSystem::CombatSystem()
{
    round = 0;
}
CombatSystem::~CombatSystem()
{

}

bool CombatSystem::Awake()
{
    LOG("LOADING COMBAT SYSTEM");
	return true;
}

bool CombatSystem::Start()
{
	return true;
}

bool CombatSystem::Update(float dt)
{
    MainLoop();
	return true;
}

bool CombatSystem::CleanUp()
{
	return true;
}

void CombatSystem::MainLoop()
{
    if (player != nullptr) 
    {
        cout << player << endl;
    }
    if (!isCombatOver(player, enemy)) 
    {
        if(isPlayerTurn) 
        {
            PlayerTurn();
            if (isCombatOver(player, enemy))
            {
                cout << "COMBAT OVER" << endl;
            }
        }
        else
        {
            EnemyTurn();
            if (isCombatOver(player, enemy))
            {
                //END COMBAT
                cout << "COMBAT OVER" << endl;
            }
        }
    }
}

void CombatSystem::EnemyTurn()
{
    cout << "ENEMY TURN:" << endl;
    if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_Y) == KEY_DOWN)
    {
        cout << "ENEMY ATTACKS!" << endl;
        isPlayerTurn = true;
    }
}

void CombatSystem::PlayerTurn()
{
    cout << "PLAYER TURN:" << endl;
    if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
    {
        cout << "PLAYER ATTACKS!" << endl;
        isPlayerTurn = false;
    }
}

bool CombatSystem::isCombatOver(Player* player, Enemy* enemy)
{
    if(player == nullptr || enemy == nullptr)
    {
        return true;
    }
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
