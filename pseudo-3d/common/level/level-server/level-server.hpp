#ifndef LEVEL_SERVER_HPP
#define LEVEL_SERVER_HPP

#include <stdio.h>
#include <vector>

#include "shape-component.hpp"
#include "sector.hpp"
#include "camera.hpp"

class LevelServer
{
protected:
	std::vector<ShapeComponent> level_polygons;
	std::vector<Camera*> cameras;
	
public:
	std::vector<Sector> sectors;
	
	LevelServer();
	virtual ~LevelServer();
	
	void clear_level_data();
	
	void remove_item(InspectorItem* item);
	
	void add_camera(Camera* camera);
	Camera* get_camera(int index);
	std::vector<Camera*> get_cameras();
	void remove_camera(Camera*& camera);
	
	void add_new_polygon(ShapeComponent new_polygon);
	ShapeComponent get_levels_shape(int index);
	std::vector<ShapeComponent> get_levels_shapes();
	void remove_shape(ShapeComponent& shape);
	void remove_shape_wall(Wall& wall);
	
	void add_new_sector(Sector new_sector);
	Sector get_sector_by_index(int index);
};

#endif
