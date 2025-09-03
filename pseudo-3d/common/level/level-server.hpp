#ifndef LEVEL_SERVER_HPP
#define LEVEL_SERVER_HPP

#include <stdio.h>
#include <vector>

#include "bsp-node.hpp"
#include "shape-component.hpp"
#include "bsp-shape.hpp"

class LevelServer
{
private:
	std::vector<ShapeComponent> level_polygons;
	BSPNode* bsp_tree;

public:
	LevelServer();
	~LevelServer();
	
	void add_new_polygon(ShapeComponent new_polygon);
	
	void create_bsp_tree();
	void separate_bsp_node(BSPNode* node, std::vector<BSPShape> node_shapes);
	
	std::vector<BSPShape> generate_bsp_shapes();
};

#endif
