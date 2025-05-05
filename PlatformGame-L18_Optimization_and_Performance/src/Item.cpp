#include "Item.h"
#include "Engine.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Physics.h"
#include "Window.h"

using namespace std;

Item::Item() : Entity(EntityType::ITEM)
{
	name = "item";
}

Item::~Item() {}

bool Item::Awake() {
	return true;
}

bool Item::Start() {

	//initilize textures
	texture = Engine::GetInstance().textures.get()->Load(parameters.attribute("texture").as_string());
	Hover = Engine::GetInstance().textures.get()->Load("Assets/Items/Hover.png");
	Selected = Engine::GetInstance().textures.get()->Load("Assets/Items/Selected.png");
	position.setX(parameters.attribute("x").as_int());
	position.setY(parameters.attribute("y").as_int());
	texW = parameters.attribute("w").as_int();
	texH = parameters.attribute("h").as_int();

	//Load animations
	idle.LoadAnimations(parameters.child("animations").child("idle"));
	currentAnimation = &idle;

	//Load atributes
	name = parameters.attribute("name").as_string();
  	type = parameters.child("effect").attribute("type").as_string();
	amount = parameters.child("effect").attribute("amount").as_int();
	
	// L08 TODO 4: Add a physics to an item - initialize the physics body
	pbody = Engine::GetInstance().physics.get()->CreateCircle((int)position.getX() + texH / 2, (int)position.getY() + texH / 2, texH / 2, bodyType::DYNAMIC);

	// L08 TODO 7: Assign collider type
	pbody->ctype = ColliderType::ITEM;

	// Set the gravity of the body
	if (!parameters.attribute("gravity").as_bool()) pbody->body->SetGravityScale(0);



	return true;
}

bool Item::Update(float dt)
{
	// L08 TODO 4: Add a physics to an item - update the position of the object from the physics.  
	if (pbody->body == nullptr) return true;

	b2Transform pbodyPos = pbody->body->GetTransform();
	position.setX(METERS_TO_PIXELS(pbodyPos.p.x) - texH / 2);
	position.setY(METERS_TO_PIXELS(pbodyPos.p.y) - texH / 2);

	Engine::GetInstance().render.get()->DrawTexture(texture, (int)position.getX(), (int)position.getY(), &currentAnimation->GetCurrentFrame());
	currentAnimation->Update();

	Stabilize();

	return true;
}

bool Item::CleanUp()
{
	return true;
}

void Item::SpawnFromEnemy()
{
	angle = Engine::GetInstance().scene.get()->RandomValue(0, 360);

	b2Vec2 Direction = b2Vec2(cos(angle)*Vel, sin(angle)*Vel);

	pbody->body->ApplyForceToCenter(Direction,true);
}

void Item::SetPosition(Vector2D pos) {
	pos.setX(pos.getX() + texW / 2);
	pos.setY(pos.getY() + texH / 2);
	b2Vec2 bodyPos = b2Vec2(PIXEL_TO_METERS(pos.getX()), PIXEL_TO_METERS(pos.getY()));
	pbody->body->SetTransform(bodyPos, 0);
}

void Item::Stabilize()
{
	if (abs(pbody->body->GetLinearVelocity().Length()) > 0.1f)
	{
		b2Vec2 Vel = pbody->body->GetLinearVelocity();
		pbody->body->ApplyForceToCenter(b2Vec2(-Vel.x/5, -Vel.y/5), true);
	}
	else
	{
		pbody->body->SetLinearVelocity(b2Vec2_zero);
	}
}

void Item::DrawInInventory(Vector2D pos, int sacale)
{
	Engine::GetInstance().render.get()->DrawUIimage(texture, (int)pos.getX(), (int)pos.getY(), sacale, &currentAnimation->GetCurrentFrame());
	currentAnimation->Update();

	Vector2D mPos = Engine::GetInstance().input->GetMousePosition() * Engine::GetInstance().window.get()->GetScale();

	//Hover
	if (mPos.getX() > pos.getX() && mPos.getX() < pos.getX() + texW * sacale &&
		mPos.getY() > pos.getY() && mPos.getY() < pos.getY() + texH * sacale)
	{
		Engine::GetInstance().render.get()->DrawUIimage(Hover, (int)pos.getX(), (int)pos.getY(), sacale);
	}
	if (Engine::GetInstance().input.get()->GetMouseButtonDown(1) && 
		mPos.getX() > pos.getX() && mPos.getX() < pos.getX() + texW * sacale &&
		mPos.getY() > pos.getY() && mPos.getY() < pos.getY() + texH * sacale)
	{
		InventorySelected = true;
	}
	else if (Engine::GetInstance().input.get()->GetMouseButtonDown(1))
	{
		InventorySelected = false;
	}

	if (InventorySelected == true)
	{
		Engine::GetInstance().render.get()->DrawUIimage(Selected, (int)pos.getX(), (int)pos.getY(), sacale);
		Engine::GetInstance().render.get()->DrawUIimage(texture, ITEM_POS_X, ITEM_POS_Y, PREVIEW_SCALE, &currentAnimation->GetCurrentFrame());
	}
}