#include "bsp-shape.hpp"

BSPShape::BSPShape()
{
	shape = nullptr;
	
	Wall wall = Wall{0, 0};
	walls.push_back(wall);
}

BSPShape::BSPShape(ShapeComponent* shape)
{
	this->shape = shape;
	std::vector<Vector2D<float>> shape_points = shape->points;
	
	size_t next_point_index;
	for(size_t point_index = 0; point_index < shape_points.size(); point_index++)
	{
		next_point_index = (point_index + 1) % shape_points.size();
		Wall new_wall = Wall{point_index, next_point_index};
		walls.push_back(new_wall);
	}
}

Wall BSPShape::get_shape_wall()
{
	Wall first_wall = walls.back();
	walls.pop_back();
	
	return first_wall;
}

ShapeComponent* BSPShape::get_shape() //Useless shit. Need to make public class
{
	return shape;
}

std::vector<Vector2D<float>> BSPShape::get_shape_points()
{
	return shape->points;
}
