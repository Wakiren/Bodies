#pragma once

#include "Module.h"
#include <list>
#include "Physics.h"
#include "Item.h"
#include "Entity.h"

class Symbol;

class TreePuzzle 
{
public:

	TreePuzzle();
	~TreePuzzle();

	bool Awake() ;
	bool Start() ;
	bool Update(float dt) ;
	bool CleanUp() ;

	void DysplayText();
	
	//////////////////////////////////

	list<Symbol*> symbols;
	PhysBody* Altar = nullptr;
};

class Symbol : public Entity
{
public:
	Symbol();
	~Symbol();

	void Start(PhysBody* body, string name_)
	{
		symbolBody = body;
		name = name_;
	}

	bool IsInZone() ;
	bool taken = false;

	/////////

	PhysBody* symbolBody = nullptr;
	string name;
};
