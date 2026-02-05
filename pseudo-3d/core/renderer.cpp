#include "renderer.hpp"

Renderer::Renderer()
{
	screen_width = DEFAULT_SCREEN_WIDTH;
	screen_height = DEFAULT_SCREEN_HEIGHT;
	
	current_camera = nullptr;
	level_server = nullptr;
	SDL_CreateWindowAndRenderer(WINDOW_NAME, screen_width, screen_height, SDL_WINDOW_RESIZABLE, &application_window, &application_renderer);
	
	color_buffer = SDL_CreateSurface(screen_width, screen_height, SDL_PIXELFORMAT_RGBA32);
	screen_width_buffer = std::vector<RendererColumn>(screen_width, RendererColumn(0, 0));
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
	
	screen_width_buffer = std::vector<RendererColumn>(screen_width, RendererColumn(0, 0));
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

int Renderer::get_visplane_index(float height_z, Color color)
{
	for (int current_plane_index = 0; current_plane_index < visual_planes.size(); current_plane_index++)
	{
		VisPlane current_plane = visual_planes[current_plane_index];
		if ((current_plane.height_z == height_z) && (current_plane.plane_color == color))
			return current_plane_index;
	}
	
	VisPlane new_visual_plane = VisPlane(height_z, color, screen_width, screen_height);
	visual_planes.push_back(new_visual_plane);
	
	return ((int)visual_planes.size() - 1);
}

bool Renderer::is_screen_space_free(int x_point, RendererColumn new_column)
{
	x_point = SDL_min(screen_width, SDL_max(0, x_point));
	RendererColumn current_column = screen_width_buffer[x_point];
	
	bool is_column_visible = (current_column.bottom < new_column.bottom) || (current_column.top < new_column.top);
	return is_column_visible;
}

std::vector<RendererColumn> Renderer::get_screen_column_ranges(int x_point, RendererColumn new_column)
{
	x_point = SDL_min(screen_width, SDL_max(0, x_point));
	
	RendererColumn current_column = screen_width_buffer[x_point];
	std::vector<RendererColumn> column_ranges;
	
	if(!current_column.bottom && !current_column.top)
	{
		column_ranges.push_back(new_column);
		screen_width_buffer[x_point] = new_column;
		return column_ranges;
	}
	
	const bool is_bottom_higher = current_column.bottom < new_column.bottom, is_top_higher = current_column.top < new_column.top;
	if(is_bottom_higher) //FIXME: It can return a value beyond screen
	{
		column_ranges.push_back(RendererColumn(new_column.bottom, -current_column.bottom));
		current_column.bottom = new_column.bottom;
	}
	if(is_top_higher)
	{
		column_ranges.push_back(RendererColumn(-current_column.top, new_column.top));
		current_column.top = new_column.top;
	}
	
	screen_width_buffer[x_point] = current_column;
	return column_ranges;
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

RendererColumn Renderer::get_wall_column(Vector2D<float> point, int sector_index)
{
	Vector2D<float> vector_to_point = point - current_camera->position;
	const float vector_length = vector_to_point * Vector2D<float>(cos(current_camera->rotation), sin(current_camera->rotation));
	const float focal_str = (screen_height / 2) / (tan(current_camera->field_of_view / 2));
	
	Sector shape_sector = level_server->get_sector_by_index(sector_index);
	Sector camera_sector = level_server->get_sector_by_index(current_camera->sector_index);
	const float camera_z = camera_sector.floor_z + current_camera->height_z;
	
	const float floor = shape_sector.floor_z - camera_z;
	const float ceiling = shape_sector.ceiling_z - camera_z;
	
	int bottom = (-floor / vector_length) * focal_str;
	int top = (ceiling / vector_length) * focal_str; //FIXME: static height - isn't depend on sector now
	
//	bottom = SDL_min(screen_height / 2, SDL_max(-screen_height / 2, bottom));
//	top = SDL_min(screen_height / 2, SDL_max(-screen_height / 2, top));
	
	return RendererColumn(bottom, top);
}

void Renderer::clear_screen_width_buffer()
{
	for(size_t buffer_index = 0; buffer_index < screen_width_buffer.size(); buffer_index++)
	{
		RendererColumn default_column = RendererColumn(0, 0);
		screen_width_buffer[buffer_index] = default_column;
	}
	visual_planes.clear();
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
	render_horizontal();
	
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
	int shape_sector_index = shape->sector_index;
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
		if(wall_points.size() != 2) continue;
		render_wall(wall_points, current_wall, shape_sector_index);
	}
}

void Renderer::render_wall(std::vector<Vector2D<float>> wall_points, Wall wall, int sector_index)
{
	if (wall.window_component) return render_window(wall.window_component, wall_points);
	
	//FIXME: stupid sector getting. It gets it in get_wall_column method.
	Sector test_sector = level_server->get_sector_by_index(sector_index);
	int floor_visual_plane_index = get_visplane_index(test_sector.floor_z, test_sector.floor_color);
	
	int f_edge_pos_x = get_point_on_camera_projection(wall_points[0]);
	int s_edge_pos_x = get_point_on_camera_projection(wall_points[1]);
	
	RendererColumn f_edge_column = get_wall_column(wall_points[0], sector_index);
	RendererColumn s_edge_column = get_wall_column(wall_points[1], sector_index);
	
	if(f_edge_pos_x > s_edge_pos_x)
	{
		std::swap(f_edge_pos_x, s_edge_pos_x);
		std::swap(f_edge_column, s_edge_column);
	}
	
	for(int wall_x = f_edge_pos_x; wall_x < s_edge_pos_x; wall_x++)
	{
		float height_k = float(wall_x - f_edge_pos_x) / float(s_edge_pos_x - f_edge_pos_x);
		
		int wall_bottom = f_edge_column.bottom + height_k * (s_edge_column.bottom - f_edge_column.bottom);
		int wall_top = f_edge_column.top + height_k * (s_edge_column.top - f_edge_column.top);
		RendererColumn current_column = RendererColumn(wall_bottom, wall_top);

		if(!is_screen_space_free(wall_x, current_column)) continue;
		std::vector<RendererColumn> ranges_to_render = get_screen_column_ranges(wall_x, current_column);
		
		for (RendererColumn range : ranges_to_render) //FIXME: Coloring is stupid shit. Broken architecture
		{
			//FIXME: It doesn't work very well - need to refactor this.
			visual_planes[floor_visual_plane_index].plane_columns[wall_x].top = (screen_height / 2) + (range.bottom + 1);
			render_column(wall_x, range, wall.color);
		}
	}
}

void Renderer::render_window(WindowComponent* window, std::vector<Vector2D<float>> wall_points)
{
	Sector f_window_sector = level_server->get_sector_by_index(window->f_sector_index);
	Sector s_window_sector = level_server->get_sector_by_index(window->s_sector_index);
	
//	float floor_delta = s_window_sector.floor_z - f_window_sector.floor_z;
//	float ceiling_delta = s_window_sector.ceiling_z - f_window_sector.ceiling_z;
	
	render_bottom_window(wall_points, window);
	render_upper_window(wall_points, window);
	
//	if(floor_delta > 0) render_bottom_window(wall_points, window);
//	if(ceiling_delta < 0) render_upper_window(wall_points, window);
}

void Renderer::render_bottom_window(std::vector<Vector2D<float>> wall_points, WindowComponent* window) //FIXME: Method is stupid. Need to change logic to draw_wall method.
{
	Sector camera_sector = level_server->get_sector_by_index(current_camera->sector_index);
	Sector f_sector = level_server->get_sector_by_index(window->f_sector_index);
	Sector s_sector = level_server->get_sector_by_index(window->s_sector_index);
	const float world_camera_z = camera_sector.floor_z + current_camera->height_z;
	
	//FIXME: It needs only second sector's visplane bottom editing in fact
	int f_visual_plane_index = get_visplane_index(f_sector.floor_z, f_sector.floor_color);
	int s_visual_plane_index = get_visplane_index(s_sector.floor_z, s_sector.floor_color);
	
	int f_pos_x = get_point_on_camera_projection(wall_points[0]);
	int s_pos_x = get_point_on_camera_projection(wall_points[1]);
	
	Vector2D<float> vector_to_f_point = wall_points[0] - current_camera->position;
	Vector2D<float> vector_to_s_point = wall_points[1] - current_camera->position;
	
	if(f_pos_x > s_pos_x)
	{
		std::swap(f_pos_x, s_pos_x);
		std::swap(vector_to_f_point, vector_to_s_point);
	}
	
	const float f_vector_length = vector_to_f_point * Vector2D<float>(cos(current_camera->rotation), sin(current_camera->rotation));
	const float s_vector_length = vector_to_s_point * Vector2D<float>(cos(current_camera->rotation), sin(current_camera->rotation));
	
	const float focal_str = (screen_height / 2) / (tan(current_camera->field_of_view / 2));
	
	const float f_floor = f_sector.floor_z - world_camera_z;
	const float s_floor = s_sector.floor_z - world_camera_z;
	
	int f_edge_bottom_bottom = (-f_floor / f_vector_length) * focal_str;
	int f_edge_bottom_top = (s_floor / f_vector_length) * focal_str;
	int s_edge_bottom_bottom = (-f_floor / s_vector_length) * focal_str;
	int s_edge_bottom_top = (s_floor / s_vector_length) * focal_str;
	
//	f_edge_bottom_bottom = SDL_min(screen_height / 2, SDL_max(-screen_height / 2, f_edge_bottom_bottom));
//	f_edge_bottom_top = SDL_min(screen_height / 2, SDL_max(-screen_height / 2, f_edge_bottom_top));
//	s_edge_bottom_bottom = SDL_min(screen_height / 2, SDL_max(-screen_height / 2, s_edge_bottom_bottom));
//	s_edge_bottom_top = SDL_min(screen_height / 2, SDL_max(-screen_height / 2, s_edge_bottom_top));
	
	RendererColumn f_column = RendererColumn(f_edge_bottom_bottom, f_edge_bottom_top);
	RendererColumn s_column = RendererColumn(s_edge_bottom_bottom, s_edge_bottom_top);
	
	for(int wall_x = f_pos_x; wall_x < s_pos_x; wall_x++)
	{
		float height_k = float(wall_x - f_pos_x) / float(s_pos_x - f_pos_x);
		
		int wall_bottom = f_column.bottom + height_k * (s_column.bottom - f_column.bottom);
		int wall_top = f_column.top + height_k * (s_column.top - f_column.top);
		
		RendererColumn new_column = RendererColumn(wall_bottom, wall_top);
		if(!is_screen_space_free(wall_x, new_column)) continue;
		
		visual_planes[f_visual_plane_index].plane_columns[wall_x].top = (screen_width / 2) + (wall_bottom + 1);
		visual_planes[s_visual_plane_index].plane_columns[wall_x].bottom = (screen_width / 2) - (wall_top - 1);
		if(f_sector.floor_z > s_sector.floor_z)
		{
			visual_planes[f_visual_plane_index].plane_columns[wall_x].top = (screen_width / 2) + (wall_bottom + 1);
			visual_planes[s_visual_plane_index].plane_columns[wall_x].bottom = (screen_width / 2) + (wall_bottom - 1);
		}
		
		render_column(wall_x, new_column, window->bottom_color);
	}
}

void Renderer::render_upper_window(std::vector<Vector2D<float>> wall_points, WindowComponent* window)
{
	Sector camera_sector = level_server->get_sector_by_index(current_camera->sector_index);
	Sector f_sector = level_server->get_sector_by_index(window->f_sector_index);
	Sector s_sector = level_server->get_sector_by_index(window->s_sector_index);
	const float world_camera_z = camera_sector.floor_z + current_camera->height_z;
	
	int f_pos_x = get_point_on_camera_projection(wall_points[0]);
	int s_pos_x = get_point_on_camera_projection(wall_points[1]);
	
	Vector2D<float> vector_to_f_point = wall_points[0] - current_camera->position;
	Vector2D<float> vector_to_s_point = wall_points[1] - current_camera->position;
	
	if(f_pos_x > s_pos_x)
	{
		std::swap(f_pos_x, s_pos_x);
		std::swap(vector_to_f_point, vector_to_s_point);
	}
	
	const float f_vector_length = vector_to_f_point * Vector2D<float>(cos(current_camera->rotation), sin(current_camera->rotation));
	const float s_vector_length = vector_to_s_point * Vector2D<float>(cos(current_camera->rotation), sin(current_camera->rotation));
	
	const float focal_str = (screen_height / 2) / (tan(current_camera->field_of_view / 2));
	
	const float f_ceiling = s_sector.ceiling_z - world_camera_z;
	const float s_ceiling = f_sector.ceiling_z - world_camera_z;
	
	int f_edge_bottom_bottom = (-f_ceiling / f_vector_length) * focal_str;
	int f_edge_bottom_top = (s_ceiling / f_vector_length) * focal_str;
	int s_edge_bottom_bottom = (-f_ceiling / s_vector_length) * focal_str;
	int s_edge_bottom_top = (s_ceiling / s_vector_length) * focal_str;
	
//	f_edge_bottom_bottom = SDL_min(screen_height / 2, SDL_max(-screen_height / 2, f_edge_bottom_bottom));
//	f_edge_bottom_top = SDL_min(screen_height / 2, SDL_max(-screen_height / 2, f_edge_bottom_top));
//	s_edge_bottom_bottom = SDL_min(screen_height / 2, SDL_max(-screen_height / 2, s_edge_bottom_bottom));
//	s_edge_bottom_top = SDL_min(screen_height / 2, SDL_max(-screen_height / 2, s_edge_bottom_top));
	
	RendererColumn f_column = RendererColumn(f_edge_bottom_bottom, f_edge_bottom_top);
	RendererColumn s_column = RendererColumn(s_edge_bottom_bottom, s_edge_bottom_top);
	
	for(int wall_x = f_pos_x; wall_x < s_pos_x; wall_x++)
	{
		float height_k = float(wall_x - f_pos_x) / float(s_pos_x - f_pos_x);
		
		int wall_bottom = f_column.bottom + height_k * (s_column.bottom - f_column.bottom);
		int wall_top = f_column.top + height_k * (s_column.top - f_column.top);
		
		RendererColumn new_column = RendererColumn(wall_bottom, wall_top);
		if(!is_screen_space_free(wall_x, new_column)) continue;
		
		render_column(wall_x, new_column, window->upper_color);
	}
}

void Renderer::render_horizontal()
{
	for(VisPlane current_plane : visual_planes)
	{
		render_plane(current_plane);
	}
}

void Renderer::render_plane(VisPlane plane)
{
	std::vector<RendererColumn> plane_columns = plane.plane_columns;
	for (int pos_y = 0; pos_y < screen_height; pos_y++)
	{
		for(int pos_x = 0; pos_x < screen_width; pos_x++)
		{
			RendererColumn current_column = plane_columns[pos_x];
			Vector2D<int> draw_pos = Vector2D<int>(pos_x, pos_y);
			
			//FIXME: Default data is stupid
			if((current_column.top == -1) || (pos_y > current_column.bottom) || (pos_y < current_column.top)) continue;
			draw_pixel_in_buffer(draw_pos, plane.plane_color);
		}
	}
}

void Renderer::render_column(int pos_x, RendererColumn range, Color color) //FIXME: Need to make column ranges renderign here. Not in wall method
{
	int height = range.bottom + range.top;
	const int y_start_pos = (screen_height / 2) + range.bottom;
	for(int cur_height = 0; cur_height < height; cur_height++)
	{
		int pos_y = y_start_pos - cur_height;
		pos_y = SDL_min(screen_height, SDL_max(0, pos_y));
		draw_pixel_in_buffer(Vector2D<int>(pos_x, pos_y), color);
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
