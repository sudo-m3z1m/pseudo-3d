#include "wall.hpp"

Wall::Wall()
{
	f_p_index = s_p_index = 0;
	normal = Vector2D<float>(1, 0);
	window_component = nullptr;
	tid = 0;
}

Wall::Wall(int f_p_index, int s_p_index, Vector2D<float> normal, WindowComponent* window_component, int tid)
{
	this->f_p_index = f_p_index;
	this->s_p_index = s_p_index;
	this->normal = normal;
	this->window_component = window_component;
	
	this->color = Color().get_random_color(255);
	this->tid = tid;
}

std::vector<InspectorItemProperty> Wall::get_inspector_item_properties()
{
	std::vector<InspectorItemProperty> properties = {
		{INT, "First index", &f_p_index},
		{INT, "Second index", &s_p_index},
		{VECTOR2, "Normal", &normal},
		{BUTTON, "Invert normal", (void*)invert_wall_normal},
		{INT, "Texture index", &tid},
		{WINDOW_COMPONENT, "Window Component", &window_component},
	};
	if(!window_component)
	{
		InspectorItemProperty button_property = {BUTTON, "Create window component", (void*)handle_window_component};
		properties.pop_back();
		properties.push_back(button_property);
	}
	
	return properties;
}

const char* Wall::get_inspector_item_name()
{
	return "Wall";
}

void Wall::invert_wall_normal(void* instance)
{
	Wall* wall_instance = static_cast<Wall*>(instance);
	wall_instance->normal.invert_vector();
}

void Wall::handle_window_component(void* instance)
{
	Wall* wall = static_cast<Wall*>(instance);
	
	WindowComponent* window_component = new WindowComponent();
	wall->window_component = window_component;
}

std::vector<Vector2D<float>> Wall::get_wall_points(std::vector<Vector2D<float>> shape_points)
{
	std::vector<Vector2D<float>> wall_points = {shape_points[f_p_index], shape_points[s_p_index]};
	return wall_points;
}
