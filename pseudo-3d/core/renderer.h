#ifndef RENDERER_H
#define RENDERER_H

#define GAME_NAME "PseudoName"

#define INIT_WIN_WIDTH 640
#define INIT_WIN_HEIGHT 640

#include <stdio.h>
#include <SDL3/SDL.h>

#include "../player/player.h"
#include "../player/raycast.h"

typedef struct Renderer
{
	SDL_Renderer* main_renderer;
	SDL_Window* main_window;
	int width, height;
} Renderer;

Renderer* initialize_renderer(int width, int height);
void draw_3d(Renderer renderer, Player player);

#endif
