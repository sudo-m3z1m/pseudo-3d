#include "renderer.hpp"

Renderer::Renderer()
{
	screen_width = DEFAULT_SCREEN_WIDTH;
	screen_height = DEFAULT_SCREEN_HEIGHT;
	
	current_camera = nullptr;
	level_server = nullptr;
	SDL_CreateWindowAndRenderer(WINDOW_NAME, screen_width, screen_height, SDL_WINDOW_RESIZABLE, &application_window, &application_renderer);
	
	color_buffer = SDL_CreateSurface(screen_width, screen_height, SDL_PIXELFORMAT_RGBA32);
	screen_width_buffer = std::vector<bool>(screen_width, false);
	textures_buffer = std::vector<SDL_Surface*>();
}

Renderer::Renderer(Camera* camera, LevelServer* level_server, int width, int height)
{	
	screen_width = width;
	screen_height = height;
	
	current_camera = camera;
	this->level_server = level_server;
	SDL_CreateWindowAndRenderer(WINDOW_NAME, screen_width, screen_height, SDL_WINDOW_RESIZABLE, &application_window, &application_renderer);
	color_buffer = SDL_CreateSurface(screen_width, screen_height, SDL_PIXELFORMAT_RGBA32);
	
	screen_width_buffer = std::vector<bool>(width, false);
	textures_buffer = std::vector<SDL_Surface*>(); //TODO: loading level textures(from level server probably)
}

Renderer::~Renderer()
{
	delete current_camera;
	delete level_server;
	delete color_buffer;
}

float Renderer::get_delta_ticks()
{
	float new_ticks = SDL_GetTicks();
	float new_delta = (new_ticks - prev_ticks) * 0.001f;
	prev_ticks = SDL_GetTicks();
	
	delta = new_delta;
	
	return new_delta;
}

bool Renderer::is_screen_space_free(int x_point)
{
	x_point = SDL_min(screen_width, SDL_max(0, x_point));
	bool is_point_taken = screen_width_buffer[x_point];
	
	if (is_point_taken) return false;
	
	screen_width_buffer[x_point] = true;
	return true;
}

int Renderer::get_point_on_camera_projection(Vector2D<float> point)
{
	Vector2D<float> vector_to_point = point - current_camera->position;
	float relative_vector_rot = vector_to_point.get_vector_rotation() - current_camera->rotation;
	
	const float h_perspective_k = (screen_width / 2) / (tan(current_camera->field_of_view / 2));
	int x_projection = screen_width / 2 - (h_perspective_k * tan(relative_vector_rot));
	x_projection = SDL_min(screen_width, SDL_max(0, x_projection));
	
	return x_projection;
}

int Renderer::get_wall_height(Vector2D<float> point)
{
	Vector2D<float> vector_to_point = point - current_camera->position;
//	const float relative_vector_rot = vector_to_point.get_vector_rotation() - current_camera->rotation;
	const float vector_length = vector_to_point * Vector2D<float>(cos(current_camera->rotation), sin(current_camera->rotation));
	
	int height = (4 / vector_length) * (screen_height / 2) / (tan(current_camera->field_of_view / 2)); //FIXME: static height
	height = SDL_min(screen_height, SDL_max(0, height));
	
	return height;
}

void Renderer::clear_screen_width_buffer()
{
	for(size_t buffer_index = 0; buffer_index < screen_width_buffer.size(); buffer_index++)
	{
		screen_width_buffer[buffer_index] = false;
	}
}

void Renderer::render()
{
	SDL_SetRenderScale(application_renderer, 4, 4);
	SDL_SetRenderDrawColor(application_renderer, 0, 0, 0, 255);
	clear_screen_width_buffer();
	SDL_RenderClear(application_renderer);
	SDL_ClearSurface(color_buffer, 0, 0, 0, 255);
	
	SDL_LockSurface(color_buffer);
	
	render_node(level_server->bsp_tree);
	
	SDL_UnlockSurface(color_buffer);
	
	render_buffer();
	SDL_RenderPresent(application_renderer);
}

