#ifndef MATH_HPP
#define MATH_HPP

#include <stdio.h>
#include <cmath>
#include "../../types/math/vector2D.hpp"

template <typename T>
int get_sign(T number)
{
	return (number > 0) - (number < 0);
}

Vector2D<float> get_line_projection_point(Vector2D<float> dot_position, Vector2D<float> line_points[]);

#endif
