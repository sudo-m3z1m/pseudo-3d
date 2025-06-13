#include "renderer.h"

void draw_3d(Renderer renderer, Player player) //TODO: Place it into the Renderer
{
	SDL_Renderer* sdl_renderer = renderer.main_renderer;
	
	float cast_width = (float)renderer.width / RAYS_COUNT;
	float horizon_pos_y = (float)renderer.height / 2;
	
	for (size_t raycast_index = 0; raycast_index < RAYS_COUNT; raycast_index++)
	{
		Raycast* current_raycast = player.raycasts[raycast_index];
		if(current_raycast->collided == false) continue;
		
		float x_coord = raycast_index * cast_width;
		float wall_height = (float)renderer.height / (current_raycast->length * cos(current_raycast->rotation));
		
		SDL_SetRenderDrawColor(sdl_renderer, 255, 255, 255, 255);
		SDL_RenderLine(sdl_renderer, x_coord, horizon_pos_y - wall_height / 2, x_coord, horizon_pos_y + wall_height / 2);
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

void draw_texture_column(Renderer renderer, Vector2D draw_position, char* texture_name)
{
	int texture_x = draw_position.x;
	char path[512];
	get_resources_path(path, texture_name);
	
	SDL_Surface* surface = SDL_LoadBMP(path);
	Color pixel_color;
	
	for (size_t texture_y = 0; texture_y < TEXTURE_SIZE; texture_y++)
	{
		SDL_ReadSurfacePixel(surface, texture_x, (int)texture_y, &pixel_color.r, &pixel_color.g, &pixel_color.b, &pixel_color.a);
		SDL_SetRenderDrawColor(renderer.main_renderer, pixel_color.r, pixel_color.g, pixel_color.b, pixel_color.a);
		SDL_RenderPoint(renderer.main_renderer, INIT_WIN_WIDTH / 2 + texture_x, INIT_WIN_HEIGHT / 2 + texture_y);
	}
	
	SDL_DestroySurface(surface);
}
