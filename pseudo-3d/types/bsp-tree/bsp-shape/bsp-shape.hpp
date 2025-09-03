#ifndef BSP_SHAPE_HPP
#define BSP_SHAPE_HPP

#include <stdio.h>
#include <vector>

#include "vector2D.hpp"
#include "shape-component.hpp"

struct Wall //Probably needed be unique and separate from this file
{
	size_t f_p_index, s_p_index;
//	Vector2D<float> normal;
};

class BSPShape
{
private:
	ShapeComponent* shape;
	std::vector<Wall> walls;
	
public:
	BSPShape();
	BSPShape(ShapeComponent* shape);
	
	Wall get_shape_wall();
	ShapeComponent* get_shape();
	std::vector<Vector2D<float>> get_shape_points();
};


#endif
