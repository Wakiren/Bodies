#pragma once

#include "Module.h"
#include "Vector2D.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

class Render : public Module
{
public:

	Render();

	// Destructor
	virtual ~Render();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	void SetViewPort(const SDL_Rect& rect);
	void ResetViewPort();

	// Drawing
	bool DrawTexture(SDL_Texture* texture, int x, int y, const SDL_Rect* section = NULL, float speed = 1.0f, double angle = 0, int pivotX = INT_MAX, int pivotY = INT_MAX) const;
	bool DrawPlayer(SDL_Texture* texture, int x, int y, float scale = 1, const SDL_Rect* section = NULL, float speed = 1.0f, double angle = 0, int pivotX = INT_MAX, int pivotY = INT_MAX) const;
	bool DrawUIimage(SDL_Texture* texture, int x, int y, float scale = 1 ,double angle = 0 ,const SDL_Rect* section = NULL, float speed = 1.0f,  int pivotX = INT_MAX, int pivotY = INT_MAX) const;
	bool DrawRectangle(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool filled = true, bool useCamera = true) const;
	bool DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool useCamera = true) const;
	bool DrawCircle(int x1, int y1, int redius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool useCamera = true) const;
	bool DrawText(const char* text, int x, int y, int size, SDL_Color color = { 0, 0, 0 });

	// Set background color
	void SetBackgroundColor(SDL_Color color);

	//Load / Save methods
	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&);

public:

	SDL_Renderer* renderer;
	SDL_Rect camera;
	SDL_Rect viewport;
	SDL_Color background;
	TTF_Font* font;

	// Rendering text
	SDL_Surface* ttf_surface = nullptr;
	SDL_Texture* ttf_texture = nullptr;
	TTF_Font* ttf_font = nullptr;

	int scale;
};