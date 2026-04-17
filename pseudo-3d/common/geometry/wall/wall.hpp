#ifndef WALL_HPP
#define WALL_HPP

#include <stdio.h>
#include <vector>

#include "vector2D.hpp"
#include "color.hpp"
#include "window-component.hpp"
#include "inspector-item.hpp"

class Wall : public InspectorItem
{
public:
	size_t f_p_index, s_p_index;
	int tid;
	Vector2D<float> normal;
	Color color;
	WindowComponent* window_component;
	
	Wall();
	Wall(int f_p_index, int s_p_index, Vector2D<float> normal, WindowComponent* window_component);
	
	std::vector<InspectorItemProperty> get_inspector_item_properties() override;
	const char* get_inspector_item_name() override;
	
	std::vector<Vector2D<float>> get_wall_points(std::vector<Vector2D<float>> shape_points); //TODO: Maybe friendly class for shapes?
};

#endif
