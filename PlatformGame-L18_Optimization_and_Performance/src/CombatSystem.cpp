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
    //MessageFunctionEstaSiQueSi("a");
    //MessageFunctionEstaSiQueSi("b");
    //MessageFunctionEstaSiQueSi("c");
    //MessageFunctionEstaSiQueSi("d");
    //MessageFunctionEstaSiQueSi("e");
    fleed = false;
	return true;
}

bool CombatSystem::Update(float dt)
{
    MessageManager();
    MainLoop();
    //if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_0)) 
    //{
    //    ShowMessages();
    //}
    ShowMessages();
    //if (!initialized && timer.ReadSec() >= delaySeconds) {
    //    Engine::GetInstance().combatui.get()->text = messageToPut;
    //    initialized = true;
    //    cout << "MESSAGE DELIVERED" << endl;
    //    return true;
    //}

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
        for (size_t i = 0; i < messages.size(); i++)
        {
            messages[i] = "";
        }
        Engine::GetInstance().entityManager.get()->UnPauseEntities();
    }
}

void CombatSystem::EnemyTurn()
{
    cout << "ENEMY ATTACKS!" << endl;

    Engine::GetInstance().audio.get()->PlayFx(Effects::ENEMY_HIT,1);

    MessageFunctionEstaSiQueSi("Enemy deals " + to_string(50) + " damage");
    //DisplayMessageAfterDelay(2);
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
        messages.clear();
        player->combatStats->isGuarding = false;
        cout << "PLAYER ATTACKS!" << endl;
       // snprintf(buffer, sizeof(buffer), "Player deals &d damage", player->damage);
        MessageFunctionEstaSiQueSi("Player deals " + to_string(player->combatStats->attackPoints) + " damage");
        timer.Start();

        Engine::GetInstance().combatui.get()->text = messages[0];
        Engine::GetInstance().audio.get()->PlayRandFx(Effects::HIT1, Effects::HIT1, Effects::HIT2, 1);

        //DisplayMessageAfterDelay(1);
        cout << Engine::GetInstance().combatui.get()->text << endl;
        player->Attack(enemy);
        isPlayerTurn = false;
        break;
    }
       

    case  CombatUI::CombatInput::GUARD:
        messages.clear();
        player->combatStats->isGuarding = false;
        cout << "PLAYER GUARDS!" << endl;
		messageToPut = "Player Guards!";

        MessageFunctionEstaSiQueSi("Player guards!");
        timer.Start();
        Engine::GetInstance().combatui.get()->text = messages[0];
		Engine::GetInstance().audio.get()->PlayFx(Effects::EQUIP,2);
        //DisplayMessageAfterDelay(1);
        cout << Engine::GetInstance().combatui.get()->text << endl;
        player->Guard();
        isPlayerTurn = false;

        break;

    case  CombatUI::CombatInput::SKILL:
        player->combatStats->isGuarding = false;
        break;

    case  CombatUI::CombatInput::FLEE:
        messages.clear();
        Engine::GetInstance().audio.get()->PlayFx(Effects::SAM_EXHAUST,3);
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
        //DisplayMessageAfterDelay(1);
        cout << "Player defeated!\n";
        cout << Engine::GetInstance().combatui.get()->text << endl;
        player->isInCombat = false;
        Engine::GetInstance().scene.get()->HandleAudio();
        dead = true;
        return true;
    }
    if (!enemy->isAlive())
    {
        for (size_t i = 0; i < messages.size(); i++)
        {
            messages[i] = "";
        }
        if (Engine::GetInstance().scene.get()->player->EnemyInCombat != nullptr) 
        {
            //messageToPut = "Enemy Defeated!";
            //DisplayMessageAfterDelay(1);
            cout << "Enemy defeated!\n";
            cout << Engine::GetInstance().combatui.get()->text << endl;
        }

       player->isInCombat = false;
       Engine::GetInstance().scene.get()->HandleAudio();


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

void CombatSystem::MessageFunctionEstaSiQueSi(string message)
{
    messages.push_back(message);
}

void CombatSystem::MessageManager()
{
    cout << timer.ReadSec() << endl;
    if (messages.size() > 0 && !locked)
    {
        Engine::GetInstance().combatui.get()->text = messages[0];
        locked = true;
        timer.Start();
    }
    if (timer.ReadSec() >= 1) 
    {
        if (messages.size() > 0)
        {
            messages.erase(messages.begin());
            locked = false;
        }
    }
}

void CombatSystem::ShowMessages()
{
    for (size_t i = 0; i < messages.size(); i++)
    {
        cout << "Message " << i << ": " << messages[i] << endl;
    }
    cout << endl;
}
