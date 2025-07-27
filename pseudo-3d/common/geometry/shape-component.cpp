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
