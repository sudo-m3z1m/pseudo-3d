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
	
	const float focal_str = current_camera->get_h_focal_str(screen_width);
	int x_projection = screen_width / 2 - (focal_str * tan(relative_vector_rot));
	x_projection = SDL_min(screen_width, SDL_max(0, x_projection));
	
	return x_projection;
}

RendererColumn Renderer::get_wall_column(Vector2D<float> point, float floor_z, float ceiling_z)
{
	Vector2D<float> vector_to_point = point - current_camera->position;
	const float vector_length = vector_to_point * Vector2D<float>(cos(current_camera->rotation), sin(current_camera->rotation));
	
	Sector camera_sector = level_server->get_sector_by_index(current_camera->sector_index);
	const float camera_z = camera_sector.floor_z + current_camera->height_z;
	const float focal_str = current_camera->get_v_focal_str(screen_height);
	
	const float floor = floor_z - camera_z;
	const float ceiling = ceiling_z - camera_z;
	
	int bottom = (-floor / vector_length) * focal_str;
	int top = (ceiling / vector_length) * focal_str;
	
//	bottom = SDL_min(screen_height / 2, SDL_max(-screen_height / 2, bottom));
//	top = SDL_min(screen_height / 2, SDL_max(-screen_height / 2, top));
	
	return RendererColumn(bottom, top);
}

std::vector<RendererColumn> Renderer::get_wall_projection_columns(
	RendererColumn f_column,
	RendererColumn s_column,
	int f_pos_x,
	int x_length,
	std::vector<int> bottom_visplanes_id,
	std::vector<int> top_visplanes_id)
{
	std::vector<RendererColumn> wall_columns;
	
	int s_pos_x = f_pos_x + x_length;
	
	float bottom_columns_delta = s_column.bottom - f_column.bottom;
	float top_columns_delta = s_column.top - f_column.top;
	
	VisPlane& f_floor_visplane = visual_planes[top_visplanes_id[0]];
	VisPlane& f_ceiling_visplane = visual_planes[bottom_visplanes_id[0]];
	
	//FIXME: Need to make something else
	f_floor_visplane.min_x = f_pos_x < f_floor_visplane.min_x ? f_pos_x : f_floor_visplane.min_x;
	f_floor_visplane.max_x = s_pos_x > f_floor_visplane.max_x ? s_pos_x : f_floor_visplane.max_x;
	f_ceiling_visplane.min_x = f_pos_x < f_ceiling_visplane.min_x ? f_pos_x : f_ceiling_visplane.min_x;
	f_ceiling_visplane.max_x = s_pos_x > f_ceiling_visplane.max_x ? s_pos_x : f_ceiling_visplane.max_x;
	//FIXME: Need to make something else
	
	if(bottom_visplanes_id.size() == 2)
	{
		visual_planes[bottom_visplanes_id[1]].min_x = f_pos_x;
		visual_planes[bottom_visplanes_id[1]].max_x = f_pos_x + x_length;
	}
	
	if(top_visplanes_id.size() == 2)
	{
		visual_planes[top_visplanes_id[1]].min_x = f_pos_x;
		visual_planes[top_visplanes_id[1]].max_x = f_pos_x + x_length;
	}
	
	for(int delta_pos_x = 0; delta_pos_x < x_length; delta_pos_x++)
	{
		float height_k = float(delta_pos_x) / float(x_length);
		
		int wall_bottom = f_column.bottom + height_k * bottom_columns_delta;
		int wall_top = f_column.top + height_k * top_columns_delta;
		int global_bottom = (screen_height / 2) + wall_bottom;
		int global_top = (screen_height / 2) - wall_top;
		
		wall_columns.push_back(RendererColumn(wall_bottom, wall_top));
		
		int current_pos_x = f_pos_x + delta_pos_x;
		f_floor_visplane.plane_columns[current_pos_x].top = global_bottom;
		f_ceiling_visplane.plane_columns[current_pos_x].bottom = global_top;
		
		if(bottom_visplanes_id.size() == 2)
		{
			VisPlane& s_floor_visplane = visual_planes[bottom_visplanes_id[1]];
			s_floor_visplane.plane_columns[current_pos_x].bottom = global_bottom;
		}
		
		if(top_visplanes_id.size() == 2)
		{
			VisPlane& s_ceiling_visplane = visual_planes[top_visplanes_id[1]];
			s_ceiling_visplane.plane_columns[current_pos_x].top = global_top;
		}
	}
	
	return wall_columns;
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
//	render_horizontal();
	
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
//	int shape_sector_index = shape->sector_index;
//	std::vector<Wall> shape_walls = shape->walls;
//	std::vector<Vector2D<float>> shape_points = shape->points;
	
	bool is_shape_in_frustrum = current_camera->is_shape_in_frustrum(shape->points);
	if(!is_shape_in_frustrum) return;
	
	for(Wall current_wall : shape->walls)
	{
		render_shape_wall(shape, current_wall);
//		std::vector<Vector2D<float>> wall_points = current_wall.get_wall_points(shape_points);
//		Vector2D<float> to_wall_vector = wall_points[0] - current_camera->position;
//		
//		if(((to_wall_vector * current_wall.normal) >= 0)) continue;
//		
//		wall_points = current_camera->clip_wall_by_frustrum(wall_points);
//		if(wall_points.size() != 2) continue;
//		render_screen_wall(wall_points, current_wall, shape_sector_index);
	}
}

