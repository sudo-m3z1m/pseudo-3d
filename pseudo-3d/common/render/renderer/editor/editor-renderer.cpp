#include "editor-renderer.hpp"

EditorRenderer::EditorRenderer() : Renderer()
{
	grid_step = 1.0f;
	grid_low_step = 0.1f;
}

EditorRenderer::EditorRenderer(Camera* camera, TextureBuffer* texture_buffer, int width, int height, LevelServer* level_server) :
	Renderer(camera, texture_buffer, width, height)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL3_InitForSDLRenderer(application_window, application_renderer);
	ImGui_ImplSDLRenderer3_Init(application_renderer);
	
	grid_step = 1.0f;
	grid_low_step = 0.1f;
	this->level_server = level_server;
}

EditorRenderer::~EditorRenderer()
{
	
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

void EditorRenderer::render()
{
	SDL_SetRenderDrawColor(application_renderer, 20, 20, 20, 255);
	SDL_RenderClear(application_renderer);

//	ImGui_ImplSDLRenderer3_NewFrame();
//	ImGui_ImplSDL3_NewFrame();
//	ImGui::NewFrame();
	
//	render_ui();
	render_grid();
	render_level_data();
	
//	ImGui::Render();
//	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), application_renderer);
	SDL_RenderPresent(application_renderer);
}

void EditorRenderer::render_ui()
{

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
		SDL_SetRenderDrawColor(application_renderer, 40, 40, 40, 255);
		if(fmodf(fabsf(world_x) + 0.001f, grid_step) < grid_low_step)
			SDL_SetRenderDrawColor(application_renderer, 80, 80, 80, 255);
		else if (!render_low_step) continue;
		
		Vector2D<int> screen_pos = get_screen_pos({world_x, 0});
		SDL_RenderLine(application_renderer, screen_pos.x, 0, screen_pos.x, screen_height);
	}
	for(float world_y = world_left_up_corner.y; world_y > world_right_down_corner.y; world_y -= grid_low_step)
	{
		SDL_SetRenderDrawColor(application_renderer, 40, 40, 40, 255);
		if(fmodf(fabsf(world_y) + 0.001f, grid_step) < grid_low_step)
			SDL_SetRenderDrawColor(application_renderer, 80, 80, 80, 255);
		else if (!render_low_step) continue;
		
		Vector2D<int> screen_pos = get_screen_pos({0, world_y});
		SDL_RenderLine(application_renderer, 0, screen_pos.y, screen_width, screen_pos.y);
	}
}

void EditorRenderer::render_level_data()
{
	
}
