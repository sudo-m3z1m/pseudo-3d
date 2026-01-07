#define SDL_MAIN_USE_CALLBACKS 1

#include <stdio.h>

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "physics-server.hpp"
#include "physics-component.hpp"
#include "level-server.hpp"
#include "math.hpp"

//PhysicsServer physics_server;
SDL_Window* main_window;
SDL_Renderer* main_renderer;
LevelServer* level_server;

//PhysicsComponent* circle;

//Vector2D<float> get_input_direction()
//{
//	Vector2D direction = Vector2D<float>();
//	
//	const bool* keys = SDL_GetKeyboardState(NULL);
//	
//	if (keys[SDL_SCANCODE_W])
//		direction.y -= 1;
//	if (keys[SDL_SCANCODE_S])
//		direction.y += 1;
//	if (keys[SDL_SCANCODE_D])
//		direction.x += 1;
//	if (keys[SDL_SCANCODE_A])
//		direction.x -= 1;
//
//	return direction;
//}


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
	
	std::vector<Vector2D<float>> f_shape_points = {Vector2D<float>(1.0f, 1.0f), Vector2D<float>(3.0f, 1.0f), Vector2D<float>(3.0f, 2.0f)};
	std::vector<Vector2D<float>> s_shape_points = {Vector2D<float>(4.0f, 1.0f), Vector2D<float>(6.0f, 1.0f), Vector2D<float>(5.0f, 0.0f)};
	std::vector<Vector2D<float>> t_shape_points = {Vector2D<float>(2.0f, 3.0f), Vector2D<float>(3.0f, 4.0f), Vector2D<float>(4.0f, 3.0f)};
	
	ShapeComponent f_shape = ShapeComponent(POLYGON, 0, f_shape_points);
	ShapeComponent s_shape = ShapeComponent(POLYGON, 0, s_shape_points);
	ShapeComponent t_shape = ShapeComponent(POLYGON, 0, t_shape_points);
	
	level_server->add_new_polygon(f_shape);
	level_server->add_new_polygon(s_shape);
	level_server->add_new_polygon(t_shape);
	
	level_server->create_bsp_tree();
	
	return;
}

SDL_AppResult SDL_AppInit(void** appstate, int argc, char** argv)
{
	SDL_CreateWindowAndRenderer("PSEUDO-NAME", 640, 640, SDL_WINDOW_RESIZABLE, &main_window, &main_renderer);
	
	create_level_server();
	
//	physics_server = PhysicsServer();
//	create_physics_components();
	
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event)
{
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate)
{
//	Vector2D<float> direction = get_input_direction();
//	circle->position = circle->position + (direction * 0.1);
	
//	physics_server.calculate_sector_colliding();
	
	SDL_SetRenderScale(main_renderer, 4, 4);
	SDL_SetRenderDrawColor(main_renderer, 0, 0, 0, 255);
	SDL_RenderClear(main_renderer);
	
//	physics_server.render_physics_components(main_renderer);
//	physics_server.render_physics_components(main_renderer);
	SDL_RenderPresent(main_renderer);
	
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
