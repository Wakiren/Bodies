#pragma once
#include "Enemy.h"
class Cannibal : public Enemy 
{
	public:

		bool Update(float dt) override;
};