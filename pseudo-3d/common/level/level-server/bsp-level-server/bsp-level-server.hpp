#ifndef BSP_LEVEL_SERVER_HPP
#define BSP_LEVEL_SERVER_HPP

#include <stdio.h>

#include "level-server.hpp"
#include "bsp-node.hpp"
#include "bsp-shape.hpp"

class BSPLevelServer : public LevelServer
{
public:
	BSPNode* bsp_tree;

	BSPLevelServer();
	~BSPLevelServer() override;
	
	int get_sector_index_by_point(BSPNode* node, Vector2D<float> point);
	
	std::vector<BSPShape*> generate_bsp_shapes();
	void create_bsp_tree();
	std::vector<BSPShape*> separate_shape_by_line(Line line, BSPShape shape);
	void separate_bsp_node(BSPNode* node, std::vector<BSPShape*> node_shapes);
	void sort_shapes(Line line, std::vector<BSPShape*>* front, std::vector<BSPShape*>* back, std::vector<BSPShape*> shapes);
};

#endif