void Renderer::render_node(BSPNode* node)
{
	if (node->shape)
	{
		render_bsp_shape(node);
		return;
	}
	
	Line node_separate_line = node->separate_line;
	const float position_side = is_point_on_line(current_camera->position, node_separate_line);
	if (position_side <= 0)
	{
		render_node(node->front);
		render_node(node->back);
		return;
	}
	render_node(node->back);
	render_node(node->front);
	return;
}

void Renderer::render_bsp_shape(BSPNode* node)
{
	BSPShape* shape = node->shape;
	std::vector<Wall> shape_walls = shape->walls;
	std::vector<Vector2D<float>> shape_points = shape->points;
	
	bool is_shape_in_frustrum = current_camera->is_shape_in_frustrum(shape_points);
	if(!is_shape_in_frustrum) return;
	
	for(Wall current_wall : shape->walls)
	{
		std::vector<Vector2D<float>> wall_points = current_wall.get_wall_points(shape_points);
		Vector2D<float> to_wall_vector = wall_points[0] - current_camera->position;
		
		if(((to_wall_vector * current_wall.normal) >= 0)) continue;
		
		wall_points = current_camera->clip_wall_by_frustrum(wall_points);
		if(wall_points.size() != 2)
		{
			continue;
		}
		render_wall(wall_points, current_wall.color);
	}
}

void Renderer::render_wall(std::vector<Vector2D<float>> wall_points, Color color)
{
	int f_edge_pos_x = get_point_on_camera_projection(wall_points[0]);
	int s_edge_pos_x = get_point_on_camera_projection(wall_points[1]);
	
	int f_edge_height = get_wall_height(wall_points[0]);
	int s_edge_height = get_wall_height(wall_points[1]);
	
	if(f_edge_pos_x > s_edge_pos_x)
	{
		std::swap(f_edge_pos_x, s_edge_pos_x);
		std::swap(f_edge_height, s_edge_height);
	}
	
	for(int wall_x = f_edge_pos_x; wall_x < s_edge_pos_x; wall_x++)
	{
		if(!is_screen_space_free(wall_x)) continue;
		float height_k = float(wall_x - f_edge_pos_x) / float(s_edge_pos_x - f_edge_pos_x);
		int wall_height = f_edge_height + height_k * (s_edge_height - f_edge_height);
		render_column(wall_x, wall_height, color); //FIXME: Coloring is stupid shit. Broken architecture
	}
}

void Renderer::render_horizontal() 
{
	
}

void Renderer::render_column(int pos_x, int height, Color color) //TODO: It doesn't had height. It had top and bottom
{
	for(int cur_height = 0; cur_height < height / 2; cur_height++)
	{
		draw_pixel_in_buffer(Vector2D<int>(pos_x, (screen_height / 2) + cur_height), color);
		draw_pixel_in_buffer(Vector2D<int>(pos_x, (screen_height / 2) - cur_height), color);
	}
}

void Renderer::draw_pixel_in_buffer(Vector2D<int> draw_pos, Color color)
{
	const int buffer_pitch = color_buffer->pitch;
	const int pixel_size = buffer_pitch / screen_width;
	const int pixel_index = draw_pos.y * buffer_pitch + draw_pos.x * pixel_size;
	uint8_t* buffer_pixels = (uint8_t*)color_buffer->pixels;
	
	buffer_pixels[pixel_index] = color.r;
	buffer_pixels[pixel_index + 1] = color.g;
	buffer_pixels[pixel_index + 2] = color.b;
	buffer_pixels[pixel_index + 3] = color.a;
}

void Renderer::render_buffer()
{
	SDL_Texture* screen_texture = SDL_CreateTextureFromSurface(application_renderer, color_buffer);
	SDL_RenderTexture(application_renderer, screen_texture, NULL, NULL);
	SDL_DestroyTexture(screen_texture);
}
