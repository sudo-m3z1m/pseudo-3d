#include "renderer.h"

void draw_3d(Renderer renderer, Player player) //TODO: Place it into the Renderer
{
	SDL_Renderer* sdl_renderer = renderer.main_renderer;
	
	float cast_width = (float)renderer.width / RAYS_COUNT;
	float horizon_pos_y = (float)renderer.height / 2;
	
	float texture_delta_x, texture_delta_y, texture_delta;
	
	Vector2D target_pos;
	
	SDL_FRect rect = (SDL_FRect){0, 0, renderer.width, horizon_pos_y};
	SDL_SetRenderDrawColor(sdl_renderer, 78, 139, 243, 255);
	SDL_RenderFillRect(renderer.main_renderer, &rect);
	
	for (size_t raycast_index = 0; raycast_index < RAYS_COUNT; raycast_index++)
	{
		Raycast* current_raycast = player.raycasts[raycast_index];
		if(!current_raycast->collided) continue;
		
		target_pos = current_raycast->target_pos;
		
		texture_delta_y = target_pos.y - floorf(target_pos.y);
		texture_delta_x = target_pos.x - floorf(target_pos.x);
		texture_delta = texture_delta_x;
		
		if (texture_delta_x < 0.01 || texture_delta_x > 0.99)
			texture_delta = texture_delta_y;
			
		float x_coord = raycast_index * cast_width;
		float wall_height = (float)renderer.height / (current_raycast->length * cos(current_raycast->rotation));
		
		SDL_SetRenderDrawColor(sdl_renderer, 255, 255, 255, 255);
//		SDL_RenderLine(sdl_renderer, x_coord, horizon_pos_y - wall_height / 2, x_coord, horizon_pos_y + wall_height / 2);
		draw_texture_column(renderer, (Vector2D){x_coord, horizon_pos_y - wall_height / 2}, wall_height, texture_delta);
	}
}

void draw_texture_column(Renderer renderer,
						 Vector2D draw_position,
						 float wall_height,
						 float texture_delta)
{
	Color pixel_color;
	SDL_Surface* texture_surface = renderer.textures_buffer[0];
	
	const int texture_pos_x = (int)(texture_delta * TEXTURE_SIZE);
	
	for (size_t draw_y = draw_position.y; draw_y < draw_position.y + wall_height; draw_y++)
	{
		float height_ratio = (draw_y - draw_position.y) / wall_height;
		int pixel_y = (int)(height_ratio * TEXTURE_SIZE);
		
		SDL_ReadSurfacePixel(texture_surface, texture_pos_x, pixel_y, &pixel_color.r, &pixel_color.g, &pixel_color.b, &pixel_color.a);
	}
}

Renderer* initialize_renderer(int width, int height)
{
	Renderer* renderer = malloc(sizeof(Renderer));
	char path[512];
	
	renderer->width = width;
	renderer->height = height;
	renderer->color_buffer = SDL_CreateSurface(width, height, SDL_PIXELFORMAT_RGBA32);
	SDL_CreateWindowAndRenderer(GAME_NAME, width, height, SDL_WINDOW_RESIZABLE, &renderer->main_window, &renderer->main_renderer);
	
	get_resources_path(path, "bricks.bmp");
//	renderer->textures_buffer[0] = SDL_LoadBMP(path);
	
	return renderer;
}
