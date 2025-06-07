#define SDL_MAIN_USE_CALLBACKS 1

#include <iostream>
#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

float pos_x = 0, pos_y = 240;
float dir_x = 1;
float dir_y = 1;

void draw_text(const char* text)
{
	int w = 0, h = 0;
	float speed = 0.1;
	
	if (pos_x > 550 || pos_x < 0)
	{
		dir_x *= -1;
	}
	if (pos_y < 0 || pos_y > 480)
	{
		dir_y *= -1;
	}
	
	float velocity_x = dir_x * speed;
	float velocity_y = dir_y * speed;
	
	pos_x += velocity_x;
	pos_y += velocity_y;
	
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	
	SDL_GetRenderOutputSize(renderer, &w, &h);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDebugText(renderer, pos_x, pos_y, text);
	SDL_RenderPresent(renderer);
}

SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv)
{
	const int width = 640;
	const int height = 480;
	
	SDL_CreateWindowAndRenderer("Test Window", width, height, SDL_WINDOW_RESIZABLE, &window, &renderer);
	
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
	int type = event->type;
	if (type == SDL_EVENT_QUIT || type == SDL_EVENT_KEY_DOWN)
	{
		return SDL_APP_SUCCESS;
	}
	
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
	const char* text = "Cool text!";
	draw_text(text);
	
	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
}
