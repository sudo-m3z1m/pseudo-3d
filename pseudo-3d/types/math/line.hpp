#ifndef LINE_HPP
#define LINE_HPP

#include <stdio.h>
#include "./vector2D.hpp"

class Line
{
public:
	Line();
	Line(Vector2D<float> f_point, Vector2D<float> s_point);
	
	float a, b, c, k, k_b;
};

#endif
