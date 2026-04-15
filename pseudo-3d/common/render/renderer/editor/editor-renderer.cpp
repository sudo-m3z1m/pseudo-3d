#include "editor-renderer.hpp"

EditorRenderer::EditorRenderer() : Renderer()
{
	grid_step = 1.0f;
	grid_low_step = 0.1f;
	level_server = nullptr;
	imgui_viewport = ImGui::GetMainViewport();
	min_zoom = EDITOR_DEFAULT_MIN_ZOOM;
	max_zoom = EDITOR_DEFAULT_MAX_ZOOM;
}

EditorRenderer::EditorRenderer(Camera* camera, TextureBuffer* texture_buffer, int width, int height, EditorLevelServer* level_server, float min_zoom, float max_zoom) :
	Renderer(camera, texture_buffer, width, height)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL3_InitForSDLRenderer(application_window, application_renderer);
	ImGui_ImplSDLRenderer3_Init(application_renderer);
	
	grid_step = 1.0f;
	grid_low_step = 0.1f;
	this->level_server = level_server;
	imgui_viewport = ImGui::GetMainViewport();
	
	this->min_zoom = min_zoom;
	this->max_zoom = max_zoom;
}

EditorRenderer::~EditorRenderer()
{
	delete level_server;
}

Vector2D<int> EditorRenderer::get_screen_pos(Vector2D<float> world_pos)
{
	int width_half = screen_width / 2, height_half = screen_height / 2;
	Vector2D<float> pos_vector = world_pos - current_camera->position;
	pos_vector = pos_vector * current_camera->field_of_view;
	
	Vector2D<int> screen_pos = {width_half + (int)pos_vector.x, height_half - (int)pos_vector.y};
	return screen_pos;
}

Vector2D<float> EditorRenderer::get_world_pos(Vector2D<int> screen_pos)
{
	int width_half = screen_width / 2, height_half = screen_height / 2;
	Vector2D<float> pos_vector = {(float)screen_pos.x - width_half, -((float)screen_pos.y - height_half)};
	pos_vector = pos_vector / current_camera->field_of_view;
	
	Vector2D<float> world_pos = current_camera->position + pos_vector;
	return world_pos;
}

Vector2D<float> EditorRenderer::get_mouse_pos()
{
	ImGuiIO& imgui_io = ImGui::GetIO();
	Vector2D<float> mouse_pos = {imgui_io.MousePos.x, imgui_io.MousePos.y};
	
	return mouse_pos;
}

void EditorRenderer::render()
{
	SDL_SetRenderDrawColor(application_renderer, 20, 20, 20, 255);
	SDL_RenderClear(application_renderer);

	ImGui_ImplSDLRenderer3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();
	
	if(ImGui::IsMouseClicked(ImGuiMouseButton_Left))
	{
		Vector2D<float> mouse_pos = get_mouse_pos();
		Vector2D<float> new_shape_pos = get_world_pos({(int)mouse_pos.x, (int)mouse_pos.y});
		new_shape_pos.x = round(new_shape_pos.x * 10) / 10;
		new_shape_pos.y = round(new_shape_pos.y * 10) / 10;
		level_server->add_point_to_current_shape(new_shape_pos);
	}
	
	render_ui();
	render_grid();
	render_level_data();
	
	ImGui::Render();
	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), application_renderer);
	SDL_RenderPresent(application_renderer);
}

