#ifndef RENDERER_HPP
#define RENDERER_HPP
	
#include <stdio.h>
#include <SDL3/SDL.h>

#include "color.hpp"
#include "camera.hpp"
#include "level-server.hpp"

struct ScreenRange
{
	int f_point, s_point;
};

class Renderer
{
private:
	float delta, prev_ticks;
	Camera* current_camera;
	LevelServer* level_server;
	SDL_Renderer* application_renderer;
	SDL_Window* application_window;
	
	SDL_Surface* color_buffer;
	std::vector<ScreenRange> screen_width_buffer;
	std::vector<SDL_Surface*> textures_buffer;
	
	int screen_width, screen_height;
	
public:
	Renderer();
	Renderer(Camera* camera, LevelServer* level_server, int width, int height);
	~Renderer();
	
	float get_delta_ticks();
	ScreenRange is_screen_space_free(ScreenRange new_range);
	int get_point_on_camera_projection(Vector2D<float> point);
	int get_wall_height(Vector2D<float> point);
	
	void render();
	void clear_screen_width_buffer();
	void render_node(BSPNode* node);
	void render_bsp_shape(BSPNode* node);
	void render_wall(std::vector<Vector2D<float>> wall_points, Color color);
	void render_horizontal();
	void render_column(int pos_x, int height, Color color);
	
	void draw_pixel_in_buffer(Vector2D<int> draw_pos, Color color);
	void render_buffer();
};

	//typedef struct Renderer
	//{
	//	SDL_Renderer* main_renderer;
	//	SDL_Surface* textures_buffer[32];
	//	SDL_Window* main_window;
	//	int width, height;
	//
	//	SDL_Surface* color_buffer;
	//} Renderer;
	//
	//Renderer* initialize_renderer(int width, int height);
	//
	//void render_buffer(Renderer renderer);
	//void draw_pixel_in_buffer(Renderer renderer, int x, int y, Color color);
	//void draw_3d(Renderer renderer, Player player);
	//void draw_horizontal_surfaces_3d(Renderer renderer, Player player);
	//
	//void draw_texture_column(Renderer renderer,
	//						 Vector2D draw_position,
	//						 float wall_height,
	//						 float texture_delta,
	//						 int texture_index);
	//
	//static void draw_texture_row(Renderer renderer,
	//							 Player player,
	//							 SDL_Surface* texture,
	//							 float distance,
	//							 int screen_delta);
	//
	#endif
