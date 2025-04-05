#ifndef __INTROSCENE__
#define __INTROSCENE_H__

#include "Module.h"
#include "SDL2/SDL.h"

class IntroScene : public Module
{
public:

	IntroScene();

	// Destructor
	virtual ~IntroScene();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	bool CleanUp();

private:

	int cooldownCounter = 0;
	int cooldown = 600;

	bool once = true;

	bool isIntro = true;
	bool isTeam = false;
	bool isTitle = true;

	bool haveToFade = false;

	bool debugMode = false;

	SDL_Rect backgroundRect = { 0, 0, 1920, 1080 };

	SDL_Texture* title;
	SDL_Texture* teamMembers;
};

#endif // __INTROSCENE__

