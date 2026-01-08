#include "bsp-node.hpp"

BSPNode::BSPNode()
{
	separate_line = Line();
	front = back = nullptr;
}

BSPNode::~BSPNode()
{
	delete front;
	delete back;
}
