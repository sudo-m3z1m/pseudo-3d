#ifndef BSP_SHAPE_HPP
#define BSP_SHAPE_HPP

#include <stdio.h>
#include <vector>

#include "vector2D.hpp"
#include "shape-component.hpp"
#include "line.hpp"

struct Wall //Probably need to be unique and separate from this file
{
	size_t f_p_index, s_p_index;
//	Vector2D<float> normal;
};

class BSPShape
{
private:
	std::vector<Wall> walls;
	void generate_walls();

public:
	std::vector<Vector2D<float>> points;
	
	BSPShape();
	BSPShape(ShapeComponent* shape);
	BSPShape(std::vector<Vector2D<float>> points);
	
//	ShapeComponent* get_shape();
//	std::vector<Vector2D<float>> get_shape_points();
	
	Wall get_shape_wall();
};


#endif
