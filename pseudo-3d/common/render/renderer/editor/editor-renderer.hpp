#ifndef EDITOR_RENDERER_HPP
#define EDITOR_RENDERER_HPP

#include <stdio.h>
#include <vector>

#include "editor-level-server.hpp"
#include "renderer.hpp"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

enum Sizes
{
	VERTEX_SCREEN_SIZE = 8
};


class EditorRenderer : public Renderer
{
protected:
	float min_zoom, max_zoom;
	float grid_step, grid_low_step;
	EditorLevelServer* level_server;
	ImGuiViewport* imgui_viewport;
	
public:
	EditorRenderer();
	EditorRenderer(Camera* camera, TextureBuffer* texture_buffer, int width, int height, EditorLevelServer* level_server, float min_zoom, float max_zoom);
	~EditorRenderer() override;
	
	Vector2D<int> get_screen_pos(Vector2D<float> world_pos);
	Vector2D<float> get_world_pos(Vector2D<int> screen_pos);
	Vector2D<float> get_mouse_pos();
	
	void add_zoom(float zoom_delta);
	
	void render() override;
	void render_ui();
	void render_grid();
	
	void render_menu();
	void render_toolbar();
	void render_inspector();
	
	void render_level_data();
	void render_shapes();
	void draw_point(Vector2D<float>& point);
	void render_line_normal(std::vector<Vector2D<int>> line_screen_points, Vector2D<float> normal);
	void render_cameras();
//	void render_buffer() override;
};

#endif
