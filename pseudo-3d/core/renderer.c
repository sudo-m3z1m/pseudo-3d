#include "renderer.h"

void draw_3d(Renderer renderer, Player player) //TODO: Place it into the Renderer
{
	float cast_width = (float)renderer.width / RAYS_COUNT;
	float horizon_pos_y = (float)renderer.height / 2;
	
	for (size_t raycast_index = 0; raycast_index < RAYS_COUNT; raycast_index++)
	{
		Color current_color;
		Raycast* current_raycast = player.raycasts[raycast_index];
		if(current_raycast->collided == false) continue;
		
		float x_coord = raycast_index * cast_width;
		float wall_height = (float)renderer.height / (current_raycast->length * cos(current_raycast->rotation));
		
		SDL_SetRenderDrawColor(renderer.main_renderer, current_color.r, current_color.g, current_color.b, current_color.a);
		SDL_RenderLine(renderer.main_renderer, x_coord, horizon_pos_y - wall_height / 2, x_coord, horizon_pos_y + wall_height / 2);
	}
}

Renderer* initialize_renderer(int width, int height)
{
	Renderer* renderer = malloc(sizeof(Renderer));
	renderer->width = width;
	renderer->height = height;
	SDL_CreateWindowAndRenderer(GAME_NAME, width, height, SDL_WINDOW_RESIZABLE, &renderer->main_window, &renderer->main_renderer);
	
	return renderer;
}
