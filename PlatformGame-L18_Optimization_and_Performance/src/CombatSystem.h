#include "DialogueSystem.h"
#include "Enemy.h"
#include "Player.h"
#include "CombatStats.h"
#include "SString.h"
#include <thread>
#include <chrono>

struct CombatStats;
class CombatSystem : public Module
{
public:
	CombatSystem();
	~CombatSystem();

	// Called before render is available
	bool Awake();
	bool Start();
	bool Update(float dt);
	bool CleanUp();

	void MainLoop();
	void EnemyTurn();
	void PlayerTurn();
	void DeletePhysicalEnemy(PhysBody* enemy);

	//Items After Combat
	void SpawnItems();

	bool isCombatOver(Player* player, Enemy* enemy);

	void displayMessageAfterDelay(const SString& message, int delayInSeconds);

public:

	Player* player = nullptr;
	Enemy* enemy = nullptr;
	Enemy* actualEnemy = nullptr;

	int round;
	bool isPlayerTurn = true;

	bool playerAttacks, EnemyAttacks;
};
