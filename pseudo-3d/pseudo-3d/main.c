#define SDL_MAIN_USE_CALLBACKS 1

#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "../Player/player.h"
#include "../Core/raycast.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
Player* player = NULL;
Raycast* raycast = NULL;

float last_ticks = 0;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv)
{
	const int width = 640;
	const int height = 480;
	
	SDL_CreateWindowAndRenderer("Test Window", width, height, SDL_WINDOW_RESIZABLE, &window, &renderer);
	initialize_player(&player, 50);
	
	raycast = malloc(sizeof(Raycast));
	raycast->max_length = 60;
	raycast->angle = 0;
	
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
	Vector2D raycast_target_position;
	float speed = player->speed;
	
	float delta = (SDL_GetTicks() - last_ticks) * 0.001f;
	last_ticks = SDL_GetTicks();
	
	int width = 0, height = 0;
	int render_scale = 5;
	
	SDL_GetRenderOutputSize(renderer, &width, &height);
	player->position.x = SDL_clamp(player->position.x, 0, width / render_scale - 1);
	player->position.y = SDL_clamp(player->position.y, 0, height / render_scale - 1);
	
	raycast->angle += player->direction.x * delta * 10;
	
	dir.x *= speed * delta;
	dir.y *= speed * delta;
	
	player->position.x += dir.x;
	player->position.y += dir.y;
	
	raycast_target_position.x = player->position.x;
	raycast_target_position.y = player->position.y;
	apply_length_pos(60, &raycast_target_position, &raycast);
	
	SDL_SetRenderScale(renderer, render_scale, render_scale);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	
	SDL_FRect player_rect = (SDL_FRect){player->position.x, player->position.y, 6, 6};
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &player_rect);
	SDL_RenderLine(renderer, player->position.x + 3, player->position.y + 3, raycast_target_position.x, raycast_target_position.y);
	
	SDL_RenderPresent(renderer);
	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
}
