#ifndef WALL_HPP
#define WALL_HPP

#include <stdio.h>
#include <vector>
#include <json.hpp>

#include "vector2D.hpp"
#include "color.hpp"
#include "window-component.hpp"

using json = nlohmann::json;

class Wall
{
private:
	size_t f_p_index, s_p_index;

public:
	Vector2D<float> normal;
	Color color;
	WindowComponent* window_component;
	
	Wall();
	Wall(int f_p_index, int s_p_index, Vector2D<float> normal, WindowComponent* window_component);
	
	std::vector<Vector2D<float>> get_wall_points(std::vector<Vector2D<float>> shape_points); //TODO: Maybe friendly class for shapes?
	
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Wall, f_p_index, s_p_index, normal, window_component);
};

#endif
