#include "Engine.h"
#include "Window.h"
#include "Render.h"
#include "Log.h"
#include "tracy/Tracy.hpp"
#include "SDL2/SDL.h"

#define VSYNC true

Render::Render() : Module()
{
	name = "render";
	background.r = 0;
	background.g = 0;
	background.b = 0;
	background.a = 0;
}

// Destructor
Render::~Render()
{}

// Called before render is available
bool Render::Awake()
{
	LOG("Create SDL rendering context");
	bool ret = true;

	Uint32 flags = SDL_RENDERER_ACCELERATED;

	//L05 TODO 5 - Load the configuration of the Render module
	if (configParameters.child("vsync").attribute("value").as_bool() == true) {
		flags |= SDL_RENDERER_PRESENTVSYNC;
		LOG("Using vsync");
	}
	
	scale = Engine::GetInstance().window.get()->GetScale();

	SDL_Window* window = Engine::GetInstance().window.get()->window;
	renderer = SDL_CreateRenderer(window, -1, flags);
	SDL_RenderSetLogicalSize(renderer, Engine::GetInstance().window.get()->width, Engine::GetInstance().window.get()->height);

	if(renderer == NULL)
	{
		LOG("Could not create the renderer! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		camera.w = Engine::GetInstance().window.get()->width ;
		camera.h = Engine::GetInstance().window.get()->height ;
		camera.x = 0;
		camera.y = 0;
	}

	//initialise the SDL_ttf library
	TTF_Init();

	return ret;
}

// Called before the first frame
bool Render::Start()
{
	LOG("render start");
	SDL_RenderGetViewport(renderer, &viewport);
	return true;
}

// Called each loop iteration
bool Render::PreUpdate()
{
	ZoneScoped;
	// Code you want to profile
	SDL_RenderClear(renderer);
	return true;
}

bool Render::Update(float dt)
{
	return true;
}

bool Render::PostUpdate()
{
	ZoneScoped;
	// Code you want to profile

	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.g, background.a);
	//Draw the Trees
	SDL_RenderPresent(renderer);
	return true;
}

// Called before quitting
bool Render::CleanUp()
{
	LOG("Destroying SDL render");
	SDL_DestroyRenderer(renderer);
	return true;
}

void Render::SetBackgroundColor(SDL_Color color)
{
	background = color;
}

bool Render::LoadState(pugi::xml_node& data)
{
	camera.x = data.child("camera").attribute("x").as_int();
	camera.y = data.child("camera").attribute("y").as_int();
	return true;
}

bool Render::SaveState(pugi::xml_node& data)
{
	pugi::xml_node cam = data.append_child("camera");
	cam.append_attribute("x") = camera.x;
	cam.append_attribute("y") = camera.y;
	return true;
}

void Render::SetViewPort(const SDL_Rect& rect)
{
	SDL_RenderSetViewport(renderer, &rect);
}

void Render::ResetViewPort()
{
	SDL_RenderSetViewport(renderer, &viewport);
}

