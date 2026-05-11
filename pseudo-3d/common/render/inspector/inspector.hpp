#ifndef INSPECTOR_TYPES_RENDER_HPP
#define INSPECTOR_TYPES_RENDER_HPP

#include <stdio.h>

#include "imgui_impl_sdl3.h"
#include "level-server.hpp"
#include "vector2D.hpp"
#include "inspector-item.hpp"
#include "window-component.hpp"

class Inspector
{
private:
	const ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse |
	ImGuiWindowFlags_NoResize |
	ImGuiWindowFlags_NoMove |
	ImGuiWindowFlags_NoTitleBar |
	ImGuiWindowFlags_NoBringToFrontOnFocus;
	
	InspectorItem** current_item; //TODO: Instead of getting new properties every render need to save it when item is changes there
	LevelServer* level_server;
	float width, height;
	Vector2D<float> screen_position;
	
public:
	Inspector();
	Inspector(InspectorItem** item, LevelServer* level_server, int width, int height, Vector2D<float>& viewport_size);
	~Inspector();
	
	void render_inspector();
	void render_item_property(InspectorItemProperty& property);
	
	void render_int_property(InspectorItemProperty& property);
	void render_float_property(InspectorItemProperty& property);
	void render_vector_property(InspectorItemProperty& property);
	void render_window_component_property(InspectorItemProperty& property);
	void render_array_property(InspectorItemProperty& property);
	void render_color_property(InspectorItemProperty& property);
	void render_button_property(InspectorItemProperty& property);
};

#endif
