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
	sector_index = shape->sector_index;
}

BSPShape::BSPShape(std::vector<Vector2D<float>> points, std::vector<Wall> walls, int sector_index)
{
	this->points = points;
	this->walls = walls;
	this->sector_index = sector_index;
}

void BSPShape::add_new_wall(Vector2D<float> f_point, Vector2D<float> s_point, Vector2D<float> normal, WindowComponent* window_component)
{ //Need to remake this function and make it like copy constructor
	if(points.size() == 0)
	{
		points.push_back(f_point); points.push_back(s_point);
		Wall new_wall = Wall(0, 1, normal, window_component);
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
	
	Wall new_wall = Wall(int(f_wall_index), int(s_wall_index), normal, window_component);
	walls.push_back(new_wall);
}
