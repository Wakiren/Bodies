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
    fleed = false;
	return true;
}

bool CombatSystem::Update(float dt)
{
    MainLoop();
    if (!initialized && timer.ReadSec() >= delaySeconds) {
        Engine::GetInstance().combatui.get()->text = messageToPut;
        initialized = true;
        cout << "MESSAGE DELIVERED" << endl;
        return true;
    }

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
        if (!player->isInCombat) 
        {
            Engine::GetInstance().entityManager.get()->UnPauseEntities();
            return;
        }
    }
    else 
    {
        return;
    }
    if (!isCombatOver(player, enemy)) 
    {
        Engine::GetInstance().entityManager.get()->PauseEntities();
        Engine::GetInstance().combatui.get()->active = true;
        actualEnemy->DrawInCombat();
        if(isPlayerTurn) 
        {
            player->isInCombat = true;
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

    messageToPut = "Enemy deals " + to_string(50) + " damage";

    DisplayMessageAfterDelay(2);
    cout << Engine::GetInstance().combatui.get()->text << endl;
    enemy->Attack(player);
    isPlayerTurn = true;
}

void CombatSystem::PlayerTurn()
{
    switch ((Engine::GetInstance().combatui.get()->combatInput))
    {

    case  CombatUI::CombatInput::ATTACK:
    {
        player->combatStats->isGuarding = false;
        cout << "PLAYER ATTACKS!" << endl;
       // snprintf(buffer, sizeof(buffer), "Player deals &d damage", player->damage);

		messageToPut = "Player deals " + to_string(player->combatStats->attackPoints) + " damage";

        DisplayMessageAfterDelay(1);
        cout << Engine::GetInstance().combatui.get()->text << endl;
        player->Attack(enemy);
        isPlayerTurn = false;
        break;
    }
       

    case  CombatUI::CombatInput::GUARD:
        player->combatStats->isGuarding = false;
        cout << "PLAYER GUARDS!" << endl;
		messageToPut = "Player Guards!";
        DisplayMessageAfterDelay(1);
        cout << Engine::GetInstance().combatui.get()->text << endl;
        player->Guard();
        isPlayerTurn = false;

        break;

    case  CombatUI::CombatInput::SKILL:
        player->combatStats->isGuarding = false;
        break;

    case  CombatUI::CombatInput::FLEE:

        fleed = true;
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
    if (fleed) 
    {
        enemy->timeStopped = 2000;

        player->combatStats->isGuarding = false;
        enemy->isInCombat = false;
        player->isInCombat = false;
        Engine::GetInstance().scene.get()->HandleAudio();
        player->EnemyInCombat = nullptr;
        enemy = nullptr;
        fleed = false;
        return true;
    }
    if (!player->isAlive()) {
		messageToPut = "Player Defeated!";
        DisplayMessageAfterDelay(1);
        cout << "Player defeated!\n";
        cout << Engine::GetInstance().combatui.get()->text << endl;
        player->isInCombat = false;
        Engine::GetInstance().scene.get()->HandleAudio();
        return true;
    }
    if (!enemy->isAlive())
    {
        if (Engine::GetInstance().scene.get()->player->EnemyInCombat != nullptr) 
        {
            messageToPut = "Enemy Defeated!";
            DisplayMessageAfterDelay(1);
            cout << "Enemy defeated!\n";
            cout << Engine::GetInstance().combatui.get()->text << endl;
			Engine::GetInstance().scene.get()->HandleAudio();
        }

       player->isInCombat = false;

        if (player->EnemyInCombat != nullptr) 
        {
            if (player->EnemyInCombat->body != nullptr)
            {
				SpawnItems();
                DeletePhysicalEnemy(player->EnemyInCombat);
                player->EnemyInCombat = nullptr;
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

void CombatSystem::DisplayMessageAfterDelay(int delaySeconds_) {

        delaySeconds = delaySeconds_;
        timer.Start(); 
        cout << "currentTime: " << timer.ReadSec();
        cout << "toDO: " << delaySeconds_;
        initialized = false;
}