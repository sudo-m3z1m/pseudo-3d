#include "level-server.hpp"

LevelServer::LevelServer()
{
	bsp_tree = new BSPNode();
}

LevelServer::~LevelServer()
{
	delete bsp_tree;
}

void LevelServer::add_new_polygon(ShapeComponent new_polygon)
{
	if (new_polygon.type == DOT) return;
	
	level_polygons.push_back(new_polygon);
}

void LevelServer::create_bsp_tree()
{
	std::vector<BSPShape> bsp_shapes = generate_bsp_shapes();
	bsp_tree = new BSPNode();
	
	separate_bsp_node(bsp_tree, bsp_shapes);
}

void LevelServer::separate_bsp_node(BSPNode* node, std::vector<BSPShape> node_shapes)
{
	if (node_shapes.empty()) return;
	if (node_shapes.size() == 1)
	{
		node->shape_component = node_shapes[0].get_shape();
		return;
	}
	
	BSPShape bsp_shape = node_shapes[0];
	
	Wall shape_wall = bsp_shape.get_shape_wall();
	std::vector<Vector2D<float>> shape_points = bsp_shape.get_shape_points();
	Line separate_line = Line(shape_points[shape_wall.f_p_index], shape_points[shape_wall.s_p_index]);
	
	std::vector<BSPShape> front, back;
	node->sort_shapes(separate_line, &front, &back, node_shapes);
	node_shapes.clear();
	
	node->front = new BSPNode(); node->back = new BSPNode();
	separate_bsp_node(node->front, front);
	separate_bsp_node(node->back, back);
}

std::vector<BSPShape> LevelServer::generate_bsp_shapes()
{
	std::vector<BSPShape> bsp_shapes;
	for(size_t shape_index = 0; shape_index < level_polygons.size(); shape_index++)
	{
		BSPShape new_bsp_shape = BSPShape(&level_polygons[shape_index]);
		bsp_shapes.push_back(new_bsp_shape);
	}
	return bsp_shapes;
}
