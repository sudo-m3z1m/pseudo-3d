#define SDL_MAIN_USE_CALLBACKS 1

#include <stdio.h>
#include <iostream>
#include <ctime>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

#include "physics-server.hpp"
#include "physics-component.hpp"
#include "level-server.hpp"
#include "camera.hpp"
#include "game-renderer.hpp"
#include "math.hpp"

//PhysicsServer physics_server;
LevelServer* level_server;
GameRenderer* renderer;
Camera* camera;

//PhysicsComponent* circle;

Vector2D<float> get_input_direction()
{
	Vector2D direction = Vector2D<float>();
	
	const bool* keys = SDL_GetKeyboardState(NULL);
	
	if (keys[SDL_SCANCODE_W])
		direction.x += 1;
	if (keys[SDL_SCANCODE_S])
		direction.x -= 1;
	if (keys[SDL_SCANCODE_D])
		direction.y -= 1;
	if (keys[SDL_SCANCODE_A])
		direction.y += 1;
	
	direction.length = direction.calculate_vector_length();
	return direction;
}

float get_input_rotation()
{
	float rotation = 0;
	const bool* keys = SDL_GetKeyboardState(NULL);
	
	if (keys[SDL_SCANCODE_RIGHT])
		rotation -= 1;
	if (keys[SDL_SCANCODE_LEFT])
		rotation += 1;
	
	return rotation;
}


//void create_physics_components()
//{
//	std::vector<Vector2D<float>> circles_points = {Vector2D<float>(0.0f, 0.0f)};
//	std::vector<Vector2D<float>> polygon_points = {Vector2D<float>(-30.0f, -30.0f), Vector2D<float>(89.0f, -42.0f), Vector2D<float>(106.0f, 56.0f), Vector2D<float>(28.0f, 83.0f), Vector2D<float>(-35.0f, 54.0f)};
//	std::vector<Vector2D<float>> line_points = {Vector2D<float>(0.0f, 0.0f), Vector2D<float>(5.0f, -52.0f)};
//	
//	ShapeComponent circle_shape = ShapeComponent(CIRCLE, 10.0f, circles_points);
//	ShapeComponent s_circle_shape = ShapeComponent(CIRCLE, 15.0f, circles_points);
//	ShapeComponent polygon_shape = ShapeComponent(POLYGON, 0, polygon_points);
//	ShapeComponent line_shape = ShapeComponent(LINE, 0, line_points);
//	
//	circle = new PhysicsComponent(circle_shape, Vector2D<float>(83.0f, 73.0f));
//	PhysicsComponent* s_circle = new PhysicsComponent(s_circle_shape, Vector2D<float>(130.0f, 90.0f));
//	PhysicsComponent* polygon = new PhysicsComponent(polygon_shape, Vector2D<float>(50.0f, 62.0f));
//	PhysicsComponent* line = new PhysicsComponent(line_shape, Vector2D<float>(78.0f, 145.0f));
//	
//	physics_server.add_physics_component(circle);
//	physics_server.add_physics_component(s_circle);
//	physics_server.add_physics_component(polygon);
//	physics_server.add_physics_component(line);
//}

