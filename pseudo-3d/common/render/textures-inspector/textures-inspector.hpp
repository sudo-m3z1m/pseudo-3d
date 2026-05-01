#ifndef textures_inspector_hpp
#define textures_inspector_hpp

#include <stdio.h>

#include "texture-buffer.hpp"
#include "imgui_impl_sdl3.h"

class TexturesInspector
{
protected:
	const ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse |
	ImGuiWindowFlags_NoResize |
	ImGuiWindowFlags_NoMove |
	ImGuiWindowFlags_NoTitleBar |
	ImGuiWindowFlags_NoBringToFrontOnFocus |
	ImGuiWindowFlags_NoScrollWithMouse |
	ImGuiWindowFlags_NoScrollbar;
	
	float width, height;
	Vector2D<float> screen_position;
	
public:
	TexturesInspector();
	TexturesInspector(int width, int height, Vector2D<float> viewport_size);
	~TexturesInspector();
	
	void render(TextureBuffer*& buffer);
};

#endif
