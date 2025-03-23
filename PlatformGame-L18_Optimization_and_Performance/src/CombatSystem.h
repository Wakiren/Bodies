#include "DialogueSystem.h"
#include "Enemy.h"
#include "Player.h"

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

	//THESE FUNCTIONS ARE REDUNDANT AND TEMPORARY, MUST FIND A WAY TO UNIFY
	void PlayerAttack(Player& player, Enemy& enemy);
	void EnemyAttack(Player& player, Enemy& enemy);
	void Guard();


public:

	Enemy* enemy;
	Player* player;

	bool isCombatOver(Player* player, Enemy* enemy);
	int round;
};
