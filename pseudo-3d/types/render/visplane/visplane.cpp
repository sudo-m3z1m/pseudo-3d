#include "visplane.hpp"

VisPlane::VisPlane()
{
	height_z = 0;
	plane_color = Color().get_random_color(255);
	plane_columns = std::vector<RendererColumn>(DEFAULT_SCREEN_WIDTH, RendererColumn(DEFAULT_SCREEN_HEIGHT, 0));
	min_x = -1;
	max_x = -1;
}

VisPlane::VisPlane(float height_z, Color color, int screen_width, int screen_height)
{
	this->height_z = height_z;
	plane_color = color;
	plane_columns = std::vector<RendererColumn>(screen_width, RendererColumn(screen_height, 0));
	min_x = -1;
	max_x = -1;
}

void VisPlane::set_x_range(int new_x)
{
	if((min_x == -1) && (max_x == -1))
	{
		min_x = max_x = new_x;
		return;
	}
	
	if(min_x > new_x) min_x = new_x;
	if(max_x < new_x) max_x = new_x;
}
