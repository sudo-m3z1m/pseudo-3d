#include "shape-component.hpp"

ShapeComponent::ShapeComponent()
{
	type = DOT;
	radius = 0;
}

ShapeComponent::ShapeComponent(ShapeType type, float radius, std::vector<Vector2D<float>> points)
{
	this->type = type;
	this->radius = radius;
	this->points = points;
}

Vector2D<float> ShapeComponent::get_line_normal(int f_point_index, int s_point_index)
{
	if (type == DOT || type == CIRCLE)
		return Vector2D<float>(1.0f, 0.0f);

	Vector2D<float> f_point = points[f_point_index], s_point = points[s_point_index];
	Vector2D<float> line_vector = s_point - f_point;
	
	return Vector2D<float>(-line_vector.y, line_vector.x).normalize_vector_2d();
}
