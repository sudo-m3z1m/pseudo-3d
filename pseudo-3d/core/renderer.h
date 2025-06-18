#ifndef RENDERER_H
#define RENDERER_H

#define GAME_NAME "PseudoName"

#define INIT_WIN_WIDTH 640
#define INIT_WIN_HEIGHT 640
#define TEXTURE_SIZE 64

#include <stdio.h>
#include <SDL3/SDL.h>

#include "../player/player.h"
#include "../player/raycast.h"

typedef struct Renderer
{
	SDL_Renderer* main_renderer;
	SDL_Surface* textures_buffer[32];
	SDL_Window* main_window;
	int width, height;
	
	SDL_Surface* color_buffer;
} Renderer;

Renderer* initialize_renderer(int width, int height);

void render_buffer(Renderer renderer);
void draw_pixel_in_buffer(Renderer renderer, int x, int y, Color color);
void draw_3d(Renderer renderer, Player player);
void draw_horizontal_surfaces_3d(Renderer renderer, Player player);

void draw_texture_column(Renderer renderer,
						 Vector2D draw_position,
						 float wall_height,
						 float texture_delta,
						 int texture_index);

static void draw_texture_row(Renderer renderer,
							 Player player,
							 SDL_Surface* texture,
							 float distance,
							 int screen_delta);

#endif
