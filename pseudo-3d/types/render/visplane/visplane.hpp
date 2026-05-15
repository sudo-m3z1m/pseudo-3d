#ifndef VISPLANE_HPP
#define VISPLANE_HPP

#include <stdio.h>
#include <vector>

#include "color.hpp"
#include "renderer-column.hpp"
#include "constants.hpp"

class VisPlane
{
public:
	float height_z;
	int min_x, max_x;
	std::vector<std::vector<RendererColumn>> plane_columns;
	Color plane_color;
	int tid;
	
	VisPlane();
	VisPlane(float height_z, Color color, int screen_width, int screen_height, int tid);
	
	void set_x_range(int new_x);
};

#endif
