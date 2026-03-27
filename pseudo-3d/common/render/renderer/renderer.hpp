#ifndef RENDERER_HPP
#define RENDERER_HPP
	
#include <stdio.h>
#include <SDL3/SDL.h>

#include "color.hpp"
#include "camera.hpp"
#include "level-server.hpp"
#include "visplane.hpp"
#include "texture-buffer.hpp"

//TODO: Need to sort functions and make pipeline structure

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
	TextureBuffer* texture_buffer;
	
	int screen_width, screen_height;
	
public:
	Renderer();
	Renderer(Camera* camera, LevelServer* level_server, TextureBuffer* texture_buffer, int width, int height);
	~Renderer();
	
	float get_delta_ticks();
	
	int get_visplane_index(float height_z, Color color);
	
	bool is_screen_space_free(int x_point, RendererColumn new_column);
	std::vector<RendererColumn> get_screen_column_ranges(int x_point, RendererColumn new_column, bool is_outside);
	
	int get_point_on_camera_projection(Vector2D<float> point);
	RendererColumn get_wall_column(Vector2D<float> point, float floor_z, float ceiling_z);
	std::vector<std::vector<RendererColumn>> get_wall_projection_columns(
		RendererColumn f_column,
		RendererColumn s_column,
		Vector2D<float> wall_offsets,
		int f_screen_pos_x,
		int tid,
		std::vector<int> floor_pids,
		std::vector<int> ceiling_pids,
		int x_length,
		bool is_outside
	);
	Vector2D<float> get_wall_offsets(std::vector<Vector2D<float>> raw_wall_points, std::vector<Vector2D<float>> wall_points);
	
	void paste_planes_column(std::vector<RendererColumn> column_ranges, int pos_x, std::vector<int> floor_visplanes_id, std::vector<int> ceiling_visplanes_id);
	void render();
	void clear_screen_width_buffer();
	void render_node(BSPNode* node);
	void render_bsp_shape(BSPNode* node);
	
	void render_shape_wall(BSPShape* shape, Wall wall);
	void render_wall_range(std::vector<std::vector<RendererColumn>> columns, int f_pos_x, int tid, Color color); //FIXME: is_outside is stupid sheet. Need to remade this. Need to make render_wall struct neither
	
//	void render_screen_wall(std::vector<Vector2D<float>> wall_points, Wall wall, int sector_index);
	void render_window(WindowComponent* window, std::vector<Vector2D<float>> raw_wall_points, std::vector<Vector2D<float>> wall_points);
	void render_bottom_window(std::vector<Vector2D<float>> raw_wall_points, std::vector<Vector2D<float>> wall_points, WindowComponent* window);
	void render_upper_window(std::vector<Vector2D<float>> raw_wall_points, std::vector<Vector2D<float>> wall_points, WindowComponent* window);
	void render_horizontal();
	void render_plane(const VisPlane& plane);
	void render_color_column(int pos_x, RendererColumn& range, Color color);
	void render_texture_column(int pos_x, RendererColumn& range, int tid);
	
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
