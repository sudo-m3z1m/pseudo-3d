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

std::vector<Wall> get_walls_from_shape_points(std::vector<Vector2D<float>> points)
{
	std::vector<Wall> new_walls;
	for(size_t point_index = 0; point_index < points.size(); point_index++)
	{
		const size_t next_point_index = (point_index + 1) % points.size();
		
		Vector2D<float> wall_vector = points[next_point_index] - points[point_index];
		Vector2D<float> normal = wall_vector.get_vector_normal();
		normal.x *= -1; normal.y *= -1;
		
		Wall new_wall = Wall(int(point_index), int(next_point_index), normal);
		
		new_walls.push_back(new_wall);
	}
	
	return new_walls;
}

Line* get_frustrum(float angle)
{
	Line* frustrum = new Line[2];
	Vector2D<float> origin_vector = Vector2D<float>(1.0f, 0.0f);
	
	frustrum[0] = Line(Vector2D<float>(0.0f, 0.0f), origin_vector.rotate_vector(-angle / 2));
	frustrum[1] = Line(Vector2D<float>(0.0f, 0.0f), origin_vector.rotate_vector(angle / 2));
	
	return frustrum;
}
