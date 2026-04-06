#ifndef EDITOR_RENDERER_HPP
#define EDITOR_RENDERER_HPP

#include <stdio.h>
#include <vector>
#include "renderer.hpp"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

class EditorRenderer : public Renderer
{
protected:
	
public:
	EditorRenderer();
	EditorRenderer(Camera* camera, TextureBuffer* texture_buffer, int width, int height);
	~EditorRenderer() override;
	
	void render() override;
	void render_buffer() override;
};

#endif
