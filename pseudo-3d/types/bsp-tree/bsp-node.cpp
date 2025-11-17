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

void BSPNode::sort_shapes(Line line, std::vector<BSPShape>* front, std::vector<BSPShape>* back, std::vector<BSPShape> shapes)
{
	separate_line = line;
	for (BSPShape current_shape : shapes)
	{
		Vector2D<float> point;
		std::vector<Vector2D<float>> shape_points = current_shape.points;
		int points_side_count = 0;
		int neutral_points_count = 0;
		
		for (size_t point_index = 0; point_index < shape_points.size(); point_index++)
		{
			point = shape_points[point_index];
			const float result = is_point_on_line(point, separate_line);
			if(result == 0)
			{
				neutral_points_count++;
				continue;
			}
			points_side_count += int(result > 0) - int(result < 0);
		}
		
		if(abs(points_side_count) + neutral_points_count != shape_points.size())
		{
			std::vector<Vector2D<float>>* new_shape_points = separate_shape_by_line(shape_points, separate_line);
			BSPShape back_shape = BSPShape(new_shape_points[0]);
			BSPShape front_shape = BSPShape(new_shape_points[1]);
			
			back->push_back(back_shape);
			back->push_back(front_shape);
			
			delete[] new_shape_points;
			continue;
		}
		if (points_side_count > 0) back->push_back(current_shape);
		if (points_side_count < 0) front->push_back(current_shape);	
	}
}
