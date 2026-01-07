#ifndef MATH_HPP
#define MATH_HPP

#include <stdio.h>
#include <cmath>
#include <vector>

#include "vector2D.hpp"
#include "line.hpp"
#include "wall.hpp"

template <typename T>
int get_sign(T number)
{
	return (number > 0) - (number < 0);
}

float is_point_on_line(Vector2D<float> point, Line line);
Vector2D<float> get_line_projection_point(Vector2D<float> dot_position, Vector2D<float> line_points[]);

Vector2D<float> get_line_segment_line_intersection(Vector2D<float> f_line_point, Vector2D<float> s_line_point, Line line);

//std::vector<Vector2D<float>>* separate_shape_by_line(std::vector<Vector2D<float>> shape_points, Line separate_line);
//void clear_separated_shape(std::vector<Vector2D<float>>* points);

#endif
