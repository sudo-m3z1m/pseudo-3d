#include "level-server.hpp"

LevelServer::LevelServer()
{
	level_polygons = std::vector<ShapeComponent>();
	sectors = std::vector<Sector>();
}

LevelServer::~LevelServer() { return; }

void LevelServer::add_new_polygon(ShapeComponent new_polygon)
{
	if (new_polygon.type == DOT) return;
	
	level_polygons.push_back(new_polygon);
}

void LevelServer::add_new_sector(Sector new_sector)
{
	sectors.push_back(new_sector);
}

Sector LevelServer::get_sector_by_index(int index)
{
	return sectors[index];
}
