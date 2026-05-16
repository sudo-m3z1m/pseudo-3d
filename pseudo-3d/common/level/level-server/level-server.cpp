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

void LevelServer::clear_level_data()
{
	cameras.clear();
	level_polygons.clear();
	sectors.clear();
}

void LevelServer::remove_item(InspectorItem* item)
{
	if(dynamic_cast<Camera*>(item) != nullptr)
	{
		Camera* camera_item = dynamic_cast<Camera*>(item);
		remove_camera(camera_item);
		return;
	}
	if(dynamic_cast<ShapeComponent*>(item) != nullptr)
	{
		ShapeComponent* shape_item = dynamic_cast<ShapeComponent*>(item);
		remove_shape(*shape_item);
		return;
	}
	if(dynamic_cast<Wall*>(item) != nullptr)
	{
		Wall* wall_item = dynamic_cast<Wall*>(item);
		remove_shape_wall(*wall_item);
		return;
	}
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

std::vector<Camera*> LevelServer::get_cameras()
{
	return cameras;
}

void LevelServer::remove_camera(Camera*& camera)
{
	auto camera_iterator = std::find(cameras.begin(), cameras.end(), camera);
	if(camera_iterator == cameras.end()) return;
	cameras.erase(camera_iterator);
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

void LevelServer::remove_shape(ShapeComponent& shape)
{
	auto shape_iterator = std::find(level_polygons.begin(), level_polygons.end(), shape);
	if(shape_iterator == level_polygons.end()) return;
	
	level_polygons.erase(shape_iterator);
	return;
}

void LevelServer::remove_shape_wall(Wall& wall)
{
	for(ShapeComponent& shape : level_polygons)
	{
		std::vector<Wall>& shape_walls = shape.walls;
		
		auto wall_iterator = std::find(shape_walls.begin(), shape_walls.end(), wall);
		if(wall_iterator == shape_walls.end()) continue;
		
		shape_walls.erase(wall_iterator);
		return;
	}
}

void LevelServer::add_new_sector(Sector new_sector)
{
	sectors.push_back(new_sector);
}

Sector LevelServer::get_sector_by_index(int index)
{
	return sectors[index];
}
