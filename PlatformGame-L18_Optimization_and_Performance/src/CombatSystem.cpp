#include "CombatSystem.h"
#include "Fighter.h"
#include "Scene.h"
#include "CombatUI.h"
#include "EntityManager.h"
#include "Physics.h"
#include "Timer.h"


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
                //END COMBAT
                cout << "COMBAT OVER" << endl;
                //std::this_thread::sleep_for(std::chrono::seconds(1));
                Engine::GetInstance().combatui.get()->active = false;
                isPlayerTurn = true;
            }
        }
        else
        {
            EnemyTurn();
            if (isCombatOver(player, enemy))
            {
                //END COMBAT
                cout << "COMBAT OVER" << endl;
                //std::this_thread::sleep_for(std::chrono::seconds(1));
                Engine::GetInstance().combatui.get()->active = false;
                isPlayerTurn = true;
                
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
    cout << "ENEMY ATTACKS!" << endl;
    displayMessageAfterDelay("Enemy Attacks!", 1);
    cout << Engine::GetInstance().combatui.get()->text.GetString() << endl;
    enemy->Attack(enemy, player);
    isPlayerTurn = true;
}

void CombatSystem::PlayerTurn()
{
    switch ((Engine::GetInstance().combatui.get()->combatInput))
    {

    case  CombatUI::CombatInput::ATTACK:
        player->combatStats->isGuarding = false;
        cout << "PLAYER ATTACKS!" << endl;
        displayMessageAfterDelay("Player Attacks!", 1);
        cout << Engine::GetInstance().combatui.get()->text.GetString() << endl;
        player->Attack(player, enemy);
        isPlayerTurn = false;
        break;

    case  CombatUI::CombatInput::GUARD:
        player->combatStats->isGuarding = false;
        cout << "PLAYER GUARDS!" << endl;
        displayMessageAfterDelay("Player Guards!", 1);
        cout << Engine::GetInstance().combatui.get()->text.GetString() << endl;
        player->Guard(player);
        isPlayerTurn = false;

        break;

    case  CombatUI::CombatInput::SKILL:
        player->combatStats->isGuarding = false;
        break;

    case  CombatUI::CombatInput::FLEE:
        break;
        
    default:
        break;
    }
    

    Engine::GetInstance().combatui.get()->combatInput = CombatUI::CombatInput::EMPTY;
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
        displayMessageAfterDelay("Player Defeated!", 1);
        cout << "Player defeated!\n";
        cout << Engine::GetInstance().combatui.get()->text.GetString() << endl;
        Engine::GetInstance().scene.get()->player->isInCombat = false;
        return true;
    }
    if (!enemy->isAlive(enemy))
    {
        if (Engine::GetInstance().scene.get()->player->EnemyInCombat != nullptr) 
        {
            displayMessageAfterDelay("Enemy Defeated!", 1);
            cout << "Enemy defeated!\n";
            cout << Engine::GetInstance().combatui.get()->text.GetString() << endl;
            displayMessageAfterDelay(" ", 1);
        }


        if (Engine::GetInstance().scene.get()->player->EnemyInCombat != nullptr) 
        {
            if (Engine::GetInstance().scene.get()->player->EnemyInCombat->body != nullptr)
            {
				SpawnItems();
                DeletePhysicalEnemy(Engine::GetInstance().scene.get()->player->EnemyInCombat);
                Engine::GetInstance().scene.get()->player->EnemyInCombat = nullptr;
            }
        }

        return true;
    }

    return false;

}

void CombatSystem::SpawnItems()
{
	// Spawn items after combat
	for (int i = 0; i < actualEnemy->ItemsInEnemy.size(); i++)
	{
		Engine::GetInstance().scene.get()->CreateItem(actualEnemy->ItemsInEnemy[i], actualEnemy->position).SpawnFromEnemy();
	}
}

void CombatSystem::displayMessageAfterDelay(const SString& message, int delayInSeconds) 
{
    
    std::this_thread::sleep_for(std::chrono::seconds(delayInSeconds));

    Engine::GetInstance().combatui.get()->text = message;
}

bool CombatSystem::DisplayMessageAfterDelay(const SString& message, int delaySeconds) {
    Timer timer;
    bool messageShown = false;

    if (!initialized) {
        timer.Start(); 
        initialized = true;
    }

    if (!messageShown && timer.ReadSec() >= delaySeconds) {
        Engine::GetInstance().combatui.get()->text = message;
        messageShown = true;
        return true; 
    }

    return false; 
}