#include "bsp-shape.hpp"

BSPShape::BSPShape()
{
	points = {Vector2D<float>(0, 0)};
	
	Wall wall = Wall{0, 0};
	walls.push_back(wall);
}

BSPShape::BSPShape(ShapeComponent* shape)
{
	points = shape->points;
	generate_walls();
}

BSPShape::BSPShape(std::vector<Vector2D<float>> points)
{
	this->points = points;
	generate_walls();
}

void BSPShape::generate_walls()
{
	size_t next_point_index;
	for(size_t point_index = 0; point_index < points.size(); point_index++)
	{
		next_point_index = (point_index + 1) % points.size();
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

//ShapeComponent* BSPShape::get_shape() //Useless shit. Need to make public class
//{
//	return shape;
//}
//
//std::vector<Vector2D<float>> BSPShape::get_shape_points()
//{
//	return shape->points;
//}
