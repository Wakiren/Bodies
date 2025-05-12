#include "TreeBoss.h"
#include "Enemy.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"
#include "Map.h"
#include "EntityManager.h"
#include "tracy/Tracy.hpp"

TreeBoss::TreeBoss() : Enemy()
{

}

bool TreeBoss::Update(float dt)
{

	//This is here for the possibility of the boss being able to move at the end of the game, using him as a normal enemy type

	return true;
}

