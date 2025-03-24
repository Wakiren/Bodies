#include "DialogueSystem.h"
#include "Enemy.h"
#include "Player.h"
#include "CombatStats.h"

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


public:

	Enemy* enemy;
	Player* player;

	bool isCombatOver(Player* player, Enemy* enemy);
	int round;
};
