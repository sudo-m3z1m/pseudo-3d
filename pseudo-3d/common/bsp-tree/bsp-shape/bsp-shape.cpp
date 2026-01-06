#include "bsp-shape.hpp"

BSPShape::BSPShape()
{
	points = {Vector2D<float>(0, 0)};
	
	Wall wall = Wall{0, 0, Vector2D<float>()};
	walls.push_back(wall);
}

BSPShape::BSPShape(ShapeComponent* shape)
{
	points = shape->points;
	walls = shape->walls;
	generate_walls();
}

BSPShape::BSPShape(std::vector<Vector2D<float>> points, std::vector<Wall> walls)
{
	this->points = points;
	this->walls = walls;
	generate_walls();
}

void BSPShape::add_new_wall(Vector2D<float> f_point, Vector2D<float> s_point, Vector2D<float> normal)
{
	std::vector<Vector2D<float>>::iterator f_point_iterator = std::find(points.begin(), points.end(), f_point);
	std::vector<Vector2D<float>>::iterator s_point_iterator = std::find(points.begin(), points.end(), s_point);
	size_t f_wall_index = f_point_iterator - points.begin(), s_wall_index = s_point_iterator - points.begin();
	
	if(f_point_iterator == points.end())
	{
		points.push_back(f_point);
		f_wall_index = points.size() - 1;
	}
	if(s_point_iterator == points.end())
	{
		points.push_back(s_point);
		s_wall_index = points.size() - 2;
	}
	
	Wall new_wall = Wall(int(f_wall_index), int(s_wall_index), normal);
	walls.push_back(new_wall);
}

//void BSPShape::generate_walls()
//{
//	size_t next_point_index;
//	for(size_t point_index = 0; point_index < points.size(); point_index++)
//	{
//		next_point_index = (point_index + 1) % points.size();
//		Wall new_wall = Wall{point_index, next_point_index};
//		walls.push_back(new_wall);
//	}
//}

//Wall BSPShape::get_shape_wall()
//{
//	Wall first_wall = walls.back();
//	walls.pop_back();
//	
//	return first_wall;
//}

//ShapeComponent* BSPShape::get_shape() //Useless shit. Need to make public class
//{
//	return shape;
//}
//
//std::vector<Vector2D<float>> BSPShape::get_shape_points()
//{
//	return shape->points;
//}
