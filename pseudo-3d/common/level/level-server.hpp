#ifndef LEVEL_SERVER_HPP
#define LEVEL_SERVER_HPP

#include <stdio.h>
#include <vector>

#include "bsp-node.hpp"
#include "shape-component.hpp"
#include "bsp-shape.hpp"
#include "sector.hpp"

class LevelServer
{
private:
	std::vector<ShapeComponent> level_polygons;
	std::vector<Sector> sectors;
	
public:
	BSPNode* bsp_tree;
	
	LevelServer();
	~LevelServer();
	
	void add_new_polygon(ShapeComponent new_polygon);
	void add_new_sector(Sector new_sector);
	Sector get_sector_by_index(int index);
	int get_sector_index_by_point(BSPNode* node, Vector2D<float> point);
	
	void create_bsp_tree();
	std::vector<BSPShape*> separate_shape_by_line(Line line, BSPShape shape);
	void separate_bsp_node(BSPNode* node, std::vector<BSPShape*> node_shapes);
	void sort_shapes(Line line, std::vector<BSPShape*>* front, std::vector<BSPShape*>* back, std::vector<BSPShape*> shapes);
	
	std::vector<BSPShape*> generate_bsp_shapes();
};

#endif