void create_level_server()
{
	level_server = new LevelServer();
	
	Sector f_test_sector = Sector(0.0f, 6.0f, 1, 1, Color(), Color());
	Sector s_test_sector = Sector(-1.0f, 7.0f, 0, 0, Color(), Color());
	Sector t_test_sector = Sector(1.5f, 4.5f, 0, 1, Color(), Color());
	
	level_server->add_new_sector(f_test_sector);
	level_server->add_new_sector(s_test_sector);
	level_server->add_new_sector(t_test_sector);
	
	std::vector<Vector2D<float>> f_shape_points = {Vector2D<float>(6.0f, 7.0f), Vector2D<float>(6.0f, -7.0f), Vector2D<float>(16.0f, -7.0f), Vector2D<float>(16.0f, 7.0f)};
	std::vector<Vector2D<float>> s_shape_points = {Vector2D<float>(-4.0f, 7.0f), Vector2D<float>(-4.0f, 2.0f), Vector2D<float>(-4.0f, -2.0f), Vector2D<float>(-4.0f, -7.0f), Vector2D<float>(6.0f, -7.0f), Vector2D<float>(6.0f, 7.0f)};
	std::vector<Vector2D<float>> t_shape_points = {Vector2D<float>(6.0f, -0.5f), Vector2D<float>(6.0f, 0.5f), Vector2D<float>(11.0f, 0.5f), Vector2D<float>(11.0f, -0.5f)};
	std::vector<Vector2D<float>> fo_shape_points = {Vector2D<float>(-4.0f, 2.0f), Vector2D<float>(-14.0f, 2.0f), Vector2D<float>(-14.0f, -2.0f), Vector2D<float>(-4.0f, -2.0f)};
	
//	std::vector<Vector2D<float>> f_shape_points = {Vector2D<float>(7.0f, -15.0f), Vector2D<float>(16.0f, 15.0f), Vector2D<float>(17.0f, 5.0f), Vector2D<float>(17.0f, -5.0f)};
//	std::vector<Vector2D<float>> s_shape_points = {Vector2D<float>(4.0f, 1.0f), Vector2D<float>(6.0f, 1.0f), Vector2D<float>(5.0f, 0.0f)};
//	std::vector<Vector2D<float>> t_shape_points = {Vector2D<float>(2.0f, 3.0f), Vector2D<float>(3.0f, 4.0f), Vector2D<float>(4.0f, 3.0f)};
	
	WindowComponent* f_window = new WindowComponent(0, 1);
	WindowComponent* f_s_window = new WindowComponent(1, 0);
	WindowComponent* s_window = new WindowComponent(0, 2);
	WindowComponent* t_window = new WindowComponent(2, 0);
	
	std::vector<Wall> f_shape_walls = {Wall(0, 1, Vector2D<float>(1.0f, 0.0f), f_s_window), Wall(1, 2, Vector2D<float>(0.0f, 1.0f), nullptr), Wall(2, 3, Vector2D<float>(-1.0f, 0.0f), nullptr), Wall(3, 0, Vector2D<float>(0.0f, -1.0f), nullptr)};
	std::vector<Wall> s_shape_walls = {Wall(0, 1, Vector2D<float>(1.0f, 0.0f), nullptr), Wall(1, 2, Vector2D<float>(1.0f, 0.0f), s_window), Wall(2, 3, Vector2D<float>(1.0f, 0.0f), nullptr), Wall(3, 4, Vector2D<float>(0.0f, 1.0f), nullptr), Wall(4, 5, Vector2D<float>(-1.0f, 0.0f), f_window), Wall(5, 0, Vector2D<float>(0.0f, -1.0f), nullptr)};
	std::vector<Wall> fo_shape_walls = {Wall(0, 1, Vector2D<float>(0.0f, -1.0f), nullptr), Wall(1, 2, Vector2D<float>(1.0f, 0.0f), nullptr), Wall(2, 3, Vector2D<float>(0.0f, 1.0f), nullptr), Wall(3, 0, Vector2D<float>(-1.0f, 0.0f), t_window)};
	
	ShapeComponent f_shape = ShapeComponent(POLYGON, 0, f_shape_points, f_shape_walls, 1);
	ShapeComponent s_shape = ShapeComponent(POLYGON, 0, s_shape_points, s_shape_walls, 0);
	ShapeComponent t_shape = ShapeComponent(POLYGON, 0, t_shape_points, std::vector<Wall>(), 1);
	ShapeComponent fo_shape = ShapeComponent(POLYGON, 0, fo_shape_points, fo_shape_walls, 2);
	
//	level_server->add_new_polygon(t_shape);
	level_server->add_new_polygon(f_shape);
	level_server->add_new_polygon(s_shape);
	level_server->add_new_polygon(fo_shape);
	
	level_server->create_bsp_tree();
	
	return;
}

SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv)
{
	std::time_t time = std::time(nullptr);
	std::tm* l_time = std::localtime(&time);
	srand(l_time->tm_sec);
	create_level_server();
	
	Vector2D<float> camera_position = Vector2D<float>(0.0f, 0.0f);
	int camera_sector_index = level_server->get_sector_index_by_point(level_server->bsp_tree, camera_position);
	camera = new Camera((PI / 3), Vector2D<float>(0.0f, 0.0f), 0, 2, camera_sector_index);
	
	TextureBuffer* texture_buffer = new TextureBuffer();
	
	const char* base_path = SDL_GetBasePath();
	char path[512];
	
	snprintf(path, 512, "%s%s", base_path, "texture.bmp");
	texture_buffer->load_texture(path);
	snprintf(path, 512, "%s%s", base_path, "floor.bmp");
	texture_buffer->load_texture(path);
	
	renderer = new GameRenderer(camera, texture_buffer, DEFAULT_SCREEN_WIDTH, DEFAULT_SCREEN_HEIGHT, level_server);
	
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
//	circle->position = circle->position + (direction * 0.1);
	
//	physics_server.calculate_sector_colliding();
//	const float delta = renderer->get_delta_ticks();
//	camera->set_camera_rotation(camera->rotation - (0.1 * delta));

//	std::cout << "Position: " << camera->position.x << ',' << camera->position.y << std::endl;
//	std::cout << "Rotation: " << camera->rotation << std::endl;
	
	float delta = renderer->get_delta_ticks();
	Vector2D<float> direction = get_input_direction();
	float rotation_direction = get_input_rotation();
	
	float rotation_speed = 1;
	float move_speed = 5;
	
	float new_rotation = (rotation_direction * rotation_speed) * delta;
	
	camera->set_camera_rotation(camera->rotation + new_rotation);
	
	Vector2D<float> velocity = (direction.rotate_vector(camera->rotation).normalize_vector_2d() * move_speed) * delta;
	
	camera->set_camera_position(camera->position + velocity);
	
	camera->sector_index = level_server->get_sector_index_by_point(level_server->bsp_tree, camera->position);
	
	renderer->render();
	
	return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result)
{
	
}


//#include "../player/player.h"
//#include "../player/raycast.h"
//#include "../core/renderer.h"

//Player* player = NULL;
//Renderer* renderer = NULL;

//SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv)
//{	
//	player = initialize_player(2, 2, (Vector2D){4, 4}, 0);
//	renderer = initialize_renderer(INIT_WIN_WIDTH, INIT_WIN_HEIGHT);
//
//	return SDL_APP_CONTINUE;
//}
//
//SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
//{
//	return SDL_APP_CONTINUE;
//}
//
//SDL_AppResult SDL_AppIterate(void* appstate)
//{
//	SDL_Renderer* sdl_renderer = renderer->main_renderer;
//	SDL_SetRenderDrawColor(sdl_renderer, 0, 0, 0, 255);
//	SDL_RenderClear(sdl_renderer);
//	SDL_ClearSurface(renderer->color_buffer, 0, 0, 0, 255);
//	SDL_LockSurface(renderer->color_buffer);
//	
//	const float delta = get_delta_ticks();
//	
//	player->collision_component->update(player);
//	player->updatable_component->update(delta, player);
//	
//	draw_3d(*renderer, *player);
//	SDL_UnlockSurface(renderer->color_buffer);
//	
//	render_buffer(*renderer);
//	
//	SDL_RenderPresent(sdl_renderer);
//	
//	return SDL_APP_CONTINUE;
//}
//
//void SDL_AppQuit(void* appstate, SDL_AppResult result)
//{
//	SDL_DestroySurface(renderer->color_buffer);
//	SDL_DestroySurface(renderer->textures_buffer[0]); //Need to remove it dynamicly
//}