// Blit to screen
bool Render::DrawTexture(SDL_Texture* texture, int x, int y, const SDL_Rect* section, float speed, double angle, int pivotX, int pivotY) const
{
	bool ret = true;

	SDL_Rect rect;
	rect.x = (int)(camera.x * speed) + x * scale;
	rect.y = (int)(camera.y * speed) + y * scale;

	if(section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	rect.w *= scale;
	rect.h *= scale;

	SDL_Point* p = NULL;
	SDL_Point pivot;

	if(pivotX != INT_MAX && pivotY != INT_MAX)
	{
		pivot.x = pivotX;
		pivot.y = pivotY;
		p = &pivot;
	}

	if(SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, SDL_FLIP_NONE) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawPlayer(SDL_Texture* texture, int x, int y, float scale_, const SDL_Rect* section, float speed, double angle, int pivotX, int pivotY) const
{
	bool ret = true;

	SDL_Rect rect;
	rect.x = (int)(camera.x * speed) + x * scale;
	rect.y = (int)(camera.y * speed) + y * scale;

	if (section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	rect.w *= scale_;
	rect.h *= scale_;

	SDL_Point* p = NULL;
	SDL_Point pivot;

	if (pivotX != INT_MAX && pivotY != INT_MAX)
	{
		pivot.x = pivotX;
		pivot.y = pivotY;
		p = &pivot;
	}

	if (SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, SDL_FLIP_NONE) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawUIimage(SDL_Texture* texture, int x, int y, float scale_, double angle, const SDL_Rect* section, float speed, int pivotX, int pivotY) const
{
	bool ret = true;

	SDL_Rect rect;

	rect.x = ((int)(camera.x * speed) + x - camera.x);
	rect.y = ((int)(camera.y * speed) + y - camera.y);


	if (section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	if (scale != 1)
	{
		rect.w *= scale_;
		rect.h *= scale_;
	}

	SDL_Point* p = NULL;
	SDL_Point pivot;

	if (pivotX != INT_MAX && pivotY != INT_MAX)
	{
		pivot.x = pivotX;
		pivot.y = pivotY;
		p = &pivot;
	}

	if (SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, SDL_FLIP_NONE) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawRectangle(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool filled, bool use_camera) const
{
	bool ret = true;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);
	if(use_camera)
	{
		rec.x = (int)(camera.x + rect.x * scale);
		rec.y = (int)(camera.y + rect.y * scale);
		rec.w *= scale;
		rec.h *= scale;
	}

	int result = (filled) ? SDL_RenderFillRect(renderer, &rec) : SDL_RenderDrawRect(renderer, &rec);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;

	if(use_camera)
		result = SDL_RenderDrawLine(renderer, camera.x + x1 * scale, camera.y + y1 * scale, camera.x + x2 * scale, camera.y + y2 * scale);
	else
		result = SDL_RenderDrawLine(renderer, x1 * scale, y1 * scale, x2 * scale, y2 * scale);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;
	SDL_Point points[360];

	float factor = (float)M_PI / 180.0f;

	for(int i = 0; i < 360; ++i)
	{
		points[i].x = (int)(x * scale + camera.x) + (int)(radius * cos(i * factor));
		points[i].y = (int)(y * scale + camera.y) + (int)(radius * sin(i * factor));
	}

	result = SDL_RenderDrawPoints(renderer, points, 360);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawText(const char* text, int x, int y, int size, SDL_Color color)
{
	bool ret = true;
	//load a font into memory
	ttf_font = TTF_OpenFont("Assets/Fonts/Remnant-Regular.ttf", size);

	if (!ttf_font)
	{
		LOG("Cannot open font. TTF_OpenFont error: %s", TTF_GetError());
		return false;
	}

	SDL_Color ttf_color;
	ttf_color.r = color.r;
	ttf_color.g = color.g;
	ttf_color.b = color.b;
	ttf_color.a = color.a;

	SDL_Rect ttf_rect;
	ttf_surface = TTF_RenderText_Solid(ttf_font, text, ttf_color);
	ttf_texture = SDL_CreateTextureFromSurface(renderer, ttf_surface);

	if (ttf_surface == nullptr)
	{
		//LOG("Cannot open font. SDL_Surface* error: %s", SDL_GetError());
		ret = false;
	}
	else
	{
		ttf_rect.x = x ;
		ttf_rect.y = y;
		ttf_rect.w = ttf_surface->w ;
		ttf_rect.h = ttf_surface->h ;

		if (SDL_RenderCopy(renderer, ttf_texture, NULL, &ttf_rect) != 0)
		{
			LOG("Cannot render text to screen. SDL_RenderCopy error: %s", SDL_GetError());
			ret = false;
		}

		SDL_DestroyTexture(ttf_texture);
		SDL_FreeSurface(ttf_surface);
		ttf_texture = nullptr;
		TTF_CloseFont(ttf_font);
	}

	return ret;
}
