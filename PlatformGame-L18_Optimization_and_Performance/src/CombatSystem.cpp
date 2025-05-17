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
    if (!isCombatOver(player, enemy)) 
    {
        Engine::GetInstance().entityManager.get()->PauseEntities();
        Engine::GetInstance().combatui.get()->active = true;
        actualEnemy->DrawInCombat();
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

    messageToPut = "Enemy deals " + to_string(50) + " damage";

    DisplayMessageAfterDelay(1);
    cout << Engine::GetInstance().combatui.get()->text << endl;
    enemy->Attack(enemy, player);
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
        player->Attack(player, enemy);
        isPlayerTurn = false;
        break;
    }
       

    case  CombatUI::CombatInput::GUARD:
        player->combatStats->isGuarding = false;
        cout << "PLAYER GUARDS!" << endl;
		messageToPut = "Player Guards!";
        DisplayMessageAfterDelay(1);
        cout << Engine::GetInstance().combatui.get()->text << endl;
        player->Guard(player);
        isPlayerTurn = false;

        break;

    case  CombatUI::CombatInput::SKILL:
        player->combatStats->isGuarding = false;
        break;

    case  CombatUI::CombatInput::FLEE:
        player->combatStats->isGuarding = false;
		player->isInCombat = false;
        enemy->isInCombat = false;
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
    Engine::GetInstance().scene.get()->player->isInCombat = false;
    if(player == nullptr || enemy == nullptr)
    {
        return true;
    }
    if (fleed) 
    {
        return true;
    }
    if (!player->isAlive(player)) {
		messageToPut = "Player Defeated!";
        DisplayMessageAfterDelay(1);
        cout << "Player defeated!\n";
        cout << Engine::GetInstance().combatui.get()->text << endl;
        Engine::GetInstance().scene.get()->player->isInCombat = false;
        Engine::GetInstance().scene.get()->HandleAudio();
        return true;
    }
    if (!enemy->isAlive(enemy))
    {
        if (Engine::GetInstance().scene.get()->player->EnemyInCombat != nullptr) 
        {
            messageToPut = "Enemy Defeated!";
            DisplayMessageAfterDelay(1);
            cout << "Enemy defeated!\n";
            cout << Engine::GetInstance().combatui.get()->text << endl;
			Engine::GetInstance().scene.get()->HandleAudio();
        }

        Engine::GetInstance().scene.get()->player->isInCombat = false;

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

void CombatSystem::DisplayMessageAfterDelay(int delaySeconds_) {

        delaySeconds = delaySeconds_;
        timer.Start(); 
        cout << "currentTime: " << timer.ReadSec();
        cout << "toDO: " << delaySeconds_;
        initialized = false;
}