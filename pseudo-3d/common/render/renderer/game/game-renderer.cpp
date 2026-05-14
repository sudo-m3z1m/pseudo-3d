#include "game-renderer.hpp"

GameRenderer::GameRenderer() : Renderer()
{
	level_server = nullptr;
	screen_width_buffer = std::vector<std::vector<RendererColumn>>(screen_width, std::vector<RendererColumn>());
	visplanes_clip_buffer = std::vector<RendererColumn>(screen_width, RendererColumn(screen_height - 1, 0));
}

GameRenderer::GameRenderer(Camera* camera, TextureBuffer* texture_buffer, FileServer* file_server, int width, int height, BSPLevelServer* level_server) :
	Renderer(camera, texture_buffer, file_server, width, height)
{
	this->level_server = level_server;
	screen_width_buffer = std::vector<std::vector<RendererColumn>>(screen_width, std::vector<RendererColumn>());
	visplanes_clip_buffer = std::vector<RendererColumn>(screen_width, RendererColumn(screen_height - 1, 0));
	
//	IMGUI_CHECKVERSION();
//	ImGui::CreateContext();
//	ImGui_ImplSDL3_InitForSDLRenderer(application_window, application_renderer);
//	ImGui_ImplSDLRenderer3_Init(application_renderer);
}

GameRenderer::~GameRenderer()
{
	delete current_camera;
	delete level_server;
	delete color_buffer;
}

int GameRenderer::get_visplane_index(float height_z, Color color, int tid)
{
	for (int current_plane_index = 0; current_plane_index < visual_planes.size(); current_plane_index++)
	{
		VisPlane current_plane = visual_planes[current_plane_index];
		if ((current_plane.height_z == height_z) && (current_plane.plane_color == color) && (current_plane.tid == tid))
			return current_plane_index;
	}
	
	VisPlane new_visual_plane = VisPlane(height_z, color, screen_width, screen_height, tid);
	visual_planes.push_back(new_visual_plane);
	
	return ((int)visual_planes.size() - 1);
}

void GameRenderer::clear_screen_width_buffer()
{
	for(size_t buffer_index = 0; buffer_index < screen_width_buffer.size(); buffer_index++)
	{
		RendererColumn default_column = RendererColumn(0, 0);
		screen_width_buffer[buffer_index] = std::vector<RendererColumn>();
	}
	visual_planes.clear();
}

bool GameRenderer::is_screen_space_free(int x_point, RendererColumn new_column)
{
	std::vector<RendererColumn> current_columns = screen_width_buffer[x_point];
	
	for(RendererColumn column : current_columns)
		if((column.top <= new_column.top) && (column.bottom >= new_column.bottom)) return false;
	
	return true;
}

std::vector<RendererColumn> GameRenderer::get_screen_column_ranges(int x_point, RendererColumn new_column, bool is_outside)
{
	std::vector<RendererColumn> current_columns = screen_width_buffer[x_point];
	std::vector<RendererColumn> column_ranges;
	
	column_ranges = new_column.subtract_columns(current_columns);
	screen_width_buffer[x_point] = {RendererColumn(screen_height - 1, 0)};
	if(is_outside)
		screen_width_buffer[x_point] = new_column.merge_columns(current_columns);
	
	return column_ranges;
}

void GameRenderer::render()
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

