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

public:

	Enemy* enemy;
	Player* player;

	vector<int> a;
	bool playerTurn = true;
};