void Renderer::render_shape_wall(BSPShape* shape, Wall wall)
{
	std::vector<Vector2D<float>> wall_points = wall.get_wall_points(shape->points);
	Vector2D<float> to_wall_vector = wall_points[0] - current_camera->position;
	
	if(((to_wall_vector * wall.normal) >= 0)) return;

	wall_points = current_camera->clip_wall_by_frustrum(wall_points);
	
	if(wall_points.size() != 2) return;
	if(wall.window_component) return render_window(wall.window_component, wall_points);
	
	Sector shape_sector = level_server->get_sector_by_index(shape->sector_index);
	
	float sector_floor_z = shape_sector.floor_z;
	float sector_ceiling_z = shape_sector.ceiling_z;
	
	std::vector<int> floor_visplane_index = {get_visplane_index(sector_floor_z, shape_sector.floor_color)};
	std::vector<int> ceiling_visplane_index = {get_visplane_index(sector_ceiling_z, shape_sector.ceiling_color)};
	
	int f_pos_x = get_point_on_camera_projection(wall_points[0]);
	int s_pos_x = get_point_on_camera_projection(wall_points[1]);
	
	RendererColumn f_column = get_wall_column(wall_points[0], sector_floor_z, sector_ceiling_z);
	RendererColumn s_column = get_wall_column(wall_points[1], sector_floor_z, sector_ceiling_z);
	
	if(f_pos_x > s_pos_x)
	{
		std::swap(f_pos_x, s_pos_x);
		std::swap(f_column, s_column);
	}
	
	std::vector<RendererColumn> columns_to_render = get_wall_projection_columns(
		f_column,
		s_column,
		f_pos_x,
		s_pos_x - f_pos_x,
		ceiling_visplane_index,
		floor_visplane_index
	);
	render_wall_range(columns_to_render, f_pos_x, 0, wall.color);
}

void Renderer::render_wall_range(std::vector<RendererColumn> columns, int f_pos_x, int texture_index, Color color)
{
	for (size_t column_index = 0; column_index < columns.size(); column_index++)
	{
		RendererColumn column = columns[column_index];
		int pos_x = f_pos_x + (int)column_index;
		
		if(!is_screen_space_free(pos_x, column)) continue;
		
//		std::vector<RendererColumn> ranges_to_render = get_screen_column_ranges(pos_x, column);
//		for (RendererColumn range : ranges_to_render) render_column(pos_x, range, color);
		render_column(pos_x, column, color);
	}
}

