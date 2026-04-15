#include "level-server.hpp"

LevelServer::LevelServer()
{
	level_polygons = std::vector<ShapeComponent>();
	sectors = std::vector<Sector>();
	cameras = std::vector<Camera*>();
}

LevelServer::~LevelServer()
{
	cameras.clear();
}

void LevelServer::add_camera(Camera* camera)
{
	cameras.push_back(camera);
}

Camera* LevelServer::get_camera(int index)
{
	if(index >= cameras.size()) return nullptr;
	
	return cameras[index];
}

void LevelServer::add_new_polygon(ShapeComponent new_polygon)
{
	if (new_polygon.type == DOT) return;
	
	level_polygons.push_back(new_polygon);
}

ShapeComponent LevelServer::get_levels_shape(int index)
{
	if(index >= level_polygons.size()) return ShapeComponent();
	
	return level_polygons[index];
}

std::vector<ShapeComponent> LevelServer::get_levels_shapes()
{
	return level_polygons;
}

void LevelServer::add_new_sector(Sector new_sector)
{
	sectors.push_back(new_sector);
}

Sector LevelServer::get_sector_by_index(int index)
{
	return sectors[index];
}
