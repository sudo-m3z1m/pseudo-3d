#include "renderer.hpp"

Renderer::Renderer()
{
	screen_width = DEFAULT_SCREEN_WIDTH;
	screen_height = DEFAULT_SCREEN_HEIGHT;
	
	current_camera = nullptr;
	level_server = nullptr;
	SDL_CreateWindowAndRenderer(WINDOW_NAME, screen_width, screen_height, SDL_WINDOW_RESIZABLE, &application_window, &application_renderer);
	
	color_buffer = SDL_CreateSurface(screen_width, screen_height, SDL_PIXELFORMAT_RGBA32);
	screen_width_buffer = std::vector<std::vector<RendererColumn>>(screen_width, std::vector<RendererColumn>());
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
	
	screen_width_buffer = std::vector<std::vector<RendererColumn>>(screen_width, std::vector<RendererColumn>());
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
	std::vector<RendererColumn> current_columns = screen_width_buffer[x_point];
	
	for(RendererColumn column : current_columns)
		if((column.top <= new_column.top) && (column.bottom >= new_column.bottom)) return false;
	
	return true;
}

std::vector<RendererColumn> Renderer::get_screen_column_ranges(int x_point, RendererColumn new_column)
{
	x_point = SDL_min(screen_width, SDL_max(0, x_point));
	
	std::vector<RendererColumn> current_columns = screen_width_buffer[x_point];
	std::vector<RendererColumn> column_ranges;
	
	column_ranges = new_column.subtract_columns(current_columns);
	screen_width_buffer[x_point] = new_column.merge_columns(current_columns);
	
	return column_ranges;
	
//	if(!current_column.bottom && !current_column.top)
//	{
//		column_ranges.push_back(new_column);
//		screen_width_buffer[x_point] = new_column;
//		return column_ranges;
//	}
//	
//	const bool is_bottom_higher = current_column.bottom < new_column.bottom, is_top_higher = current_column.top < new_column.top;
//	if(is_bottom_higher) //FIXME: It can return a value beyond screen
//	{
//		column_ranges.push_back(RendererColumn(new_column.bottom, -current_column.bottom));
//		current_column.bottom = new_column.bottom;
//	}
//	if(is_top_higher)
//	{
//		column_ranges.push_back(RendererColumn(-current_column.top, new_column.top));
//		current_column.top = new_column.top;
//	}
//	
//	screen_width_buffer[x_point] = current_column;
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
	
	if(vector_length <= 0.1) return RendererColumn();
	
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
	int x_length)
{
	std::vector<RendererColumn> wall_columns;
	
	float bottom_columns_delta = s_column.bottom - f_column.bottom;
	float top_columns_delta = s_column.top - f_column.top;
	
	for(int delta_pos_x = 0; delta_pos_x < x_length; delta_pos_x++)
	{
		float height_k = float(delta_pos_x) / float(x_length);
		
		int wall_bottom = f_column.bottom + height_k * bottom_columns_delta;
		int wall_top = f_column.top + height_k * top_columns_delta;
		
		wall_columns.push_back(RendererColumn(wall_bottom, wall_top));
	}
	
	return wall_columns;
}

void Renderer::paste_planes_column(std::vector<RendererColumn> column_ranges, int pos_x, std::vector<int> floor_visplanes_id, std::vector<int> ceiling_visplanes_id)
{
	int bottom = column_ranges[0].bottom, top = column_ranges[0].top;

	for (RendererColumn column : column_ranges)
	{
		if(top > column.top) top = column.top;
		if(bottom < column.bottom) bottom = column.bottom;
	}
	
	if(floor_visplanes_id.size() != 0)
	{
		VisPlane& floor_plane = visual_planes[floor_visplanes_id[0]];
		floor_plane.set_x_range(pos_x);
		floor_plane.plane_columns[pos_x].top = bottom;
	}
	
	if(ceiling_visplanes_id.size() != 0)
	{
		VisPlane& ceiling_plane = visual_planes[ceiling_visplanes_id[0]];
		ceiling_plane.set_x_range(pos_x);
		ceiling_plane.plane_columns[pos_x].bottom = top;
	}
	
	std::vector<VisPlane*> window_planes = std::vector<VisPlane*>();
	
	if(floor_visplanes_id.size() == 2) window_planes.push_back(&visual_planes[floor_visplanes_id[1]]);
	if(ceiling_visplanes_id.size() == 2) window_planes.push_back(&visual_planes[ceiling_visplanes_id[1]]);
	
	for (VisPlane* plane : window_planes)
	{
		plane->set_x_range(pos_x);
		
		plane->plane_columns[pos_x].top = top;
		plane->plane_columns[pos_x].bottom = bottom;
	}
}

