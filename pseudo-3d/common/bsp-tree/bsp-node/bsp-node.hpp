#ifndef BSP_NODE_HPP
#define BSP_NODE_HPP

#include <stdio.h>
#include <iostream>
#include <vector>

#include "line.hpp"
#include "math.hpp"
#include "shape-component.hpp"
#include "bsp-shape.hpp"

class BSPNode
{
public:
	Line separate_line;
	std::vector<BSPShape*> shapes;
	BSPNode* back;
	BSPNode* front;
	
	BSPNode();
	~BSPNode();
};

#endif
