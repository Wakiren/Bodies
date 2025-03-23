#include "CombatSystem.h"

CombatSystem::CombatSystem()
{
}

CombatSystem::~CombatSystem()
{
}

bool CombatSystem::Awake()
{
	return false;
}

bool CombatSystem::Start()
{
	return false;
}

bool CombatSystem::Update(float dt)
{
	MainLoop();
	return false;
}

bool CombatSystem::CleanUp()
{
	return false;
}

void CombatSystem::MainLoop()
{

}
