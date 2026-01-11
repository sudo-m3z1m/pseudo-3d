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
	BSPShape shape; //TODO: Need to be a pointer for shape in heap
	BSPNode* back;
	BSPNode* front;
	
	BSPNode();
	~BSPNode();
};

#endif
