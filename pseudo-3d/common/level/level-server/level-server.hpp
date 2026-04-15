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
	std::vector<Sector> sectors;
	std::vector<Camera*> cameras;
	
public:	
	LevelServer();
	virtual ~LevelServer();
	
	void add_camera(Camera* camera);
	Camera* get_camera(int index);
	
	void add_new_polygon(ShapeComponent new_polygon);
	ShapeComponent get_levels_shape(int index);
	std::vector<ShapeComponent> get_levels_shapes();
	
	void add_new_sector(Sector new_sector);
	Sector get_sector_by_index(int index);
};

#endif
