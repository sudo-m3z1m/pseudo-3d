#include "math.hpp"

Vector2D<float> get_line_projection_point(Vector2D<float> dot_position, Vector2D<float> line_points[])
{
	Vector2D<float> line_vector = line_points[1] - line_points[0];
	Vector2D<float> components_vector = dot_position - line_points[0];
	
	float line_length = powf(line_vector.x, 2) + powf(line_vector.y, 2);
	float dot_product = line_vector * components_vector;
	
	const float line_projection_k = std::fminf(line_length, std::fmaxf(0, dot_product)) / line_length;
	Vector2D<float> projection_point = line_points[0] + line_vector * line_projection_k;
	
	return projection_point;
}

Vector2D<float> get_line_segment_line_intersection(Vector2D<float> f_line_point, Vector2D<float> s_line_point, Line line)
{
	float f_point_k = is_point_on_line(f_line_point, line);
	float s_point_k = is_point_on_line(s_line_point, line);
	Vector2D<float> line_segment_vector = s_line_point - f_line_point;
	
	float t = f_point_k / (f_point_k - s_point_k);
	
	Vector2D<float> intersection_point;
	intersection_point.x = f_line_point.x + (t * line_segment_vector.x);
	intersection_point.y = f_line_point.y + (t * line_segment_vector.y);
	
	return intersection_point;
}

float is_point_on_line(Vector2D<float> point, Line line)
{
	return (line.a * point.x) + (line.b * point.y) + line.c;
}

//std::vector<Vector2D<float>>* separate_shape_by_line(std::vector<Vector2D<float>> shape_points, Line separate_line)
//{
//	size_t next_point_index;
//
//	std::vector<Vector2D<float>>* new_shapes = new std::vector<Vector2D<float>>[2];
//	
//	int f_point_k, s_point_k;
//	int f_point_side_index, s_point_side_index;
//	
//	for(size_t point_index = 0; point_index < shape_points.size(); point_index++)
//	{
//		next_point_index = (point_index + 1) % shape_points.size();
//
//		Vector2D<float> f_point = shape_points[point_index], s_point = shape_points[next_point_index];
//		
//		f_point_k = is_point_on_line(f_point, separate_line);
//		s_point_k = is_point_on_line(s_point, separate_line);
//		f_point_side_index = std::signbit(f_point_k);
//		s_point_side_index = std::signbit(s_point_k);
//		
//		if(f_point_k == 0 || s_point_k == 0)
//		{
//			const int points_index = f_point_side_index + s_point_side_index;
//			new_shapes[points_index].push_back(f_point);
//			new_shapes[points_index].push_back(s_point);
//			continue;
//		}
//		
//		if (f_point_side_index != s_point_side_index)
//		{
////			Vector2D<float> back_point = (shape_points[point_index] * int(f_point_k > 0)) +
////			(shape_points[next_point_index] * int(s_point_k > 0));
////			Vector2D<float> front_point = (shape_points[point_index] * int(f_point_k < 0)) +
////			(shape_points[next_point_index] * int(s_point_k < 0));
//			Vector2D<float> new_point = get_line_segment_line_intersection(f_point, s_point, separate_line);
//			
//			new_shapes[f_point_side_index].push_back(f_point); new_shapes[f_point_side_index].push_back(new_point);
//			new_shapes[s_point_side_index].push_back(new_point); new_shapes[s_point_side_index].push_back(s_point);
////			new_shapes[0].push_back(back_point); new_shapes[0].push_back(new_point);
////			new_shapes[1].push_back(new_point); new_shapes[1].push_back(front_point);
//			
//			continue;
//		}
//		
//		new_shapes[f_point_side_index].push_back(shape_points[point_index]);
//		new_shapes[s_point_side_index].push_back(shape_points[next_point_index]);
//	}
//	
//	clear_separated_shape(&new_shapes[0]);
//	clear_separated_shape(&new_shapes[1]);
//	
//	return new_shapes;
//}
//
//void clear_separated_shape(std::vector<Vector2D<float>>* points)
//{
//	size_t next_point_index, point_index = 0;
//	Vector2D<float> current_point, next_point;
//	
//	while(point_index < points->size())
//	{
//		next_point_index = (point_index + 1) % points->size();
//		current_point = (*points)[point_index];
//		next_point = (*points)[next_point_index];
//		
//		if (current_point != next_point)
//		{
//			point_index++;
//			continue;
//		}
//		points->erase(points->begin() + point_index);
//	}
//}
