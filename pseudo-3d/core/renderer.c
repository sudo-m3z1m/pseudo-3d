#include "renderer.h"

void draw_3d(Renderer renderer, Player player) //TODO: Place it into the Renderer
{
//	float cast_width = (float)width / RAYCASTS_COUNT;
//	float origin_y = (float)height / 2;
//	
//	for (size_t raycast_index = 0; raycast_index < RAYCASTS_COUNT; raycast_index++)
//	{
//		Color current_color;
//		Raycast* current_raycast = raycasts[raycast_index];
//		if(current_raycast->collided == false) continue;
//		
//		current_color = current_raycast->color;
//		float x_coord = raycast_index * cast_width;
//		float wall_height = (float)height / (current_raycast->length * cos(current_raycast->angle - global_rotation));
//		
//		SDL_SetRenderDrawColor(renderer, current_color.r, current_color.g, current_color.b, current_color.a);
//		SDL_RenderLine(renderer, x_coord, origin_y - wall_height / 2, x_coord, origin_y + wall_height / 2);
//	}
}