void Renderer::clear_screen_width_buffer()
{
	for(size_t buffer_index = 0; buffer_index < screen_width_buffer.size(); buffer_index++)
	{
		RendererColumn default_column = RendererColumn(0, 0);
		screen_width_buffer[buffer_index] = std::vector<RendererColumn>();
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
	
	bool is_shape_in_frustrum = current_camera->is_shape_in_frustrum(shape->points);
	if(!is_shape_in_frustrum) return;
	
	for(Wall current_wall : shape->walls)
	{
		render_shape_wall(shape, current_wall);
	}
}

void Renderer::render_shape_wall(BSPShape* shape, Wall wall)
{
	std::vector<Vector2D<float>> wall_points = wall.get_wall_points(shape->points);
	float wall_camera_dot_product = (wall_points[0] - current_camera->position) * wall.normal;
	
	if((wall_camera_dot_product) >= 0) return;

	wall_points = current_camera->clip_wall_by_frustrum(wall_points);
	
	if(wall_points.size() != 2) return;
	if(wall.window_component) return render_window(wall.window_component, wall_points);
	
	Sector shape_sector = level_server->get_sector_by_index(shape->sector_index);
	
	float sector_floor_z = shape_sector.floor_z;
	float sector_ceiling_z = shape_sector.ceiling_z;
	
	int floor_visplane_index = get_visplane_index(sector_floor_z, shape_sector.floor_color);
	int ceiling_visplane_index = get_visplane_index(sector_ceiling_z, shape_sector.ceiling_color);
	
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
		s_pos_x - f_pos_x
	);
	render_wall_range(columns_to_render, f_pos_x, 0, wall.color, ceiling_visplane_index, floor_visplane_index);
}

void Renderer::render_wall_range(std::vector<RendererColumn> columns, int f_pos_x, int texture_index, Color color, int bottom_plane_id, int top_plane_id)
{
	for (size_t column_index = 0; column_index < columns.size(); column_index++)
	{
		RendererColumn column = columns[column_index];
		column.bottom = screen_height / 2 + column.bottom;
		column.top = screen_height / 2 - column.top;
		int pos_x = f_pos_x + (int)column_index;
		
		if(!is_screen_space_free(pos_x, column)) continue;
		
		std::vector<RendererColumn> ranges_to_render = get_screen_column_ranges(pos_x, column);
		paste_planes_column(ranges_to_render, pos_x, {top_plane_id}, {bottom_plane_id});
		for (RendererColumn range : ranges_to_render) render_column(pos_x, range, color);
	}
}

void Renderer::render_window(WindowComponent* window, std::vector<Vector2D<float>> wall_points)
{
	Sector f_window_sector = level_server->get_sector_by_index(window->f_sector_index);
	Sector s_window_sector = level_server->get_sector_by_index(window->s_sector_index);
	
	float floor_delta = s_window_sector.floor_z - f_window_sector.floor_z;
	float ceiling_delta = s_window_sector.ceiling_z - f_window_sector.ceiling_z;
	
	int f_floor_visplane_index = get_visplane_index(f_window_sector.floor_z, f_window_sector.floor_color);
	int s_floor_visplane_index = get_visplane_index(s_window_sector.floor_z, s_window_sector.floor_color);
	
	int f_ceiling_visplane_index = get_visplane_index(f_window_sector.ceiling_z, f_window_sector.ceiling_color);
	int s_ceiling_visplane_index = get_visplane_index(s_window_sector.ceiling_z, s_window_sector.ceiling_color);
	
	std::vector<int> floor_visplanes_indeces = {f_floor_visplane_index, s_floor_visplane_index};
	std::vector<int> ceiling_visplanes_indeces = {f_ceiling_visplane_index, s_ceiling_visplane_index};
	
	if(floor_delta > 0)
	{
		floor_visplanes_indeces.clear();
		render_bottom_window(wall_points, window);
	}
	if(ceiling_delta < 0)
	{
		ceiling_visplanes_indeces.clear();
		render_upper_window(wall_points, window);
	}
	
	if(floor_delta > 0 && ceiling_delta < 0) return;
	
	int f_pos_x = get_point_on_camera_projection(wall_points[0]);
	int s_pos_x = get_point_on_camera_projection(wall_points[1]);
	
	RendererColumn f_column = get_wall_column(wall_points[0], f_window_sector.floor_z, f_window_sector.ceiling_z);
	RendererColumn s_column = get_wall_column(wall_points[1], f_window_sector.floor_z, f_window_sector.ceiling_z);
	
	if(f_pos_x > s_pos_x)
	{
		std::swap(f_pos_x, s_pos_x);
		std::swap(f_column, s_column);
	}
	
	std::vector<RendererColumn> columns = get_wall_projection_columns(
		f_column,
		s_column,
		f_pos_x,
		s_pos_x - f_pos_x
	);
	
	for (size_t column_index = 0; column_index < columns.size(); column_index++)
	{
		RendererColumn column = columns[column_index];
		column.bottom = screen_height / 2 + column.bottom;
		column.top = screen_height / 2 - column.top;
		int pos_x = f_pos_x + (int)column_index;
		
		if(!is_screen_space_free(pos_x, column)) continue;
		
		paste_planes_column({column}, pos_x, floor_visplanes_indeces, ceiling_visplanes_indeces);
	}
}