void GameRenderer::render_node(BSPNode* node)
{
	//FIXME: If without tree it isn't working btw
	if (!node->shapes.empty())
	{
		for(BSPShape*& shape : node->shapes) render_bsp_shape(shape);
		return;
	}
	//FIXME: If without tree it isn't working btw
	
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

void GameRenderer::render_bsp_shape(BSPShape*& shape)
{
	bool is_shape_in_frustrum = current_camera->is_shape_in_frustrum(shape->points);
	if(!is_shape_in_frustrum) return;
	
	std::vector<Wall> windows;
	
	for(Wall& current_wall : shape->walls)
	{
		if(current_wall.window_component) //FIXME: If we not rendering windows here why we need this statement in render_shape_wall?
		{
			windows.push_back(current_wall);
			continue;
		}
		render_shape_wall(shape, current_wall);
	}
	
	for(Wall& window : windows)
	{
		render_shape_wall(shape, window);
	}
}
void GameRenderer::render_shape_wall(BSPShape* shape, Wall wall)
{
	std::vector<Vector2D<float>> wall_points;
	std::vector<Vector2D<float>> raw_wall_points = wall.get_wall_points(shape->points);
	
	float wall_camera_dot_product = (raw_wall_points[0] - current_camera->position) * wall.normal;
	if((wall_camera_dot_product) >= 0) return;
	
	wall_points = current_camera->clip_wall_by_frustrum(raw_wall_points);
	if(wall_points.size() != 2) return;
	
	if(wall.window_component) return render_window(wall.window_component, raw_wall_points, wall_points);
	
	Sector shape_sector = level_server->get_sector_by_index(shape->sector_index);
	
	float sector_floor_z = shape_sector.floor_z;
	float sector_ceiling_z = shape_sector.ceiling_z;
	
	int floor_visplane_index = get_visplane_index(sector_floor_z, shape_sector.floor_color, shape_sector.floor_tid);
	int ceiling_visplane_index = get_visplane_index(sector_ceiling_z, shape_sector.ceiling_color, shape_sector.ceiling_tid);
	
	int f_pos_x = get_point_on_camera_projection(wall_points[0]);
	int s_pos_x = get_point_on_camera_projection(wall_points[1]);
	
	int tid = wall.tid;
	
	RendererColumn f_column = get_wall_column(wall_points[0], sector_floor_z, sector_ceiling_z);
	RendererColumn s_column = get_wall_column(wall_points[1], sector_floor_z, sector_ceiling_z);
	
	if(f_pos_x > s_pos_x) //FIXME: It doesn't needed actually
	{
		std::swap(f_pos_x, s_pos_x);
		std::swap(f_column, s_column);
		
		std::swap(raw_wall_points[0], raw_wall_points[1]);
		std::swap(wall_points[0], wall_points[1]);
	}
	
	Vector2D<float> wall_offsets = get_wall_offsets(raw_wall_points, wall_points);
		//FIXME: Need to make RenderWall class or smth else
	std::vector<std::vector<RendererColumn>> columns_to_render = get_wall_projection_columns(
																							 wall_points,
																							 f_column,
																							 s_column,
																							 wall.window_component,
																							 wall_offsets,
																							 f_pos_x,
																							 tid,
																							 {floor_visplane_index},
																							 {ceiling_visplane_index},
																							 s_pos_x - f_pos_x,
																							 false
																							 );
		//FIXME: Need to make RenderWall class or smth else
	render_wall_range(columns_to_render, f_pos_x, tid, wall.color);
}

void GameRenderer::render_window(WindowComponent* window, std::vector<Vector2D<float>> raw_wall_points, std::vector<Vector2D<float>> wall_points)
{
	Sector f_window_sector = level_server->get_sector_by_index(window->f_sector_index);
	Sector s_window_sector = level_server->get_sector_by_index(window->s_sector_index);
	
	float floor_delta = s_window_sector.floor_z - f_window_sector.floor_z;
	float ceiling_delta = s_window_sector.ceiling_z - f_window_sector.ceiling_z;
	
	int f_floor_visplane_index = get_visplane_index(f_window_sector.floor_z, f_window_sector.floor_color, f_window_sector.floor_tid);
	int s_floor_visplane_index = get_visplane_index(s_window_sector.floor_z, s_window_sector.floor_color, s_window_sector.floor_tid);
	
	int f_ceiling_visplane_index = get_visplane_index(f_window_sector.ceiling_z, f_window_sector.ceiling_color, f_window_sector.ceiling_tid);
	int s_ceiling_visplane_index = get_visplane_index(s_window_sector.ceiling_z, s_window_sector.ceiling_color, s_window_sector.ceiling_tid);
	
	std::vector<int> floor_visplanes_indeces = {f_floor_visplane_index, s_floor_visplane_index};
	std::vector<int> ceiling_visplanes_indeces = {f_ceiling_visplane_index, s_ceiling_visplane_index};
	
	if(floor_delta > 0)
	{
		floor_visplanes_indeces.clear();
		render_bottom_window(raw_wall_points, wall_points, window);
	}
	if(ceiling_delta < 0)
	{
		ceiling_visplanes_indeces.clear();
		render_upper_window(raw_wall_points, wall_points, window);
	}
	
	if(floor_delta > 0 && ceiling_delta < 0) return;
	
	int f_pos_x = get_point_on_camera_projection(wall_points[0]);
	int s_pos_x = get_point_on_camera_projection(wall_points[1]);
	
	RendererColumn f_column = get_wall_column(wall_points[0], f_window_sector.floor_z, f_window_sector.ceiling_z);
	RendererColumn s_column = get_wall_column(wall_points[1], f_window_sector.floor_z, f_window_sector.ceiling_z);
	
	int tid = window->bottom_tid;
	
	if(f_pos_x > s_pos_x)
	{
		std::swap(f_pos_x, s_pos_x);
		std::swap(f_column, s_column);
	}
	
	std::vector<std::vector<RendererColumn>> columns = get_wall_projection_columns(
																				   wall_points,
																				   f_column,
																				   s_column,
																				   window,
																				   Vector2D<float>(),
																				   f_pos_x,
																				   tid,
																				   floor_visplanes_indeces,
																				   ceiling_visplanes_indeces,
																				   s_pos_x - f_pos_x,
																				   true
																				   );
}

void GameRenderer::render_bottom_window(std::vector<Vector2D<float>> raw_wall_points, std::vector<Vector2D<float>> wall_points, WindowComponent* window)
{
	Sector f_window_sector = level_server->get_sector_by_index(window->f_sector_index);
	Sector s_window_sector = level_server->get_sector_by_index(window->s_sector_index);
	
	int f_visplane_index = get_visplane_index(f_window_sector.floor_z, f_window_sector.floor_color, f_window_sector.floor_tid);
	int s_visplane_index = get_visplane_index(s_window_sector.floor_z, s_window_sector.floor_color, s_window_sector.floor_tid);
	
		//THIS: need to make it another function
	int f_pos_x = get_point_on_camera_projection(wall_points[0]);
	int s_pos_x = get_point_on_camera_projection(wall_points[1]);
	
	RendererColumn f_column = get_wall_column(wall_points[0], f_window_sector.floor_z, s_window_sector.floor_z);
	RendererColumn s_column = get_wall_column(wall_points[1], f_window_sector.floor_z, s_window_sector.floor_z);
	
	int tid = window->bottom_tid;
	
	if(f_pos_x > s_pos_x)
	{
		std::swap(f_pos_x, s_pos_x);
		std::swap(f_column, s_column);
		
		std::swap(raw_wall_points[0], raw_wall_points[1]);
		std::swap(wall_points[0], wall_points[1]);
	}
	
	Vector2D<float> wall_offsets = get_wall_offsets(raw_wall_points, wall_points);
	std::vector<std::vector<RendererColumn>> columns_to_render = get_wall_projection_columns(
																							 wall_points,
																							 f_column,
																							 s_column,
																							 window,
																							 wall_offsets,
																							 f_pos_x,
																							 tid,
																							 {f_visplane_index},
																							 {s_visplane_index},
																							 s_pos_x - f_pos_x,
																							 true
																							 );
	
	render_wall_range(columns_to_render, f_pos_x, tid, Color());
}

void GameRenderer::render_upper_window(std::vector<Vector2D<float>> raw_wall_points, std::vector<Vector2D<float>> wall_points, WindowComponent* window)
{
	Sector f_window_sector = level_server->get_sector_by_index(window->f_sector_index);
	Sector s_window_sector = level_server->get_sector_by_index(window->s_sector_index);
	
	int f_visplane_index = get_visplane_index(f_window_sector.ceiling_z, f_window_sector.ceiling_color, f_window_sector.ceiling_tid);
	int s_visplane_index = get_visplane_index(s_window_sector.ceiling_z, s_window_sector.ceiling_color, s_window_sector.ceiling_tid);
	
		//THIS: need to make it another function
	int f_pos_x = get_point_on_camera_projection(wall_points[0]);
	int s_pos_x = get_point_on_camera_projection(wall_points[1]);
	
	RendererColumn f_column = get_wall_column(wall_points[0], s_window_sector.ceiling_z, f_window_sector.ceiling_z);
	RendererColumn s_column = get_wall_column(wall_points[1], s_window_sector.ceiling_z, f_window_sector.ceiling_z);
	
	int tid = window->upper_tid;
	
	if(f_pos_x > s_pos_x)
	{
		std::swap(f_pos_x, s_pos_x);
		std::swap(f_column, s_column);
		
		std::swap(raw_wall_points[0], raw_wall_points[1]);
		std::swap(wall_points[0], wall_points[1]);
	}
	
	Vector2D<float> wall_offsets = get_wall_offsets(raw_wall_points, wall_points);
	std::vector<std::vector<RendererColumn>> columns_to_render = get_wall_projection_columns(
																							 wall_points,
																							 f_column,
																							 s_column,
																							 window,
																							 wall_offsets,
																							 f_pos_x,
																							 tid,
																							 {s_visplane_index},
																							 {f_visplane_index},
																							 s_pos_x - f_pos_x,
																							 true
																							 );
	
	render_wall_range(columns_to_render, f_pos_x, tid, Color());
}

void GameRenderer::render_wall_range(std::vector<std::vector<RendererColumn>> columns, int f_pos_x, int tid, Color color)
{
	for (size_t column_index = 0; column_index < columns.size(); column_index++)
	{
		std::vector<RendererColumn> ranges = columns[column_index];
		int pos_x = f_pos_x + (int)column_index;
		
		for (RendererColumn range : ranges) render_texture_column(pos_x, range, tid);
	}
}

void GameRenderer::render_horizontal()
{
	Sector camera_sector = level_server->get_sector_by_index(current_camera->sector_index);
	const float camera_z = camera_sector.floor_z + current_camera->height_z;
	
	for(const VisPlane& current_plane : visual_planes)
	{
		render_plane(current_plane, camera_z);
	}
}

void GameRenderer::render_plane(const VisPlane& plane, float global_camera_height)
{
		//FIXME: Method is lagging
	std::vector<RendererColumn> plane_columns = plane.plane_columns;
	if(plane.min_x == -1 || plane.max_x == -1) return;
	
	float height = abs(global_camera_height - plane.height_z);
	
	for(int pos_x = plane.min_x; pos_x <= plane.max_x; pos_x++)
	{
		RendererColumn column = plane_columns[pos_x];
		if(column.top >= column.bottom) continue;
		render_plane_texture_column(pos_x, column, plane.tid, height);
	}
		//FIXME: Method is lagging
}

void GameRenderer::render_color_column(int pos_x, RendererColumn& range, Color color)
{
	int column_x = SDL_min(screen_width - 1, SDL_max(0, pos_x));
	for(int pos_y = range.top; pos_y < range.bottom; pos_y++)
	{
		draw_pixel_in_buffer(Vector2D<int>(column_x, pos_y), color);
	}
}

void GameRenderer::render_texture_column(int pos_x, RendererColumn& range, int tid)
{
	int column_x = SDL_min(screen_width - 1, SDL_max(0, pos_x));
	float v = range.v_top;
	
	for(int pos_y = range.top; pos_y < range.bottom; pos_y++)
	{
		Vector2D<int> texture_point = {range.u, (int)v};
		Color color = texture_buffer->get_texture_pixel(tid, texture_point);
		draw_pixel_in_buffer(Vector2D<int>(column_x, pos_y), color);
		v += range.v_step;
	}
}

void GameRenderer::render_plane_texture_column(int pos_x, RendererColumn& range, int tid, float height)
{
	int column_x = SDL_min(screen_width - 1, SDL_max(0, pos_x));
	
	Vector2D<float> camera_pos = current_camera->position;
	float camera_rotation = current_camera->rotation;
	Vector2D<float> camera_plane = {-sin(camera_rotation), cos(camera_rotation)};
	Vector2D<float> camera_direction = {cos(camera_rotation), sin(camera_rotation)};
	SDL_Surface* texture = texture_buffer->get_texture_surface(tid);
	
	float half_width = screen_width * 0.5f;
	float half_height = screen_height * 0.5f;
	
	float focal_len = current_camera->get_focal_len(screen_width);
	float dx = column_x - half_width;
	float x_dir = dx / focal_len;
	Vector2D<float> world_dir = {camera_direction.x - camera_plane.x * x_dir, camera_direction.y - camera_plane.y * x_dir};
	
	float world_dist;
	Vector2D<float> world_pos;
	Vector2D<int> pixel_pos;
	
	for(int pos_y = range.top; pos_y < range.bottom; pos_y++)
	{
		float dy = pos_y - half_height;
		world_dist = (height * focal_len) / abs(dy);
		world_pos.x = camera_pos.x + world_dist * world_dir.x;
		world_pos.y = camera_pos.y + world_dist * world_dir.y;
		
		pixel_pos.x = (int)(world_pos.x * WORLD_TEXTURE_SCALE) % texture->w;
		pixel_pos.y = (int)(world_pos.y * WORLD_TEXTURE_SCALE) % texture->h;
		
		if (pixel_pos.x < 0) pixel_pos.x += texture->w;
		if (pixel_pos.y < 0) pixel_pos.y += texture->h;
		
		Color pixel = texture_buffer->get_texture_pixel(tid, pixel_pos);
		draw_pixel_in_buffer({column_x, pos_y}, pixel);
	}
}

int GameRenderer::get_point_on_camera_projection(Vector2D<float> point)
{
	Vector2D<float> vector_to_point = point - current_camera->position;
	float relative_vector_rot = vector_to_point.get_vector_rotation() - current_camera->rotation;
	
	const float focal_len = current_camera->get_focal_len(screen_width);
	int x_projection = screen_width / 2 - (focal_len * tan(relative_vector_rot));
	x_projection = SDL_min(screen_width - 1, SDL_max(0, x_projection));
	
	return x_projection;
}

RendererColumn GameRenderer::get_wall_column(Vector2D<float> point, float floor_z, float ceiling_z)
{
	Vector2D<float> vector_to_point = point - current_camera->position;
	const float vector_length = vector_to_point * Vector2D<float>(cos(current_camera->rotation), sin(current_camera->rotation));
	
		//	if(vector_length <= 0.1) return RendererColumn();
	
	Sector camera_sector = level_server->get_sector_by_index(current_camera->sector_index);
	const float camera_z = camera_sector.floor_z + current_camera->height_z;
	const float focal_len = current_camera->get_focal_len(screen_width);
	
	const float floor = floor_z - camera_z;
	const float ceiling = ceiling_z - camera_z;
	
	int bottom = (-floor / vector_length) * focal_len;
	int top = (ceiling / vector_length) * focal_len;
	
	bottom = screen_height / 2 + bottom;
	top = screen_height / 2 - top;
	
	return RendererColumn(bottom, top);
}

std::vector<std::vector<RendererColumn>> GameRenderer::get_wall_projection_columns(
																				   std::vector<Vector2D<float>> wall_points,
																				   RendererColumn f_column,
																				   RendererColumn s_column,
																				   WindowComponent* window,
																				   Vector2D<float> wall_offsets,
																				   int f_screen_pos_x, //FIXME: It's just fields in RenderWall class
																				   int tid,
																				   std::vector<int> floor_pids,
																				   std::vector<int> ceiling_pids,
																				   int x_length,
																				   bool is_outside
																				   )
{
	SDL_Surface* texture = texture_buffer->get_texture_surface(tid);
	std::vector<std::vector<RendererColumn>> wall_columns;
	
	float bottom_columns_delta = s_column.bottom - f_column.bottom;
	float top_columns_delta = s_column.top - f_column.top;
	
	float f_point_z = 1 / (wall_points[0] - current_camera->position).length;
	float s_point_z = 1 / (wall_points[1] - current_camera->position).length;
	float f_offset = f_point_z * wall_offsets.x;
	float s_offset = s_point_z * wall_offsets.y;
	
	float linear_z_step = (s_point_z - f_point_z) / (x_length - 1);
	float linear_offset_step = (s_offset - f_offset) / (x_length - 1);
	
	int texture_h = texture->h;
	float offset = f_offset;
	float z = f_point_z;
	int current_u;
	int pos_x = f_screen_pos_x;
	
	for(int delta_pos_x = 0; delta_pos_x <= x_length; delta_pos_x++)
	{
		pos_x = f_screen_pos_x + delta_pos_x;
		float height_k = float(delta_pos_x) / float(x_length);
		
		int wall_bottom = f_column.bottom + height_k * bottom_columns_delta;
		int wall_top = f_column.top + height_k * top_columns_delta;
		
		current_u = (offset / z) * WORLD_TEXTURE_SCALE;
		z += linear_z_step;
		offset += linear_offset_step;
		
		int screen_wall_bottom = SDL_min(screen_height - 1, SDL_max(0, wall_bottom));
		int screen_wall_top = SDL_min(screen_height - 1, SDL_max(0, wall_top));
		
		float v_step = (float)texture_h / (wall_bottom - wall_top);
		float v_top = (float)(screen_wall_top - wall_top) * v_step;
		
		RendererColumn column = RendererColumn(screen_wall_bottom, screen_wall_top, current_u, v_top, v_step);
		
		if(!is_screen_space_free(pos_x, column))
		{
			wall_columns.push_back(std::vector<RendererColumn>());
			continue;
		}
		
		std::vector<RendererColumn> ranges_to_render = {column};
		if(window == nullptr) ranges_to_render = get_screen_column_ranges(pos_x, column, is_outside);
		
		paste_planes_column(ranges_to_render, pos_x, floor_pids, ceiling_pids);
		
		wall_columns.push_back({column});
	}
	
	return wall_columns;
}

Vector2D<float> GameRenderer::get_wall_offsets(std::vector<Vector2D<float>> raw_wall_points, std::vector<Vector2D<float>> wall_points)
{
	Vector2D<float> f_point_vector = wall_points[0] - raw_wall_points[0];
	Vector2D<float> s_point_vector = wall_points[1] - raw_wall_points[0];
	
	Vector2D<float> offsets = {f_point_vector.length, s_point_vector.length};
	
	return offsets;
}

void GameRenderer::paste_planes_column(std::vector<RendererColumn> column_ranges, int pos_x, std::vector<int> floor_visplanes_id, std::vector<int> ceiling_visplanes_id)
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
		floor_plane.plane_columns[pos_x].bottom = visplanes_clip_buffer[pos_x].bottom;
		floor_plane.plane_columns[pos_x].top = bottom;
		visplanes_clip_buffer[pos_x].bottom = bottom;
	}
	
	if(ceiling_visplanes_id.size() != 0)
	{
		VisPlane& ceiling_plane = visual_planes[ceiling_visplanes_id[0]];
		ceiling_plane.set_x_range(pos_x);
		ceiling_plane.plane_columns[pos_x].bottom = top;
		ceiling_plane.plane_columns[pos_x].top = visplanes_clip_buffer[pos_x].top;
		visplanes_clip_buffer[pos_x].top = top;
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
