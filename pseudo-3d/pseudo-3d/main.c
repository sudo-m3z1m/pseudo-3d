#define SDL_MAIN_USE_CALLBACKS 1
#define RAYCASTS_COUNT 30
#define RAYCASTS_SPREAD (PI / 2)

#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "../Player/player.h"
#include "../Core/raycast.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
Player* player = NULL;
Raycast** raycasts = NULL;

float last_ticks = 0;

void draw_raycasts(Player** player)
{
	for (size_t raycast_index = 0; raycast_index < RAYCASTS_COUNT; raycast_index++)
	{
		const Vector2D origin = (*player)->position;
		Raycast* current_raycast = raycasts[raycast_index];
		
		Vector2D last_point = (Vector2D){origin.x, origin.y};
		apply_length_pos(current_raycast->max_length, &last_point, &current_raycast);
		
		SDL_RenderLine(renderer, origin.x, origin.y, last_point.x, last_point.y);
	}
}

SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv)
{
	const int width = 640;
	const int height = 480;
	
	SDL_CreateWindowAndRenderer("Test Window", width, height, SDL_WINDOW_RESIZABLE, &window, &renderer);
	initialize_player(&player, 10);
	initialize_raycasts(&raycasts, RAYCASTS_SPREAD, RAYCASTS_COUNT);
	
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
	SDL_KeyboardEvent event_key = event->key;
	
	player->direction = (Vector2D){0.0f, 0.0f};
	if (event->type == SDL_EVENT_KEY_DOWN)
	{
		switch (event_key.key) {
			case SDLK_RIGHT:
				player->direction.x += 1;
				break;
			case SDLK_LEFT:
				player->direction.x -= 1;
				break;
			case SDLK_UP:
				player->direction.y -= 1;
				break;
			case SDLK_DOWN:
				player->direction.y += 1;
				break;
				
			default:
				break;
		}
	}
	
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
	Vector2D dir = player->direction;
	float speed = player->speed;
	
	float delta = (SDL_GetTicks() - last_ticks) * 0.001f;
	last_ticks = SDL_GetTicks();
	
	int width = 0, height = 0;
	int render_scale = 1;
	
	SDL_GetRenderOutputSize(renderer, &width, &height);

//	player->position.x = SDL_clamp(player->position.x, 0, width / render_scale - 1);
//	player->position.y = SDL_clamp(player->position.y, 0, height / render_scale - 1);
	
	player->position.x = width / render_scale / 2;
	player->position.y = height / render_scale / 2;
	
	update_raycasts(&raycasts, dir.x * delta * speed, RAYCASTS_COUNT);
	
//	dir.x *= speed * delta;
//	dir.y *= speed * delta;
	
//	player->position.x += dir.x;
//	player->position.y += dir.y;
	
	SDL_SetRenderScale(renderer, render_scale, render_scale);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	
	SDL_FRect player_rect = (SDL_FRect){player->position.x - 3, player->position.y - 3, 6, 6};
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &player_rect);
	draw_raycasts(&player);
	
	SDL_RenderPresent(renderer);
	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
}
