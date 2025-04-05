#include "IntroScene.h"
#include "Log.h"
#include "Engine.h"
#include "Textures.h"
#include "Render.h"
#include "FadeManager.h"
#include "MainMenu.h"

IntroScene::IntroScene()
{
	name = "introscene";
}

IntroScene::~IntroScene()
{
}

bool IntroScene::Awake()
{
	LOG("Loading intro scene");
	bool ret = true;

	return ret;
}

bool IntroScene::Start()
{
	title = Engine::GetInstance().textures.get()->Load("Assets/Textures/IntroTitle.png");

	teamMembers = Engine::GetInstance().textures.get()->Load("Assets/Textures/TeamMembers.png");

	return true;
}

bool IntroScene::PreUpdate()
{
	return true;
}

bool IntroScene::Update(float dt)
{
	bool isFading = Engine::GetInstance().fadeManager.get()->IsFading();
	FadeType type = Engine::GetInstance().fadeManager.get()->GetCurrentFadeType();

	if (isTitle) {
		
		Engine::GetInstance().render.get()->DrawUIimage(title, 0, 0, 1, &backgroundRect);
		cooldownCounter++;
		if (cooldownCounter >= cooldown)
		{
			if (once)
			{
				Engine::GetInstance().fadeManager.get()->Fade(3.0f, 300);
				once = false;
			}

			if (isFading && type == FadeType::FADE_IN)
			{
				isTitle = false;
				isTeam = true;
				cooldownCounter = 0;
			}
		}
	}

	else if (isTeam) {
		
		Engine::GetInstance().render.get()->DrawUIimage(teamMembers, 0, 0, 1, &backgroundRect);
		cooldownCounter++;
		if (cooldownCounter >= cooldown)
		{
			if (!once)
			{
				Engine::GetInstance().fadeManager.get()->Fade(3.0f, 300);
				once = true;
			}

			if (isFading && type == FadeType::FADE_IN) {

				isTeam = false;
				isIntro = false;
				cooldownCounter = 0;
			}
		}

	}

	if (!isIntro)
	{
		Engine::GetInstance().introScene.get()->active = false;
		Engine::GetInstance().mainMenu.get()->active = true;
	}

	bool ret = true;
	return ret;
}

bool IntroScene::PostUpdate()
{
	return true;
}

bool IntroScene::CleanUp()
{
	LOG("Freeing intro scene");
	return true;
}
