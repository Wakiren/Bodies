#include "Player.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"
#include "EntityManager.h"
#include "tracy/Tracy.hpp"
#include "CombatSystem.h"
#include <cmath>
#include <iostream>
#include "GuiManager.h"
using namespace std;


Player::Player() : Fighter(EntityType::PLAYER)
{
	name = "Player";
}

Player::~Player() {

}

bool Player::Awake() {

	combatStats = new CombatStats;
	return true;
}

bool Player::Start() {

	//L03: TODO 2: Initialize Player parameters
	texture = Engine::GetInstance().textures.get()->Load(parameters.attribute("texture").as_string());
	position.setX(parameters.attribute("x").as_int());
	position.setY(parameters.attribute("y").as_int());
	texW = parameters.attribute("w").as_int();
	texH = parameters.attribute("h").as_int();

	//Load animations
	idle.LoadAnimations(parameters.child("animations").child("idle"));
	walk.LoadAnimations(parameters.child("animations").child("walk"));
	currentAnimation = &idle;

	// L08 TODO 5: Add physics to the player - initialize physics body
	pbody = Engine::GetInstance().physics.get()->CreateCircle((int)position.getX(), (int)position.getY(), texW / 2, bodyType::DYNAMIC);

	// L08 TODO 6: Assign player class (using "this") to the listener of the pbody. This makes the Physics module to call the OnCollision method
	pbody->listener = this;

	// L08 TODO 7: Assign collider type
	pbody->ctype = ColliderType::PLAYER;

	// Set the gravity of the body
	pbody->body->SetGravityScale(0);

	//initialize audio effect
	pickCoinFxId = Engine::GetInstance().audio.get()->LoadFx("Assets/Audio/Fx/retro-video-game-coin-pickup-38299.ogg");

	//Combat Stats
	combatStats->attackPoints = parameters.child("combat").attribute("attackPoints").as_int();
	combatStats->health = parameters.child("combat").attribute("health").as_int();
	combatStats->maxHealth = parameters.child("combat").attribute("maxHealth").as_int();
	combatStats->defensePoints = parameters.child("combat").attribute("defensePoints").as_int();

	// Initialize Inventory
	inventory = new Inventory();

	return true;
}

bool Player::Update(float dt)
{
	ZoneScoped;

	b2Vec2 velocity = b2Vec2(0,0);

	if (!parameters.attribute("gravity").as_bool()) {
		velocity = b2Vec2(0,0);
	}


	b2Transform pbodyPos = pbody->body->GetTransform();
	position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2);
	position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2);

	Engine::GetInstance().render.get()->DrawTexture(texture, (int)position.getX(), (int)position.getY(),
	&currentAnimation->GetCurrentFrame(), 1, spriteAngle);
	currentAnimation->Update();


	if (Engine::GetInstance().dialogueSystem.get()->inDialog == false && openInventory == false)
	{
		MoveToMousePos();
	}

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		Engine::GetInstance().scene.get()->CreateItem("MixedMeat",position);
	}
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_I) == KEY_DOWN)
	{
		if (openInventory == false)
		{
			openInventory = true;
		}
		else
		{
			openInventory = false;
			inventory->ResetInventory();
		}
	}
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_O) == KEY_DOWN)
	{
		inventory->OrganizeInventory();
	}

	return true;
}

bool Player::CleanUp()
{
	LOG("Cleanup player");
	pbody->body->SetLinearVelocity(b2Vec2_zero);
	Engine::GetInstance().textures.get()->UnLoad(texture);
	return true;
}

// L08 TODO 6: Define OnCollision function for the player. 

