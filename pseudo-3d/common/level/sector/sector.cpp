#include "sector.hpp"

Sector::Sector()
{
	floor_z = 0;
	ceiling_z = 0;
	floor_tid = 0;
	ceiling_tid = 0;
	
	floor_color = Color().get_random_color(255);
	ceiling_color = Color().get_random_color(255);
}

Sector::Sector(float floor_z, float ceiling_z, int floor_tid, int ceiling_tid, Color floor_color, Color ceiling_color)
{
	this->floor_z = floor_z;
	this->ceiling_z = ceiling_z;
	this->floor_tid = floor_tid;
	this->ceiling_tid = ceiling_tid;
	
	this->floor_color = floor_color;
	this->ceiling_color = ceiling_color;
}

Sector::~Sector()
{
	physics_components.clear();
}

std::vector<InspectorItemProperty> Sector::get_inspector_item_properties()
{
	return {
		{FLOAT, "Floor height", &floor_z},
		{FLOAT, "Ceiling height", &ceiling_z},
		{INT, "Floor texture index", &floor_tid},
		{INT, "Ceiling texture index", &ceiling_tid},
	};
}

const char* Sector::get_inspector_item_name()
{
	return "Sector";
}
