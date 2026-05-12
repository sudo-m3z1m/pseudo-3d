#include "bsp-node.hpp"

BSPNode::BSPNode()
{
	separate_line = Line();
	front = back = nullptr;
	shapes = std::vector<BSPShape*>();
}

BSPNode::~BSPNode()
{
	delete front;
	delete back;
	shapes.clear();
}
