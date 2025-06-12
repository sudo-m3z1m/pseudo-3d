#include <stdio.h>

#include "../player/player.h"
#include "../player/raycast.h"

//SDL_Window* window = NULL;
//SDL_Renderer* renderer = NULL;
Player* player = NULL;

SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv)
{
//	const int width = 640; // TODO: This all into the Renderer
//	const int height = 640;
//	
//	SDL_CreateWindowAndRenderer("Test Window", width, height, SDL_WINDOW_RESIZABLE, &window, &renderer);
//	initialize_player(&player, 1, (Vector2D){2.0f, 3.0f});
//	initialize_raycasts(&raycasts, RAYCASTS_SPREAD, RAYCASTS_COUNT);
	
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
