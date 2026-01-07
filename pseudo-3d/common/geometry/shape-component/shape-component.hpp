#ifndef SHAPE_COMPONENT_HPP
#define SHAPE_COMPONENT_HPP

#include <stdio.h>
#include <vector>

#include "vector2D.hpp"
#include "wall.hpp"
#include "math.hpp"

enum ShapeType
{
	DOT,
	CIRCLE,
	LINE,
	RECTANGLE,
	POLYGON
};

class ShapeComponent
{
public:
	ShapeComponent();
	ShapeComponent(ShapeType type, float radius, std::vector<Vector2D<float>> points);

	ShapeType type;
	std::vector<Vector2D<float>> points;
	std::vector<Wall> walls;
	
	//TODO: Getting SAT projection with shape separation
	Vector2D<float> get_line_normal(int f_point, int s_point);
	
	float radius;
};

#endif
