#include "visplane.hpp"

VisPlane::VisPlane()
{
	height_z = 0;
	plane_color = Color().get_random_color(255);
	plane_columns = std::vector<RendererColumn>(DEFAULT_SCREEN_WIDTH, RendererColumn(DEFAULT_SCREEN_HEIGHT, 0));
	min_x = DEFAULT_SCREEN_WIDTH / 2;
	max_x = DEFAULT_SCREEN_WIDTH / 2;
}

VisPlane::VisPlane(float height_z, Color color, int screen_width, int screen_height)
{
	this->height_z = height_z;
	plane_color = color;
	plane_columns = std::vector<RendererColumn>(screen_width, RendererColumn(screen_height, 0));
	min_x = screen_width / 2;
	max_x = screen_width / 2;
}
