#ifndef GAME_RENDERER_HPP
#define GAME_RENDERER_HPP

#include <stdio.h>

#include "renderer.hpp"
#include "bsp-level-server.hpp"
#include "visplane.hpp"

//TODO: Need to make pipeline structure

class GameRenderer : public Renderer {
protected:
	BSPLevelServer* level_server;
	std::vector<std::vector<RendererColumn>> screen_width_buffer;
	std::vector<VisPlane> visual_planes;
	
public:
	GameRenderer();
	GameRenderer(Camera* camera, TextureBuffer* texture_buffer, FileServer* file_server, int width, int height, BSPLevelServer* level_server);
	~GameRenderer() override;
	
	int get_visplane_index(float height_z, Color color, int tid);
	void clear_screen_width_buffer();
	bool is_screen_space_free(int x_point, RendererColumn new_column);
	std::vector<RendererColumn> get_screen_column_ranges(int x_point, RendererColumn new_column, bool is_outside);
	
	void render() override;
	void render_node(BSPNode* node);
	void render_bsp_shape(BSPNode* node);
	void render_shape_wall(BSPShape* shape, Wall wall);
	void render_window(WindowComponent* window, std::vector<Vector2D<float>> raw_wall_points, std::vector<Vector2D<float>> wall_points);
	void render_bottom_window(std::vector<Vector2D<float>> raw_wall_points, std::vector<Vector2D<float>> wall_points, WindowComponent* window);
	void render_upper_window(std::vector<Vector2D<float>> raw_wall_points, std::vector<Vector2D<float>> wall_points, WindowComponent* window);
	void render_wall_range(std::vector<std::vector<RendererColumn>> columns, int f_pos_x, int tid, Color color);
	
	void render_horizontal();
	void render_plane(const VisPlane& plane, float global_camera_height);
	
	void render_color_column(int pos_x, RendererColumn& range, Color color);
	void render_texture_column(int pos_x, RendererColumn& range, int tid);
	void render_plane_texture_column(int pos_x, RendererColumn& range, int tid, float height);
	
	int get_point_on_camera_projection(Vector2D<float> point);
	RendererColumn get_wall_column(Vector2D<float> point, float floor_z, float ceiling_z);
	//FIXME: is_outside is stupid shit. Need to remade this. Need to make render_wall struct too
	std::vector<std::vector<RendererColumn>> get_wall_projection_columns(
		std::vector<Vector2D<float>> wall_points,
		RendererColumn f_column,
		RendererColumn s_column,
		WindowComponent* window,
		Vector2D<float> wall_offsets,
		int f_screen_pos_x,
		int tid,
		std::vector<int> floor_pids,
		std::vector<int> ceiling_pids,
		int x_length,
		bool is_outside
	);
	//FIXME: is_outside is stupid shit. Need to remade this. Need to make render_wall struct too
	
	Vector2D<float> get_wall_offsets(std::vector<Vector2D<float>> raw_wall_points, std::vector<Vector2D<float>> wall_points);
	void paste_planes_column(std::vector<RendererColumn> column_ranges, int pos_x, std::vector<int> floor_visplanes_id, std::vector<int> ceiling_visplanes_id);
};

#endif
