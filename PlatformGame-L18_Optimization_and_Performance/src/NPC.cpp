#include "NPC.h"
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
#include "Inventory.h"
#include "window.h"
#include "GuiManager.h"

using namespace std;

NPC::NPC(NPCType type) : Entity(EntityType::NPC)
{
	this->type = type;
}

NPC::~NPC() {

}

bool NPC::Awake() {
	return true;
}

bool NPC::Start() {

	//initilize textures
	texture = Engine::GetInstance().textures.get()->Load(parameters.attribute("texture").as_string());
	frontTexture = Engine::GetInstance().textures.get()->Load(parameters.attribute("frontTexture").as_string());
	position.setX(parameters.attribute("x").as_int());
	position.setY(parameters.attribute("y").as_int());
	texW = parameters.attribute("w").as_int();
	texH = parameters.attribute("h").as_int();

	idle.LoadAnimations(parameters.child("animations").child("idle"));
	currentAnimation = &idle;

	//Add a physics to an item - initialize the physics body
	pbody = Engine::GetInstance().physics.get()->CreateCircle((int)position.getX() + texH / 2, (int)position.getY() + texH / 2, texH / 2, bodyType::DYNAMIC);

	//Assign collider type
	pbody->ctype = ColliderType::UNKNOWN;

	pbody->listener = this;

	// Set the gravity of the body
	pbody->body->SetGravityScale(0);

	return true;
}

bool NPC::Update(float dt)
{
	Engine::GetInstance().render.get()->DrawTexture(texture, (int)position.getX(), (int)position.getY() - 2, &currentAnimation->GetCurrentFrame());
	currentAnimation->Update();

	if (CheckDistance(Engine::GetInstance().scene.get()->GetPlayerPosition()) < 100)
	{
		Engine::GetInstance().scene.get()->StartDialogue(*this);
		
		if (ActionTimer.ReadSec() > 30)
		{
			if (type == NPCType::HENRY)
			{
				Engine::GetInstance().audio.get()->PlayRandFx(Effects::COUGHF1, Effects::COUGHF2, Effects::COUGHF3,7);
				ActionTimer.Start();
			}
		}
	}

	CheckForProgress();

	return true;
}

bool NPC::CleanUp()
{
	Engine::GetInstance().physics.get()->DeletePhysBody(pbody);
	return true;
}

void NPC::SetPosition(Vector2D pos) {
	pos.setX(pos.getX() + texW / 2);
	pos.setY(pos.getY() + texH / 2);
	b2Vec2 bodyPos = b2Vec2(PIXEL_TO_METERS(pos.getX()), PIXEL_TO_METERS(pos.getY()));
	pbody->body->SetTransform(bodyPos, 0);
}

Vector2D NPC::GetPosition() {
	b2Vec2 bodyPos = pbody->body->GetTransform().p;
	Vector2D pos = Vector2D(METERS_TO_PIXELS(bodyPos.x), METERS_TO_PIXELS(bodyPos.y));
	return pos;
}

float NPC::CheckDistance(Vector2D pos)
{
	Vector2D target = pos;
	Vector2D distance;
	distance.setX(abs(target.getX() - abs(GetPosition().getX())));
	distance.setY(abs(target.getY() - abs(GetPosition().getY())));
	return distance.magnitude();
}

void NPC::CheckForProgress()
{
	if (Engine::GetInstance().scene.get()->player->inventory == nullptr)
	{
		return;
	}


	for (int i = 0; i < Engine::GetInstance().scene.get()->player->inventory->items.size(); i++)
	{
		if (Engine::GetInstance().scene.get()->player->inventory->items[i]->name == "Eye" && NextID == 1)
		{
			bloked = false;
		}
		if (Engine::GetInstance().scene.get()->player->inventory->items[i]->name == "Alpha" && NextID == 2)
		{
			bloked = false;
		}
		if (Engine::GetInstance().scene.get()->player->inventory->items[i]->name == "Beta" &&
			Engine::GetInstance().scene.get()->player->inventory->items[i]->name == "Omega"&&
			Engine::GetInstance().scene.get()->player->inventory->items[i]->name == "Sigma"
			&& NextID == 3)
		{
			bloked = false;
		}
	}
}

void NPC::CheckForAction()
{
	if (Engine::GetInstance().scene.get()->player->inventory == nullptr)
	{
		return;
	}

	for (int i = 0; i < Engine::GetInstance().scene.get()->player->inventory->items.size(); i++)
	{
		if (Engine::GetInstance().scene.get()->player->inventory->items[i]->name == "Eye" && type == NPCType::BILLY)
		{
			Engine::GetInstance().guiManager.get()->NoEye = false;
		}
	}
}

void NPC::DrawDialog()
{
	// TODO 6: Draw the dialog
	Engine::GetInstance().render.get()->DrawUIimage(frontTexture, 0, Engine::GetInstance().window.get()->height - NPC_FRONT_TEXTURE_Y * 6, 6);
}

