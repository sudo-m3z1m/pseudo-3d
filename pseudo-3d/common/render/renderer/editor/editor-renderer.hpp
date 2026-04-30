#ifndef EDITOR_RENDERER_HPP
#define EDITOR_RENDERER_HPP

#include <stdio.h>
#include <vector>
#include <string>

#include "editor-level-server.hpp"
#include "nfd.hpp"
#include "renderer.hpp"
#include "inspector.hpp"
#include "sectors-inspector.hpp"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

enum Sizes
{
	VERTEX_SCREEN_SIZE = 8
};


class EditorRenderer : public Renderer
{
protected:
	const ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse |
	ImGuiWindowFlags_NoResize |
	ImGuiWindowFlags_NoMove |
	ImGuiWindowFlags_NoTitleBar |
	ImGuiWindowFlags_NoBringToFrontOnFocus |
	ImGuiWindowFlags_NoScrollbar |
	ImGuiWindowFlags_NoScrollWithMouse;
	Inspector* inspector;
	SectorsInspector* sectors_inspector;
	
	InspectorItem* current_item;
	
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
	Vector2D<float> get_mouse_screen_pos();
	Vector2D<float> get_mouse_world_pos();
	
	void get_mouse_item();
	void add_zoom(float zoom_delta);
	
	void render() override; //TODO: Handles clicks too. Now it's OK but it's gets worse in future
	void render_ui();
	void render_grid();
	
	void render_menu();
	void render_toolbar();
	
	void render_level_data();
	void render_shapes();
	void draw_point(Vector2D<float>& point);
	void render_line_normal(std::vector<Vector2D<int>> line_screen_points, Vector2D<float> normal);
	void render_cameras();
//	void render_buffer() override;
	
	void handle_mouse_click(); //TODO: Current tool clicking and holding mouse methods!
};

#endif
