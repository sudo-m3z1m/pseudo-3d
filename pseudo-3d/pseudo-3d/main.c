#define SDL_MAIN_USE_CALLBACKS 1

#include <stdio.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "../player/player.h"
#include "../player/raycast.h"
#include "../core/renderer.h"

Player* player = NULL;
Renderer* renderer = NULL;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv)
{	
	player = initialize_player(2, 2, (Vector2D){4, 4}, 0);
	renderer = initialize_renderer(INIT_WIN_WIDTH, INIT_WIN_HEIGHT);

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
	
	for (size_t texture_x = 0; texture_x < TEXTURE_SIZE; texture_x++)
	{
		draw_texture_column(*renderer, (Vector2D){texture_x, 0}, "bricks.bmp");
	}
	
//	const float delta = get_delta_ticks();
	
//	player->updatable_component->update(delta, player);
//	draw_3d(*renderer, *player);
	
	SDL_RenderPresent(sdl_renderer);
	
	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
	
}
