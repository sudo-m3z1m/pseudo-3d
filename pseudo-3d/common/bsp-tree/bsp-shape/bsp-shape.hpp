#ifndef BSP_SHAPE_HPP
#define BSP_SHAPE_HPP

#include <stdio.h>
#include <vector>

#include "vector2D.hpp"
#include "shape-component.hpp"
#include "line.hpp"
#include "wall.hpp"

class BSPShape
{
private:
	void generate_walls();

public:
	std::vector<Vector2D<float>> points;
	std::vector<Wall> walls;
	
	BSPShape();
	BSPShape(ShapeComponent* shape);
	BSPShape(std::vector<Vector2D<float>> points, std::vector<Wall> walls);

	void add_new_wall(Vector2D<float> f_point, Vector2D<float> s_point, Vector2D<float> normal);
};


#endif
