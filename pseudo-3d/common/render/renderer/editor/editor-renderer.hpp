#ifndef EDITOR_RENDERER_HPP
#define EDITOR_RENDERER_HPP

#include <stdio.h>
#include <vector>

#include "editor-level-server.hpp"
#include "renderer.hpp"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

class EditorRenderer : public Renderer
{
protected:
	float grid_step, grid_low_step;
	EditorLevelServer* level_server;
	
public:
	EditorRenderer();
	EditorRenderer(Camera* camera, TextureBuffer* texture_buffer, int width, int height, EditorLevelServer* level_server);
	~EditorRenderer() override;
	
	Vector2D<int> get_screen_pos(Vector2D<float> world_pos);
	Vector2D<float> get_world_pos(Vector2D<int> screen_pos);
	
	void render() override;
	void render_ui();
	void render_grid();
	
	void render_level_data();
	void render_shapes();
	void render_line_normal(std::vector<Vector2D<int>> line_screen_points, Vector2D<float> normal);
//	void render_buffer() override;
};

#endif