void Renderer::render_bottom_window(std::vector<Vector2D<float>> wall_points, WindowComponent* window)
{
	Sector f_window_sector = level_server->get_sector_by_index(window->f_sector_index);
	Sector s_window_sector = level_server->get_sector_by_index(window->s_sector_index);
	
	int f_visplane_index = get_visplane_index(f_window_sector.floor_z, f_window_sector.floor_color);
	int s_visplane_index = get_visplane_index(s_window_sector.floor_z, s_window_sector.floor_color);
	
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
		s_pos_x - f_pos_x
	);
	//THIS: need to make it another function
	
	render_wall_range(columns_to_render, f_pos_x, 0, window->bottom_color, s_visplane_index, f_visplane_index);
}

void Renderer::render_upper_window(std::vector<Vector2D<float>> wall_points, WindowComponent* window)
{
	Sector f_window_sector = level_server->get_sector_by_index(window->f_sector_index);
	Sector s_window_sector = level_server->get_sector_by_index(window->s_sector_index);
	
	int f_visplane_index = get_visplane_index(f_window_sector.ceiling_z, f_window_sector.ceiling_color);
	int s_visplane_index = get_visplane_index(s_window_sector.ceiling_z, s_window_sector.ceiling_color);
	
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
		s_pos_x - f_pos_x
	);
	//THIS: need to make it another function
	
	render_wall_range(columns_to_render, f_pos_x, 0, window->upper_color, f_visplane_index, s_visplane_index);
}

void Renderer::render_horizontal()
{
	for(const VisPlane& current_plane : visual_planes)
	{
		render_plane(current_plane);
	}
}

void Renderer::render_plane(const VisPlane& plane)
{
	std::vector<RendererColumn> plane_columns = plane.plane_columns;
	for(int pos_x = plane.min_x; pos_x < plane.max_x; pos_x++)
	{
		RendererColumn column = plane_columns[pos_x];
		if(column.top >= column.bottom) continue;
		render_column(pos_x, column, plane.plane_color);
	}
//FIXME: Method is lagging
//	for (int pos_y = 0; pos_y < screen_height; pos_y++)
//	{
//		for(int pos_x = plane.min_x; pos_x < plane.max_x; pos_x++)
//		{
//			RendererColumn current_column = plane_columns[pos_x];
//			Vector2D<int> draw_pos = Vector2D<int>(pos_x, pos_y);
//			
//			if((pos_y > current_column.bottom) || (pos_y < current_column.top)) continue;
//			draw_pixel_in_buffer(draw_pos, plane.plane_color);
//		}
//	}
//FIXME: Method is lagging
}

void Renderer::render_column(int pos_x, RendererColumn& range, Color color)
{
	for(int pos_y = range.top; pos_y < range.bottom; pos_y++)
	{
		int rounded_pos_y = SDL_min(screen_height, SDL_max(0, pos_y));
		int rounded_pos_x = SDL_min(screen_width, SDL_max(0, pos_x));
		draw_pixel_in_buffer(Vector2D<int>(rounded_pos_x, rounded_pos_y), color);
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
