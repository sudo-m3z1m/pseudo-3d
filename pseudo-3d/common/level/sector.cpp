#include "sector.hpp"

Sector::Sector()
{
	floor_z = 0;
	ceiling_z = 0;
}

Sector::Sector(float floor_z, float ceiling_z)
{
	this->floor_z = floor_z;
	this->ceiling_z = ceiling_z;
}

Sector::~Sector()
{
	physics_components.clear();
}