//void Renderer::render_screen_wall(std::vector<Vector2D<float>> wall_points, Wall wall, int sector_index)
//{
//	if (wall.window_component) return render_window(wall.window_component, wall_points);
//	
//	//FIXME: stupid sector getting. It gets it in get_wall_column method.
//	Sector test_sector = level_server->get_sector_by_index(sector_index);
//	int floor_visual_plane_index = get_visplane_index(test_sector.floor_z, test_sector.floor_color);
//	
//	int f_edge_pos_x = get_point_on_camera_projection(wall_points[0]);
//	int s_edge_pos_x = get_point_on_camera_projection(wall_points[1]);
//	
//	RendererColumn f_edge_column = get_wall_column(wall_points[0], sector_index);
//	RendererColumn s_edge_column = get_wall_column(wall_points[1], sector_index);
//	
//	if(f_edge_pos_x > s_edge_pos_x)
//	{
//		std::swap(f_edge_pos_x, s_edge_pos_x);
//		std::swap(f_edge_column, s_edge_column);
//	}
//	
//	for(int wall_x = f_edge_pos_x; wall_x < s_edge_pos_x; wall_x++)
//	{
//		float height_k = float(wall_x - f_edge_pos_x) / float(s_edge_pos_x - f_edge_pos_x);
//		
//		int wall_bottom = f_edge_column.bottom + height_k * (s_edge_column.bottom - f_edge_column.bottom);
//		int wall_top = f_edge_column.top + height_k * (s_edge_column.top - f_edge_column.top);
//		RendererColumn current_column = RendererColumn(wall_bottom, wall_top);
//
//		if(!is_screen_space_free(wall_x, current_column)) continue;
//		std::vector<RendererColumn> ranges_to_render = get_screen_column_ranges(wall_x, current_column);
//		
//		for (RendererColumn range : ranges_to_render) //FIXME: Coloring is stupid shit. Broken architecture
//		{
//			//FIXME: It doesn't work very well - need to refactor this.
//			visual_planes[floor_visual_plane_index].plane_columns[wall_x].top = (screen_height / 2) + (range.bottom + 1);
//			render_column(wall_x, range, wall.color);
//		}
//	}
//}

void Renderer::render_window(WindowComponent* window, std::vector<Vector2D<float>> wall_points)
{
	Sector f_window_sector = level_server->get_sector_by_index(window->f_sector_index);
	Sector s_window_sector = level_server->get_sector_by_index(window->s_sector_index);
	
	float floor_delta = s_window_sector.floor_z - f_window_sector.floor_z;
	float ceiling_delta = s_window_sector.ceiling_z - f_window_sector.ceiling_z;
	
	if(floor_delta > 0) render_bottom_window(wall_points, window);
	if(ceiling_delta < 0) render_upper_window(wall_points, window);
	
//	if(floor_delta < 0 && ceiling_delta > 0)
//	{
//		int f_floor_visplane_index = get_visplane_index(f_window_sector.floor_z, f_window_sector.floor_color);
//		int s_floor_visplane_index = get_visplane_index(s_window_sector.floor_z, s_window_sector.floor_color);
//		int f_ceiling_visplane_index = get_visplane_index(f_window_sector.ceiling_z, f_window_sector.ceiling_color);
//		int s_ceiling_visplane_index = get_visplane_index(s_window_sector.ceiling_z, s_window_sector.ceiling_color);
//		
//		std::vector<int> bottom_planes_indeces = {f_ceiling_visplane_index, s_floor_visplane_index};
//		std::vector<int> top_planes_indeces = {f_floor_visplane_index, s_ceiling_visplane_index};
//		
//		int f_pos_x = get_point_on_camera_projection(wall_points[0]);
//		int s_pos_x = get_point_on_camera_projection(wall_points[1]);
//		
//		RendererColumn f_column = get_wall_column(wall_points[0], f_window_sector.floor_z, f_window_sector.ceiling_z);
//		RendererColumn s_column = get_wall_column(wall_points[1], f_window_sector.floor_z, f_window_sector.ceiling_z);
//		
//		if(f_pos_x > s_pos_x)
//		{
//			std::swap(f_pos_x, s_pos_x);
//			std::swap(f_column, s_column);
//		}
//		
//		get_wall_projection_columns(
//			f_column,
//			s_column,
//			f_pos_x,
//			s_pos_x - f_pos_x,
//			bottom_planes_indeces,
//			top_planes_indeces
//		);
//	}
}

