#include "shape-component.hpp"

bool ShapeComponent::operator==(const ShapeComponent& shape)
{
	std::vector<Vector2D<float>> shape_points = shape.points;
	if(points.size() != shape_points.size()) return false;
	for (int point_index = 0; point_index < points.size(); point_index++)
	{
		if(points[point_index] != shape_points[point_index]) return false;
	}
	return true;
}

ShapeComponent::ShapeComponent()
{
	sector_index = 0;
	type = DOT;
	radius = 0;
}

ShapeComponent::ShapeComponent(ShapeType type, float radius, std::vector<Vector2D<float>> points, std::vector<Wall> walls, int sector_index)
{
	this->sector_index = sector_index;
	this->type = type;
	this->radius = radius;
	this->points = points;
	this->walls = walls;
	if(!walls.size()) this->walls = get_walls_from_shape_points(points); //TODO: walls will be readed from map file
}

ShapeComponent::~ShapeComponent()
{
	
}

std::vector<InspectorItemProperty> ShapeComponent::get_inspector_item_properties()
{
	points_properties = std::vector<InspectorItemProperty>();
	for(int point_index = 0; point_index < points.size(); point_index++)
		points_properties.push_back({VECTOR2, "Point", &points[point_index]});
	
	std::vector<InspectorItemProperty> properties = {
		{INT, "Sector index", &sector_index},
		{ARRAY, "Points", &points_properties},
	};
	
	return properties;
}

const char* ShapeComponent::get_inspector_item_name()
{
	return "Shape";
}

Vector2D<float> ShapeComponent::get_center_point()
{
	Vector2D<float> center_point;
	for(Vector2D<float>& point : points)
	{
		center_point += point;
	}
	center_point = center_point / points.size();
	
	return center_point;
}

Vector2D<float> ShapeComponent::get_line_normal(int f_point_index, int s_point_index)
{
	if (type == DOT || type == CIRCLE)
		return Vector2D<float>(1.0f, 0.0f);

	Vector2D<float> f_point = points[f_point_index], s_point = points[s_point_index];
	Vector2D<float> line_vector = s_point - f_point;
	
	return Vector2D<float>(-line_vector.y, line_vector.x).normalize_vector_2d();
}
