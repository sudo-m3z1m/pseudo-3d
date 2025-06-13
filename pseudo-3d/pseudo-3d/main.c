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
	if (event->type != SDL_EVENT_KEY_DOWN)
		return SDL_APP_CONTINUE;
	
	SDL_Surface* color_buffer = renderer->color_buffer;
	SDL_LockSurface(color_buffer);
	
	uint8_t* pixels = (uint8_t*)color_buffer->pixels;
	int pixel_index = last_pixel_pos.y * color_buffer->pitch + last_pixel_pos.x;
	
	pixels[pixel_index] = 255;
	pixels[pixel_index + 1] = 255;
	pixels[pixel_index + 2] = 255;
	pixels[pixel_index + 3] = 255;
	
	SDL_UnlockSurface(color_buffer);
	last_pixel_pos.x++;
	if (last_pixel_pos.x > color_buffer->w)
	{
		last_pixel_pos.x = 0;
		last_pixel_pos.y++;
	}
	
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
	SDL_Renderer* sdl_renderer = renderer->main_renderer;
	SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 255);
	SDL_RenderClear(sdl_renderer);
	
	SDL_Texture* screen_texture = SDL_CreateTextureFromSurface(renderer->main_renderer, renderer->color_buffer);
	SDL_RenderTexture(renderer->main_renderer, screen_texture, NULL, NULL);
	
//	const float delta = get_delta_ticks();
//	
//	player->updatable_component->update(delta, player);
//	draw_3d(*renderer, *player);
	
	SDL_RenderPresent(sdl_renderer);
	
	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
	SDL_DestroySurface(renderer->color_buffer);
	SDL_DestroySurface(renderer->textures_buffer[0]); //Need to remove it directly
}
