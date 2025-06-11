#define RAYCASTS_COUNT 1024
#define RAYCASTS_SPREAD (PI / 3)

#include <stdio.h>

#include "../player/player.h"
#include "../player/raycast.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
Player* player = NULL;
Raycast** raycasts = NULL;

float global_rotation = 0;

float last_ticks = 0;

void draw_3d(int width, int height) //TODO: Place it into the Renderer
{
	float cast_width = (float)width / RAYCASTS_COUNT;
	float origin_y = (float)height / 2;
	
	for (size_t raycast_index = 0; raycast_index < RAYCASTS_COUNT; raycast_index++)
	{
		Color current_color;
		Raycast* current_raycast = raycasts[raycast_index];
		if(current_raycast->collided == false) continue;
		
		current_color = current_raycast->color;
		float x_coord = raycast_index * cast_width;
		float wall_height = (float)height / (current_raycast->length * cos(current_raycast->angle - global_rotation));
		
		SDL_SetRenderDrawColor(renderer, current_color.r, current_color.g, current_color.b, current_color.a);
		SDL_RenderLine(renderer, x_coord, origin_y - wall_height / 2, x_coord, origin_y + wall_height / 2);
	}
}

void get_raycasts_collisions(Player** player, int width, int height) //TODO: Place it to update with rotation updating
{
	char map[] = MAP;
	
	for (size_t raycast_index = 0; raycast_index < RAYCASTS_COUNT; raycast_index++)
	{
		const Vector2D origin = (*player)->position;
		Raycast* current_raycast = raycasts[raycast_index];
		
		Vector2D last_point = (Vector2D){origin.x, origin.y};
		apply_collision_pos(&last_point, &current_raycast, map);
	}
}

SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv)
{
	const int width = 640; // TODO: This all into the Renderer
	const int height = 640;
	
	SDL_CreateWindowAndRenderer("Test Window", width, height, SDL_WINDOW_RESIZABLE, &window, &renderer);
	initialize_player(&player, 1, (Vector2D){2.0f, 3.0f});
	initialize_raycasts(&raycasts, RAYCASTS_SPREAD, RAYCASTS_COUNT);
	
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
	
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
}
