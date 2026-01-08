#include "wall.hpp"

Wall::Wall()
{
	f_p_index = s_p_index = 0;
	normal = Vector2D<float>(1, 0);
}

Wall::Wall(int f_p_index, int s_p_index, Vector2D<float> normal)
{
	this->f_p_index = f_p_index;
	this->s_p_index = s_p_index;
	this->normal = normal;
}

std::vector<Vector2D<float>> Wall::get_wall_points(std::vector<Vector2D<float>> shape_points)
{
	std::vector<Vector2D<float>> wall_points = {shape_points[f_p_index], shape_points[s_p_index]};
	return wall_points;
}
