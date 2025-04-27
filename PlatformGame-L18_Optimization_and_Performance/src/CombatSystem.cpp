#include "CombatSystem.h"
#include "Fighter.h"
#include "Scene.h"
#include "CombatUI.h"
#include "EntityManager.h"
#include "Physics.h"


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
        Engine::GetInstance().entityManager.get()->PauseEntities();
        Engine::GetInstance().combatui.get()->active = true;
        if(isPlayerTurn) 
        {
            Engine::GetInstance().scene.get()->player->isInCombat = true;
            PlayerTurn();
            if (isCombatOver(player, enemy))
            {
                cout << "COMBAT OVER" << endl;
                Engine::GetInstance().combatui.get()->active = false;
            }
        }
        else
        {
            EnemyTurn();
            if (isCombatOver(player, enemy))
            {
                //END COMBAT
                cout << "COMBAT OVER" << endl;
                Engine::GetInstance().combatui.get()->active = false;
            }
        }
    }
    else 
    {
        Engine::GetInstance().entityManager.get()->UnPauseEntities();
    }
}

void CombatSystem::EnemyTurn()
{
    /*if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
    {
        cout << "ENEMY ATTACKS!" << endl;
        enemy->Attack(enemy, player);
        isPlayerTurn = true;
    }*/
    cout << "ENEMY ATTACKS!" << endl;
    enemy->Attack(enemy, player);
    isPlayerTurn = true;
}

void CombatSystem::PlayerTurn()
{
    if (Engine::GetInstance().combatui.get()->combatInput == CombatUI::CombatInput::ATTACK)
    {
        cout << "PLAYER ATTACKS!" << endl;
        player->Attack(player, enemy);
        isPlayerTurn = false;
        Engine::GetInstance().combatui.get()->combatInput = CombatUI::CombatInput::EMPTY;
        cout << "inputValue" << Engine::GetInstance().combatui.get()->combatInput << endl;
    }
}

void CombatSystem::DeletePhysicalEnemy(PhysBody* enemy)
{
    Engine::GetInstance().physics.get()->DeletePhysBody(enemy);
}

bool CombatSystem::isCombatOver(Player* player, Enemy* enemy)
{
    if(player == nullptr || enemy == nullptr)
    {
        return true;
    }
    if (!player->isAlive(player)) {
        cout << "Player defeated!\n";
        Engine::GetInstance().scene.get()->player->isInCombat = false;
        return true;
    }
    if (!enemy->isAlive(enemy)) {
        cout << "Enemy defeated!\n";

        if (Engine::GetInstance().scene.get()->player->EnemyInCombat != nullptr) 
        {
            if (Engine::GetInstance().scene.get()->player->EnemyInCombat->body != nullptr)
            {
                DeletePhysicalEnemy(Engine::GetInstance().scene.get()->player->EnemyInCombat);
                Engine::GetInstance().scene.get()->player->EnemyInCombat = nullptr;

            }
        }

        Engine::GetInstance().scene.get()->player->isInCombat = false;
        return true;
    }

    return false;

}
