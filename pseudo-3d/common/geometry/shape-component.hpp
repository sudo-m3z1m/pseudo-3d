#ifndef SHAPE_COMPONENT_HPP
#define SHAPE_COMPONENT_HPP

#include <stdio.h>
#include <vector>
#include "../../types/common/vector2D.hpp"

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
	
	//TODO: Getting SAT projection with shape separation
	
	float radius;
};

#endif
