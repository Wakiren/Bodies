#include "Engine.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "Scene.h"
#include "Log.h"
#include "Entity.h"
#include "EntityManager.h"
#include "Player.h"
#include "Map.h"
#include "Item.h"
#include "Enemy.h"
#include "Narcissist.h"
#include "Cannibal.h"
#include "NPC.h"
#include "GuiControl.h"
#include "GuiManager.h"
#include "DialogueSystem.h"
#include "Map.h"
#include "PauseMenu.h"

Scene::Scene() : Module()
{
	name = "scene";
}

// Destructor
Scene::~Scene()
{}

// Called before render is available
bool Scene::Awake()
{
	LOG("Loading Scene");
	bool ret = true;

	//L04: TODO 3b: Instantiate the player using the entity manager
	player = (Player*)Engine::GetInstance().entityManager->CreateEntity(EntityType::PLAYER, "");
	player->SetParameters(configParameters.child("entities").child("player"));
	
	//L08 Create a new item using the entity manager and set the position to (200, 672) to test
	for(pugi::xml_node itemNode = configParameters.child("entities").child("items").child("item"); itemNode; itemNode = itemNode.next_sibling("item"))
	{
		Item* item = (Item*)Engine::GetInstance().entityManager->CreateEntity(EntityType::ITEM, itemNode.name());
		item->SetParameters(itemNode);
	}
	for (pugi::xml_node NPCnode = configParameters.child("entities").child("NPCs").child("NPC"); NPCnode; NPCnode = NPCnode.next_sibling("NPC"))
	{
		NPC* npc = (NPC*)Engine::GetInstance().entityManager->CreateEntity(EntityType::NPC, NPCnode.attribute("name").as_string());
		npc->SetParameters(NPCnode);
	}

	// L16: TODO 2: Instantiate a new GuiControlButton in the Scene
	/*SDL_Rect btPos = { 520, 350, 120,20 };
	guiBt = (GuiControlButton*) Engine::GetInstance().guiManager->CreateGuiControl(GuiControlType::BUTTON, 1, "MyButton", btPos, this);*/

	return ret;
}

// Called before the first frame
bool Scene::Start()
{

	//L06 TODO 3: Call the function to load the map. 
	Engine::GetInstance().map->Load(configParameters.child("map").attribute("path").as_string(), configParameters.child("map").attribute("name").as_string());

	// Texture to highligh mouse position 
	mouseTileTex = Engine::GetInstance().textures.get()->Load("Assets/Maps/MapMetadata.png");

	// Initalize the camera position
	int w, h;
	Engine::GetInstance().window.get()->GetWindowSize(w, h);
	Engine::GetInstance().render.get()->camera.x = 0;
	Engine::GetInstance().render.get()->camera.y = 0;

	WWidth = Engine::GetInstance().window.get()->width;
	WHeight = Engine::GetInstance().window.get()->height;

	if (Engine::GetInstance().map.get()->GetDataLayer() != nullptr)
	{
		for (int i = 0; i < Engine::GetInstance().map.get()->GetDataLayer()->width; i++)
		{
			for (int j = 0; j < Engine::GetInstance().map.get()->GetDataLayer()->height; j++)
			{
				//CANNIBALS
				if (Engine::GetInstance().map.get()->GetDataLayer()->Get(i, j) == 1951)
				{
					pugi::xml_node enemyNode = configParameters.child("entities").child("enemies").child("Cannibal");
					
					Vector2D enemyPos = Engine::GetInstance().map->MapToWorld(i, j);

					if (RandomValue(1, enemyNode.attribute("SpawnRate").as_int()) == 1)
					{
						Cannibal* enemy = (Cannibal*)Engine::GetInstance().entityManager->CreateEntity(EntityType::ENEMY, enemyNode.name());
						enemy->SetParameters(enemyNode);
						enemy->OGPosition = enemyPos;
						enemyList.push_back(enemy);
					}
				}

				//NARCISSISTS
				if (Engine::GetInstance().map.get()->GetDataLayer()->Get(i, j) == 1952)
				{
					pugi::xml_node enemyNode = configParameters.child("entities").child("enemies").child("Narcissist");

					Vector2D enemyPos = Engine::GetInstance().map->MapToWorld(i, j);

					if (RandomValue(1, enemyNode.attribute("SpawnRate").as_int()) == 1)
					{
						Narcissist* enemy = (Narcissist*)Engine::GetInstance().entityManager->CreateEntity(EntityType::ENEMY, enemyNode.name());
						enemy->SetParameters(enemyNode);
						enemy->OGPosition = enemyPos;
						enemyList.push_back(enemy);
					}
				}
			}
		}
	}




	return true;
}

