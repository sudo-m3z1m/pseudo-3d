#include "bsp-node.hpp"

BSPNode::BSPNode()
{
	separate_line = Line();
	front = back = nullptr;
	shape = nullptr;
}

BSPNode::~BSPNode()
{
	delete front;
	delete back;
	delete shape;
}
