#include "renderer.h"

void draw_pixel_in_buffer(Renderer renderer, int x, int y, Color color)
{
	SDL_Surface* color_buffer = renderer.color_buffer;
	uint8_t* pixels = (uint8_t*)color_buffer->pixels;
	int buffer_pitch = color_buffer->pitch;
	
	int pixel_size = buffer_pitch / renderer.width;
	int pixel_index = y * buffer_pitch + x * pixel_size;
	
	pixels[pixel_index] = color.r;
	pixels[pixel_index + 1] = color.g;
	pixels[pixel_index + 2] = color.b;
	pixels[pixel_index + 3] = color.a;
}

void draw_3d(Renderer renderer, Player player)
{
	SDL_Renderer* sdl_renderer = renderer.main_renderer;
	
	float cast_width = (float)renderer.width / RAYS_COUNT;
	float horizon_pos_y = (float)renderer.height / 2;
	
	float texture_delta_x, texture_delta_y, texture_delta;
	
	Vector2D target_pos;
	
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
		draw_texture_column(renderer, (Vector2D){x_coord, horizon_pos_y - wall_height / 2}, wall_height, texture_delta);
	}
}

void draw_texture_column(Renderer renderer,
						 Vector2D draw_position,
						 float wall_height,
						 float texture_delta)
{
	SDL_Surface* color_buffer = renderer.color_buffer;
	
	Color pixel_color;
	SDL_Surface* texture_surface = renderer.textures_buffer[0];
	
	const int texture_pos_x = (int)(texture_delta * TEXTURE_SIZE);
	
	for (size_t draw_y = draw_position.y; draw_y < draw_position.y + wall_height; draw_y++)
	{
		float height_ratio = (draw_y - draw_position.y) / wall_height;
		int pixel_y = (int)(height_ratio * TEXTURE_SIZE);
		
		SDL_ReadSurfacePixel(texture_surface, texture_pos_x, pixel_y, &pixel_color.r, &pixel_color.g, &pixel_color.b, &pixel_color.a);
		draw_pixel_in_buffer(renderer, draw_position.x, (int)draw_y, pixel_color);
	}
}

void draw_floor_3d(Renderer renderer, Player player)
{
	SDL_Surface* floor_texture = renderer.textures_buffer[1];
	
	int texture_width = floor_texture->w;
	int texture_height = floor_texture->h;
	
	Vector2D player_dir = player.direction;
	if (player_dir.x == 0 && player_dir.y == 0) player_dir.x = 1.0f;
	player_dir = rotate_vector(player_dir, player.rotation);
	
	Vector2D plane = (Vector2D){0, tanf(FOV / 2)};
	plane = rotate_vector(plane, player.rotation);
	
	Vector2D ray_dir_left = (Vector2D){player_dir.x - plane.x, player_dir.y - plane.y};
	Vector2D ray_dir_right = (Vector2D){player_dir.x + plane.x, player_dir.y + plane.y};
	
	float camera_height = (float)renderer.height / 2;
	float distance;
	
	Color pixel_color;
	
	printf("player dir: (%f, %f), plane: (%f, %f),\n left: (%f, %f), right: (%f, %f)\n", player_dir.x, player_dir.y, plane.x, plane.y, ray_dir_left.x, ray_dir_left.y, ray_dir_right.x, ray_dir_right.y);
	
	for (size_t draw_y = renderer.height / 2 + 1; draw_y < renderer.height; draw_y++)
	{
		distance = camera_height / (draw_y - renderer.height / 2);
		Vector2D floor_pos;
		floor_pos.x = distance * ray_dir_left.x + player.position.x;
		floor_pos.y = distance * ray_dir_left.y + player.position.y;

		int floor_cell_x;
		int floor_cell_y;
		
		Vector2D floor_step;
		floor_step.x = distance * (ray_dir_right.x - ray_dir_left.x) / renderer.width;
		floor_step.y = distance * (ray_dir_right.y - ray_dir_left.y) / renderer.width;
		
		for (size_t draw_x = 0; draw_x < renderer.width; draw_x++)
		{
			floor_cell_x = (int)floor_pos.x;
			floor_cell_y = (int)floor_pos.y;
			
			int pixel_x = (int)(texture_width * (floor_pos.x - floor_cell_x)) & (texture_width - 1);
			int pixel_y = (int)(texture_height * (floor_pos.y - floor_cell_y)) & (texture_height - 1);
			
			floor_pos.x += floor_step.x;
			floor_pos.y += floor_step.y;
			
			SDL_ReadSurfacePixel(floor_texture, pixel_x, pixel_y, &pixel_color.r, &pixel_color.g, &pixel_color.b, &pixel_color.a);
			draw_pixel_in_buffer(renderer, (int)draw_x, (int)draw_y, pixel_color);
		}
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
	
	get_resources_path(path, "bricks.bmp");			//TODO: Need to simplify this.
	renderer->textures_buffer[0] = SDL_LoadBMP(path);
	get_resources_path(path, "floor-bricks.bmp");
	renderer->textures_buffer[1] = SDL_LoadBMP(path);
	
	return renderer;
}

void render_buffer(Renderer renderer)
{
	SDL_Texture* screen_texture = SDL_CreateTextureFromSurface(renderer.main_renderer, renderer.color_buffer);
	SDL_RenderTexture(renderer.main_renderer, screen_texture, NULL, NULL);
	SDL_DestroyTexture(screen_texture);
}
