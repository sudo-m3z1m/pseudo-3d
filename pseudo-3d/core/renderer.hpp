#ifndef RENDERER_HPP
#define RENDERER_HPP
	
#include <stdio.h>
#include <SDL3/SDL.h>

#include "color.hpp"
#include "camera.hpp"
#include "level-server.hpp"
#include "visplane.hpp"

//Refactoring goals:
//TODO: Remade rendering sectors diffs function
//TODO: Remade screen_width_buffer ranges getting.
//TODO: Bring RendererColumns data using to global coordinates.
//TODO: Remade visplanes generating and rendering. It concerns rendering diffs function.
//TODO: Remade way to store and getting visplanes data
//TODO: Remade way to store and getting sectors datax
//TODO: Fix columns clamping and lagging
//TODO: Remade function for getting angle of vector

class Renderer
{
private:
	float delta, prev_ticks;
	Camera* current_camera;
	LevelServer* level_server;
	SDL_Renderer* application_renderer;
	SDL_Window* application_window;
	
	SDL_Surface* color_buffer;
	std::vector<std::vector<RendererColumn>> screen_width_buffer;
	std::vector<VisPlane> visual_planes;
//	std::vector<SDL_Surface*> textures_buffer;
	
	int screen_width, screen_height;
	
public:
	Renderer();
	Renderer(Camera* camera, LevelServer* level_server, int width, int height);
	~Renderer();
	
	float get_delta_ticks();
	
	int get_visplane_index(float height_z, Color color);
	
	bool is_screen_space_free(int x_point, RendererColumn new_column);
	std::vector<RendererColumn> get_screen_column_ranges(int x_point, RendererColumn new_column, bool is_outside);
	
	int get_point_on_camera_projection(Vector2D<float> point);
	RendererColumn get_wall_column(Vector2D<float> point, float floor_z, float ceiling_z);
	std::vector<RendererColumn> get_wall_projection_columns(
		RendererColumn f_column,
		RendererColumn s_column,
		int x_length
	);
	std::vector<RendererColumn> get_wall_projection(std::vector<Vector2D<float>>& wall_points, int floor_z, int ceiling_z);
	
	void paste_planes_column(std::vector<RendererColumn> column_ranges, int pos_x, std::vector<int> floor_visplanes_id, std::vector<int> ceiling_visplanes_id);
	void render();
	void clear_screen_width_buffer();
	void render_node(BSPNode* node);
	void render_bsp_shape(BSPNode* node);
	
	void render_shape_wall(BSPShape* shape, Wall wall);
	void render_wall_range(std::vector<RendererColumn> columns, int f_pos_x, int texture_index, Color color, int bottom_plane_id, int top_plane_id, bool is_outside); //FIXME: is_outside is stupid sheet. Need to remade this
	
//	void render_screen_wall(std::vector<Vector2D<float>> wall_points, Wall wall, int sector_index);
	void render_window(WindowComponent* window, std::vector<Vector2D<float>> wall_points);
	void render_bottom_window(std::vector<Vector2D<float>> wall_points, WindowComponent* window);
	void render_upper_window(std::vector<Vector2D<float>> wall_points, WindowComponent* window);
	void render_horizontal();
	void render_plane(const VisPlane& plane);
	void render_column(int pos_x, RendererColumn& range, Color color);
	
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
