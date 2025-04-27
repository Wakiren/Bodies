#include "PuzzleManager.h"
#include "Player.h"
#include "Engine.h"
#include "Textures.h"
#include "Scene.h"
#include "Log.h"
#include "Item.h"
#include "tracy/Tracy.hpp"

PuzzleManager::PuzzleManager() : Module()
{

}

// Destructor
PuzzleManager::~PuzzleManager()
{}

// Called before render is available
bool PuzzleManager::Awake()
{
	LOG("Loading PuzzleManager");

	treePuzzle = new TreePuzzle();
	treePuzzle->Awake();

	return true;

}

bool PuzzleManager::Start() {

	bool ret = true;

	treePuzzle->Start();

	return ret;
}

// Called before quitting
bool PuzzleManager::CleanUp()
{
	bool ret = true;

	return ret;
}

bool PuzzleManager::Update(float dt)
{
	bool ret = true;

	ActivePuzzle = Engine::GetInstance().scene.get()->player->PlayerPuzzle;
	treePuzzle->Update(dt);

	if (ActivePuzzle == Puzzle::TREE_PUZZLE) {

	}	

	return ret;
}