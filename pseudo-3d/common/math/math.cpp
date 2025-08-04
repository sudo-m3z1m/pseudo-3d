#include "math.hpp"

Vector2D<float> get_line_projection_point(Vector2D<float> dot_position, Vector2D<float> line_points[])
{
	Vector2D<float> line_vector = line_points[1] - line_points[0];
	Vector2D<float> components_vector = dot_position - line_points[0];
	
	float line_length = powf(line_vector.x, 2) + powf(line_vector.y, 2);
	float dot_product = line_vector * components_vector;
	
	const float line_projection_k = std::fminf(line_length, std::fmaxf(0, dot_product)) / line_length;
	Vector2D<float> projection_point = line_points[0] + line_vector * line_projection_k;
	
	return projection_point;
}
