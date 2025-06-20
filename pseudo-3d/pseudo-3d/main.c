#define SDL_MAIN_USE_CALLBACKS 1

#include <stdio.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "../player/player.h"
#include "../player/raycast.h"
#include "../core/renderer.h"

Player* player = NULL;
Renderer* renderer = NULL;

Vector2D last_pixel_pos;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv)
{	
	player = initialize_player(2, 2, (Vector2D){4, 4}, 0);
	renderer = initialize_renderer(INIT_WIN_WIDTH, INIT_WIN_HEIGHT);
	
	last_pixel_pos.x = 0; last_pixel_pos.y = 0;

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
	SDL_Renderer* sdl_renderer = renderer->main_renderer;
	SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 255);
	SDL_RenderClear(sdl_renderer);
	SDL_ClearSurface(renderer->color_buffer, 0, 0, 0, 255);
	SDL_LockSurface(renderer->color_buffer);
	
	const float delta = get_delta_ticks();
	
	player->updatable_component->update(delta, player);
	player->collision_component->update(player);
	
	draw_3d(*renderer, *player);
	SDL_UnlockSurface(renderer->color_buffer);
	
	render_buffer(*renderer);
	
	SDL_RenderPresent(sdl_renderer);
	
	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
	SDL_DestroySurface(renderer->color_buffer);
	SDL_DestroySurface(renderer->textures_buffer[0]); //Need to remove it dynamicly
}
