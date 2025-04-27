#pragma once

#include "Module.h"
#include "TreePuzzle.h"
#include <list>

using namespace std;

enum class Puzzle
{
	TREE_PUZZLE,
};

class PuzzleManager : public Module
{
public:

	PuzzleManager();

	// Destructor
	 ~PuzzleManager();

	// Called before render is available
	 bool Awake();

	// Called after Awake
	 bool Start();

	// Called every frame
	 bool Update(float dt);

	// Called before quitting
	 bool CleanUp();

	/////////////////////////////

	Puzzle ActivePuzzle;

private:

	TreePuzzle* treePuzzle = nullptr;

};
