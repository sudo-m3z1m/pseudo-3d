#include "bsp-shape.hpp"

BSPShape::BSPShape()
{
	points = std::vector<Vector2D<float>>();
	walls = std::vector<Wall>();
}

BSPShape::BSPShape(ShapeComponent* shape)
{
	points = shape->points;
	walls = shape->walls;
}

BSPShape::BSPShape(std::vector<Vector2D<float>> points, std::vector<Wall> walls)
{
	this->points = points;
	this->walls = walls;
}

void BSPShape::add_new_wall(Vector2D<float> f_point, Vector2D<float> s_point, Vector2D<float> normal)
{
	if(points.size() == 0)
	{
		points.push_back(f_point); points.push_back(s_point);
		Wall new_wall = Wall(0, 1, normal);
		walls.push_back(new_wall);
		return;
	}
	
	size_t f_wall_index = (points.size() - 1), s_wall_index = 0;
	if(f_point != points[f_wall_index])
	{
		points.push_back(f_point);
		f_wall_index = points.size() - 1;
	}
	if(s_point != points[s_wall_index])
	{
		points.push_back(s_point);
		s_wall_index = points.size() - 1;
	}
	
	Wall new_wall = Wall(int(f_wall_index), int(s_wall_index), normal);
	walls.push_back(new_wall);
	
//	std::vector<Vector2D<float>>::iterator f_point_iterator = std::find(points.begin(), points.end(), f_point);
//	size_t f_wall_index = f_point_iterator - points.begin();
//	if(f_point_iterator == points.end())
//	{
//		points.push_back(f_point);
//		f_wall_index = points.size() - 1;
//	}
//	
//	std::vector<Vector2D<float>>::iterator s_point_iterator = std::find(points.begin(), points.end(), s_point);
//	size_t s_wall_index = s_point_iterator - points.begin();
//	if(s_point_iterator == points.end())
//	{
//		points.push_back(s_point);
//		s_wall_index = points.size() - 1;
//	}
//	
//	Wall new_wall = Wall(int(f_wall_index), int(s_wall_index), normal);
//	walls.push_back(new_wall);
}
