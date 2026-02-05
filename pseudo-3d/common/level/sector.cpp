#include "sector.hpp"

Sector::Sector()
{
	floor_z = 0;
	ceiling_z = 0;
	
	floor_color = Color().get_random_color(255);
	ceiling_color = Color().get_random_color(255);
}

Sector::Sector(float floor_z, float ceiling_z)
{
	this->floor_z = floor_z;
	this->ceiling_z = ceiling_z;
	
	floor_color = Color().get_random_color(255);
	ceiling_color = Color().get_random_color(255);
}

Sector::~Sector()
{
	physics_components.clear();
}
