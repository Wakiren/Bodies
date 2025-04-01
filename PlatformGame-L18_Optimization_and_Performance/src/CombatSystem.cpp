#include "CombatSystem.h"
#include "Fighter.h"

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
    if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
    {
        cout << "ENEMY ATTACKS!" << endl;
        enemy->Attack(enemy, player);
        isPlayerTurn = true;
    }
}

void CombatSystem::PlayerTurn()
{
    if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_P) == KEY_DOWN)
    {
        cout << "PLAYER ATTACKS!" << endl;
        player->Attack(player, enemy);
        isPlayerTurn = false;
    }
}

bool CombatSystem::isCombatOver(Player* player, Enemy* enemy)
{
    if(player == nullptr || enemy == nullptr)
    {
        return true;
    }
    if (!player->isAlive(player)) {
        cout << "Player defeated!\n";
        return true;
    }
    if (!enemy->isAlive(player)) {
        cout << "Enemy defeated!\n";
        return true;
    }

    return false;

}
