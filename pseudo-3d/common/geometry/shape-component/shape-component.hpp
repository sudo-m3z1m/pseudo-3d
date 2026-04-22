#ifndef SHAPE_COMPONENT_HPP
#define SHAPE_COMPONENT_HPP

#include <stdio.h>
#include <vector>

#include "vector2D.hpp"
#include "wall.hpp"
#include "math.hpp"

enum ShapeType : uint32_t
{
	DOT,
	CIRCLE,
	LINE,
	RECTANGLE,
	POLYGON
};

class ShapeComponent : public InspectorItem
{
public:
	int sector_index; //TODO: Temporary probably
	std::vector<InspectorItemProperty> points_properties;
	
	ShapeType type;
	std::vector<Vector2D<float>> points;
	std::vector<Wall> walls;
	float radius;
	
	ShapeComponent();
	ShapeComponent(ShapeType type, float radius, std::vector<Vector2D<float>> points, std::vector<Wall> walls, int sector_index);
	
	std::vector<InspectorItemProperty> get_inspector_item_properties() override;
	const char* get_inspector_item_name() override;
	
	Vector2D<float> get_center_point();
	
	//TODO: Getting SAT projection with shape separation
	Vector2D<float> get_line_normal(int f_point, int s_point);
};

#endif