void Player::OnCollision(PhysBody* physA, PhysBody* physB) {
	
	Vector2D playerPos = GetPosition();
	Vector2D mousePos = Engine::GetInstance().input.get()->GetMousePosition();
	mousePos.setX(mousePos.getX() - Engine::GetInstance().render.get()->camera.x / Engine::GetInstance().render.get()->scale);
	mousePos.setY(mousePos.getY() - Engine::GetInstance().render.get()->camera.y / Engine::GetInstance().render.get()->scale);

	movementVector = vecZero;
	pbody->body->SetLinearVelocity({ 0,0 });
	spriteAngle = atan2(mousePos.getX() - playerPos.getX(), mousePos.getY() - playerPos.getY()) * -180 / b2_pi;
	currentAnimation = &idle;

	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		break;
	case ColliderType::ITEM:
		Engine::GetInstance().audio.get()->PlayFx(pickCoinFxId);

		//Set the Item to the player
		for (int i = 0; i < Engine::GetInstance().scene.get()->itemList.size(); i++)
		{
			if (Engine::GetInstance().scene.get()->itemList[i]->pbody == physB && inventory->isFull == false)
			{
				inventory->AddItem(Engine::GetInstance().scene.get()->itemList[i]);
			}
		}

		//Delete the Item from the scene
		Engine::GetInstance().physics.get()->DeletePhysBody(physB); // Deletes the body of the item from the physics world

		break;
	case ColliderType::UNKNOWN:
		break;
	case ColliderType::ENEMY:
	
		EnemyInCombat = physB;

		for (int i = 0; i < Engine::GetInstance().scene.get()->enemyList.size(); i++)
		{
			if (Engine::GetInstance().scene.get()->enemyList[i]->pbody == physB)
			{
				Engine::GetInstance().combatSystem.get()->actualEnemy = Engine::GetInstance().scene.get()->enemyList[i];
			}
		}

		Fighter* player = new Fighter(pbody->listener->type);
		player->combatStats = combatStats;


		Fighter* enemy = new Fighter(physB->listener->type);
		enemy->combatStats = new CombatStats;
		enemy->combatStats->health = 100;
		enemy->combatStats->attackPoints = 10;
		enemy->combatStats->defensePoints = 0;
		enemy->combatStats->maxHealth = 100;


		Engine::GetInstance().combatSystem.get()->player = (Player*)player;
		Engine::GetInstance().combatSystem.get()->enemy = (Enemy*)enemy;
		cout << "Combat Created" << endl;
		break;
	}
}

void Player::OnCollisionEnd(PhysBody* physA, PhysBody* physB)
{

	switch (physB->ctype)
	{
	case ColliderType::PLATFORM:
		break;
	case ColliderType::ITEM:
		break;
	case ColliderType::UNKNOWN:
		break;
	}
}

void Player::SetPosition(Vector2D pos) {
	pos.setX(pos.getX() + texW / 2);
	pos.setY(pos.getY() + texH / 2);
	b2Vec2 bodyPos = b2Vec2(PIXEL_TO_METERS(pos.getX()), PIXEL_TO_METERS(pos.getY()));
	pbody->body->SetTransform(bodyPos,0);
}

void Player::MoveToMousePos()
{
	Vector2D playerPos = GetPosition();
	Vector2D mousePos = Engine::GetInstance().input.get()->GetMousePosition();
	mousePos.setX(mousePos.getX() - Engine::GetInstance().render.get()->camera.x / Engine::GetInstance().render.get()->scale);
	mousePos.setY(mousePos.getY() - Engine::GetInstance().render.get()->camera.y / Engine::GetInstance().render.get()->scale);

	if (Engine::GetInstance().input.get()->GetMouseButtonDown(1))
	{
		destination = mousePos;
		movementVector = destination - playerPos;
		movementVector = movementVector.normalized();
	}

	if (abs(destination.getX() - playerPos.getX()) > 1 || abs(destination.getY() - playerPos.getY()) > 1)
	{
		spriteAngle = atan2(destination.getX() - playerPos.getX(), destination.getY() - playerPos.getY()) * -180 / b2_pi;
		pbody->body->SetLinearVelocity({ movementVector.getX() * speed, movementVector.getY() * speed });
		currentAnimation = &walk;
	}
	else
	{
		movementVector = vecZero;
		pbody->body->SetLinearVelocity({ 0,0 });
		spriteAngle = atan2(mousePos.getX() - playerPos.getX(), mousePos.getY() - playerPos.getY()) * -180 / b2_pi;
		currentAnimation = &idle;
	}
}

Vector2D Player::GetPosition() {
	b2Vec2 bodyPos = pbody->body->GetTransform().p;
	Vector2D pos = Vector2D(METERS_TO_PIXELS(bodyPos.x), METERS_TO_PIXELS(bodyPos.y));
	return pos;
}

void Player::OnPause()
{
	pbody->body->SetLinearVelocity(b2Vec2_zero);
}