// Called each loop iteration
bool Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool Scene::Update(float dt)
{
	
	//L03 TODO 3: Make the camera movement independent of framerate
	float camSpeed = 2;
	int scale = Engine::GetInstance().window.get()->GetScale();


	if (Engine::GetInstance().physics.get()->debug) {
		if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
			Engine::GetInstance().render.get()->camera.y += ceil(camSpeed * dt);

		if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
			Engine::GetInstance().render.get()->camera.y -= ceil(camSpeed * dt);

		if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
			Engine::GetInstance().render.get()->camera.x += ceil(camSpeed * dt);

		if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
			Engine::GetInstance().render.get()->camera.x -= ceil(camSpeed * dt);
	}

	Vector2D mapLimit = Engine::GetInstance().map->MapToWorld(Engine::GetInstance().map->GetWidth(), Engine::GetInstance().map->GetHeight());

	if (!Engine::GetInstance().physics.get()->debug) {

		Engine::GetInstance().render.get()->camera.y = Slower(Engine::GetInstance().render.get()->camera.y, (-player->position.getY() * scale) + WHeight / 2, 0.2f);

		Engine::GetInstance().render.get()->camera.x = Slower(Engine::GetInstance().render.get()->camera.x, (-player->position.getX() * scale) + WWidth / 2, 0.2f);
	}

	// L10 TODO 6: Implement a method that repositions the player in the map with a mouse click

	//Get mouse position and obtain the map coordinate
	Vector2D mousePos = Engine::GetInstance().input.get()->GetMousePosition();
	Vector2D mouseTile = Engine::GetInstance().map.get()->WorldToMap(mousePos.getX() - Engine::GetInstance().render.get()->camera.x / scale,
																     mousePos.getY() - Engine::GetInstance().render.get()->camera.y / scale);

	//Render a texture where the mouse is over to highlight the tile, use the texture 'mouseTileTex'
	Vector2D highlightTile = Engine::GetInstance().map.get()->MapToWorld(mouseTile.getX(),mouseTile.getY());
	SDL_Rect rect = { 0,0,32,32 };

	if (Engine::GetInstance().physics.get()->debug) 
	{
		Engine::GetInstance().render.get()->DrawTexture(mouseTileTex, highlightTile.getX(), highlightTile.getY(), &rect);
		//If mouse button is pressed modify enemy position
		if (Engine::GetInstance().input.get()->GetMouseButtonDown(SDL_BUTTON_RIGHT) == KEY_DOWN) {
			enemyList[0]->SetPosition(Vector2D(highlightTile.getX(), highlightTile.getY()));
			enemyList[0]->ResetPath();
		}
	}

	// saves the tile pos for debugging purposes
	if (mouseTile.getX() >= 0 && mouseTile.getY() >= 0 || once) {
		tilePosDebug = "[" + std::to_string((int)mouseTile.getX()) + "," + std::to_string((int)mouseTile.getY()) + "] ";
		once = true;
	}
	return true;
}

// Called each loop iteration
bool Scene::PostUpdate()
{
	bool ret = true;

	if(Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		LoadState();

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		SaveState();





	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
	{
		Engine::GetInstance().dialogueSystem.get()->SaveDialogueState();
	}

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
	{
		Engine::GetInstance().dialogueSystem.get()->LoadDialogueState();
	}

	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_8) == KEY_DOWN)
	{
		Engine::GetInstance().pauseMenu.get()->active = true;
		Engine::GetInstance().pauseMenu.get()->once = true;
	}

	return ret;
}

// Called before quitting
bool Scene::CleanUp()
{
	LOG("Freeing scene");
	return true;
}

// Return the player position
Vector2D Scene::GetPlayerPosition()
{
	return player->GetPosition();
}

// L15 TODO 1: Implement the Load function
void Scene::LoadState() {

	pugi::xml_document loadFile;
	pugi::xml_parse_result result = loadFile.load_file("config.xml");

	if(result == NULL)
	{
		LOG("Could not load file. Pugi error: %s", result.description());
		return;
	}

	pugi::xml_node sceneNode = loadFile.child("config").child("scene");

	//Read XML and restore information

	//Player position
	Vector2D playerPos = Vector2D(sceneNode.child("entities").child("player").attribute("x").as_int(),
								  sceneNode.child("entities").child("player").attribute("y").as_int());
	player->SetPosition(playerPos);

	//enemies
	// ...

}

// L15 TODO 2: Implement the Save function
void Scene::SaveState() {

	pugi::xml_document loadFile;
	pugi::xml_parse_result result = loadFile.load_file("config.xml");

	if (result == NULL)
	{
		LOG("Could not load file. Pugi error: %s", result.description());
		return;
	}

	pugi::xml_node sceneNode = loadFile.child("config").child("scene");

	//Save info to XML 

	//Player position
	sceneNode.child("entities").child("player").attribute("x").set_value(player->GetPosition().getX());
	sceneNode.child("entities").child("player").attribute("y").set_value(player->GetPosition().getY());

	//enemies
	// ...

	//Saves the modifications to the XML 
	loadFile.save_file("config.xml");
}

bool Scene::OnGuiMouseClickEvent(GuiControl* control)
{
	// L15: DONE 5: Implement the OnGuiMouseClickEvent method
	LOG("Press Gui Control: %d", control->id);

	return true;
}

float Scene::Slower(float ogPos, float goalPos, float time)
{
	float acceleration = goalPos - ogPos;
	float speed = ogPos + time * acceleration;
	return speed;
}

int Scene::RandomValue(int min, int max)
{
	return rand() % (max - min + 1) + min;
}

void Scene::StartDialogue(NPC &npc)
{
	if (Engine::GetInstance().input.get()->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && Engine::GetInstance().dialogueSystem->inDialog == false && npc.Interactable == true)
	{
		Engine::GetInstance().dialogueSystem.get()->LoadDialogue("dialogues.xml", npc);
		Engine::GetInstance().dialogueSystem.get()->inDialog = true;
	}
	Engine::GetInstance().render.get()->DrawText("Press Space to start dialogue", 10, 10, 50, { 255, 255, 255, 255 });
}