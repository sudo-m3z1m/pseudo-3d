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
private:
	Line separate_line;

public:
	BSPShape shape;
	BSPNode* back;
	BSPNode* front;
	
	BSPNode();
	~BSPNode();
	
	void sort_shapes(Line line, std::vector<BSPShape>* front, std::vector<BSPShape>* back, std::vector<BSPShape> shapes);
};

#endif
