#ifndef INSPECTOR_TYPES_RENDER_HPP
#define INSPECTOR_TYPES_RENDER_HPP

#include <stdio.h>

#include "imgui_impl_sdl3.h"
//#include "imgui_impl_sdlrenderer3.h"
#include "vector2D.hpp"
#include "inspector-item.hpp"
#include "window-component.hpp"

class Inspector
{
private:
	InspectorItem* current_item;
	float width, height;
	Vector2D<float> screen_position;
	
public:
	Inspector();
	Inspector(InspectorItem* item, int width, int height, Vector2D<float> viewport_size);
	~Inspector();
	
	void set_current_item(InspectorItem* current_item);
	
	void render_inspector(const ImGuiWindowFlags& window_flags);
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