void EditorRenderer::render_ui()
{
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse |
	ImGuiWindowFlags_NoResize |
	ImGuiWindowFlags_NoMove |
	ImGuiWindowFlags_NoTitleBar |
	ImGuiWindowFlags_NoBringToFrontOnFocus |
	ImGuiWindowFlags_NoScrollbar |
	ImGuiWindowFlags_NoScrollWithMouse;
	
	ImGui::BeginMainMenuBar();
	if(ImGui::BeginMenu("File"))
	{
		if(ImGui::MenuItem("Save .map"))
		{
			std::cout << "Map Saved" << std::endl;
		}
		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();
	
	ImGui::SetNextWindowPos(imgui_viewport->WorkPos);
	
	ImGui::Begin("Editor tools", NULL, window_flags);
	if(ImGui::Button("Create new point"))
	{
		std::cout << "Added point to the map" << std::endl;
	}
	ImGui::SameLine();
	
	if(ImGui::Button("Create new wall"))
	{
		std::cout << "Started wall creation" << std::endl;
	}
	ImGui::SameLine();
	
	if(ImGui::Button("Create new shape"))
	{
		level_server->create_new_shape();
	}
	ImGui::SameLine();
	
	if(ImGui::Button("Mark as room"))
	{
		std::cout << "Started room marking" << std::endl;
	}
	ImGui::SameLine();
	ImGui::End();
}

void EditorRenderer::render_grid()
{
	bool render_low_step = current_camera->field_of_view > 50.0f;
	Vector2D<float> world_left_up_corner = get_world_pos({0, 0});
	Vector2D<float> world_right_down_corner = get_world_pos({screen_width, screen_height});
	
	world_left_up_corner.x = floorf(world_left_up_corner.x / grid_low_step) * grid_low_step;
	world_left_up_corner.y = floorf(world_left_up_corner.y / grid_low_step) * grid_low_step;
	
	for(float world_x = world_left_up_corner.x; world_x < world_right_down_corner.x; world_x += grid_low_step)
	{
		SDL_SetRenderDrawColor(
			application_renderer,
			Color::GRID_SECONDARY_COLOR.r,
			Color::GRID_SECONDARY_COLOR.g,
			Color::GRID_SECONDARY_COLOR.b,
			Color::GRID_SECONDARY_COLOR.a
		);
		
		if(fmodf(fabsf(world_x) + 0.001f, grid_step) < grid_low_step)
		{
			SDL_SetRenderDrawColor(
				application_renderer,
				Color::GRID_MAIN_COLOR.r,
				Color::GRID_MAIN_COLOR.g,
				Color::GRID_MAIN_COLOR.b,
				Color::GRID_MAIN_COLOR.a
			);
		}
		else if (!render_low_step) continue;
		
		Vector2D<int> screen_pos = get_screen_pos({world_x, 0});
		SDL_RenderLine(application_renderer, screen_pos.x, 0, screen_pos.x, screen_height);
	}
	for(float world_y = world_left_up_corner.y; world_y > world_right_down_corner.y; world_y -= grid_low_step)
	{
		SDL_SetRenderDrawColor(
			application_renderer,
			Color::GRID_SECONDARY_COLOR.r,
			Color::GRID_SECONDARY_COLOR.g,
			Color::GRID_SECONDARY_COLOR.b,
			Color::GRID_SECONDARY_COLOR.a
		);
		
		if(fmodf(fabsf(world_y) + 0.001f, grid_step) < grid_low_step)
		{
			SDL_SetRenderDrawColor(
				application_renderer,
				Color::GRID_MAIN_COLOR.r,
				Color::GRID_MAIN_COLOR.g,
				Color::GRID_MAIN_COLOR.b,
				Color::GRID_MAIN_COLOR.a
			);
		}
		else if (!render_low_step) continue;
		
		Vector2D<int> screen_pos = get_screen_pos({0, world_y});
		SDL_RenderLine(application_renderer, 0, screen_pos.y, screen_width, screen_pos.y);
	}
}

void EditorRenderer::render_level_data()
{
	render_shapes();
}

void EditorRenderer::render_shapes()
{
	std::vector<ShapeComponent> level_shapes = level_server->get_levels_shapes();
	
	for (ShapeComponent& shape : level_shapes)
	{
		SDL_SetRenderDrawColor(
			application_renderer,
			Color::VERTEX_COLOR.r,
			Color::VERTEX_COLOR.g,
			Color::VERTEX_COLOR.b,
			Color::VERTEX_COLOR.a
		);
		for(Vector2D<float>& point : shape.points)
		{
			Vector2D<int> screen_space_point = get_screen_pos(point);
			SDL_FRect rect_point = SDL_FRect();
			rect_point.x = screen_space_point.x - VERTEX_SCREEN_SIZE / 2;
			rect_point.y = screen_space_point.y - VERTEX_SCREEN_SIZE / 2;
			rect_point.h = VERTEX_SCREEN_SIZE;
			rect_point.w = VERTEX_SCREEN_SIZE;
			
			SDL_RenderFillRect(application_renderer, &rect_point);
		}
		
		for (Wall& wall : shape.walls)
		{
			SDL_SetRenderDrawColor(application_renderer, Color::LINE_COLOR.r, Color::LINE_COLOR.g, Color::LINE_COLOR.b, Color::LINE_COLOR.a);
			if(wall.window_component)
			{
				SDL_SetRenderDrawColor(
					application_renderer,
					Color::LINE_WINDOW_COLOR.r,
					Color::LINE_WINDOW_COLOR.g,
					Color::LINE_WINDOW_COLOR.b,
					Color::LINE_WINDOW_COLOR.a
				);
			}
			
			std::vector<Vector2D<float>> world_points = wall.get_wall_points(shape.points);
			Vector2D<int> f_screen_point = get_screen_pos(world_points[0]);
			Vector2D<int> s_screen_point = get_screen_pos(world_points[1]);
			
			SDL_RenderLine(application_renderer, f_screen_point.x, f_screen_point.y, s_screen_point.x, s_screen_point.y);
			render_line_normal({f_screen_point, s_screen_point}, wall.normal);
		}
	}
}

void EditorRenderer::render_line_normal(std::vector<Vector2D<int>> line_screen_points, Vector2D<float> normal)
{
	Vector2D<int> f_point = line_screen_points[0] + ((line_screen_points[1] - line_screen_points[0]) / 2);
	Vector2D<int> s_point = f_point;
	Vector2D<float> s_point_vector = normal * current_camera->field_of_view;
	s_point.x += (int)s_point_vector.x;
	s_point.y -= (int)s_point_vector.y;
	
	SDL_RenderLine(application_renderer, f_point.x, f_point.y, s_point.x, s_point.y);
}
