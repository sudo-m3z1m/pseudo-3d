#include "bsp-node.hpp"

BSPNode::BSPNode()
{
	separate_line = Line();
	front = back = nullptr;
	shape_component = nullptr;
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
		std::vector<Vector2D<float>> shape_points = current_shape.get_shape_points();
		int points_side_count = 0;
		
		Vector2D<float> center_point;
		
		for (size_t point_index = 0; point_index < shape_points.size(); point_index++)
		{
			point = shape_points[point_index];
			center_point += point;
//			const float result = (separate_line.a * point.x) + (separate_line.b * point.y) + separate_line.c;
//			points_side_count += int(result > 0) - int(result < 0);
		}
		
		center_point.x = center_point.x / shape_points.size();
		center_point.y = center_point.y / shape_points.size();
		const float result = (separate_line.a * center_point.x) + (separate_line.b * center_point.y) + separate_line.c;
		
		if (result == 0) continue;
		if (result > 0) back->push_back(current_shape);
		if (result < 0) front->push_back(current_shape);
		
//		if (abs(points_side_count) != shape_points.size()) continue;
//		if (points_side_count > 0) back->push_back(current_shape);
//		if (points_side_count < 0) front->push_back(current_shape);
	}
}
