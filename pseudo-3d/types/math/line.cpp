#include "line.hpp"

Line::Line()
{
	a = b = c = k = k_b = 0;
}

Line::Line(Vector2D<float> f_point, Vector2D<float> s_point)
{
	a = f_point.y - s_point.y;
	b = s_point.x - f_point.x;
	c = (-a * f_point.x) - (b * f_point.y);
	
	k = -(a / b);
	k_b = -(c / b);
}