void Renderer::render_bottom_window(std::vector<Vector2D<float>> wall_points, WindowComponent* window)
{
	Sector f_window_sector = level_server->get_sector_by_index(window->f_sector_index);
	Sector s_window_sector = level_server->get_sector_by_index(window->s_sector_index);
	
	int f_visplane_index = get_visplane_index(f_window_sector.floor_z, f_window_sector.floor_color);
	int s_visplane_index = get_visplane_index(s_window_sector.floor_z, s_window_sector.floor_color);
	
	std::vector<int> bottom_planes_indeces = {s_visplane_index};
	std::vector<int> top_planes_indeces = {f_visplane_index};
	
	//THIS: need to make it another function
	int f_pos_x = get_point_on_camera_projection(wall_points[0]);
	int s_pos_x = get_point_on_camera_projection(wall_points[1]);
	
	RendererColumn f_column = get_wall_column(wall_points[0], f_window_sector.floor_z, s_window_sector.floor_z);
	RendererColumn s_column = get_wall_column(wall_points[1], f_window_sector.floor_z, s_window_sector.floor_z);
	
	if(f_pos_x > s_pos_x)
	{
		std::swap(f_pos_x, s_pos_x);
		std::swap(f_column, s_column);
	}
	
	std::vector<RendererColumn> columns_to_render = get_wall_projection_columns(
		f_column,
		s_column,
		f_pos_x,
		s_pos_x - f_pos_x,
		bottom_planes_indeces,
		top_planes_indeces
	);
	//THIS: need to make it another function
	
	render_wall_range(columns_to_render, f_pos_x, 0, window->bottom_color);
}

void Renderer::render_upper_window(std::vector<Vector2D<float>> wall_points, WindowComponent* window)
{
	Sector f_window_sector = level_server->get_sector_by_index(window->f_sector_index);
	Sector s_window_sector = level_server->get_sector_by_index(window->s_sector_index);
	
	int f_visplane_index = get_visplane_index(f_window_sector.ceiling_z, f_window_sector.ceiling_color);
	int s_visplane_index = get_visplane_index(s_window_sector.ceiling_z, s_window_sector.ceiling_color);
	
	std::vector<int> bottom_planes_indeces = {f_visplane_index};
	std::vector<int> top_planes_indeces = {s_visplane_index};
	
	//THIS: need to make it another function
	int f_pos_x = get_point_on_camera_projection(wall_points[0]);
	int s_pos_x = get_point_on_camera_projection(wall_points[1]);
	
	RendererColumn f_column = get_wall_column(wall_points[0], s_window_sector.ceiling_z, f_window_sector.ceiling_z);
	RendererColumn s_column = get_wall_column(wall_points[1], s_window_sector.ceiling_z, f_window_sector.ceiling_z);
	
	if(f_pos_x > s_pos_x)
	{
		std::swap(f_pos_x, s_pos_x);
		std::swap(f_column, s_column);
	}
	
	std::vector<RendererColumn> columns_to_render = get_wall_projection_columns(
		f_column,
		s_column,
		f_pos_x,
		s_pos_x - f_pos_x,
		bottom_planes_indeces,
		top_planes_indeces
	);
	//THIS: need to make it another function
	
	render_wall_range(columns_to_render, f_pos_x, 0, window->upper_color);
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
		for(int pos_x = plane.min_x; pos_x < plane.max_x; pos_x++)
		{
			RendererColumn current_column = plane_columns[pos_x];
			Vector2D<int> draw_pos = Vector2D<int>(pos_x, pos_y);
			
			if((pos_y > current_column.bottom) || (pos_y < current_column.top)) continue;
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
