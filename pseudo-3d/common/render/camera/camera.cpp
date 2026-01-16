#include "camera.hpp"

Camera::Camera()
{
	rotation = 0;
	position = Vector2D<float>();
	field_of_view = DEFAULT_FOV;
	
	Line* temp_frustrum = get_frustrum(field_of_view);
	frustrum = new ShapeComponent();
	
	delete[] temp_frustrum;
}

Camera::Camera(float field_of_view, Vector2D<float> position, float rotation)
{
	this->rotation = rotation;
	this->position = position;
	this->field_of_view = field_of_view;
	
	Line* temp_frustrum = get_frustrum(field_of_view);
	
	delete[] temp_frustrum;
}

bool Camera::is_shape_in_frustrum(std::vector<Vector2D<float>> shape_points)
{
	std::vector<float> points_angles;
	float f_point_angle = (shape_points[0] - position).get_vector_rotation();
	float min_angle = f_point_angle, max_angle = f_point_angle;
	
	for(Vector2D<float> point : shape_points)
	{
		float angle_to_point = (point - position).get_vector_rotation();
		points_angles.push_back(angle_to_point);
		
		min_angle = angle_to_point ? angle_to_point < min_angle : min_angle;
		max_angle = angle_to_point ? angle_to_point > max_angle : max_angle;
		
		if (is_point_in_frustrum(point)) return true;
	}

	if(max_angle - min_angle < PI) return true;
	
	return false; //TODO: Need to consider about angles
}

bool Camera::is_point_in_frustrum(Vector2D<float> point)
{
	const float min_angle = rotation - (field_of_view / 2), max_angle = rotation + (field_of_view / 2);
	float point_angle;
	
	point_angle = (point - position).get_vector_rotation();
	if(point_angle >= min_angle && point_angle <= max_angle) return true;
	
	return false;
}

std::vector<Vector2D<float>> Camera::clip_wall_by_frustrum(std::vector<Vector2D<float>> wall_points)
{
	std::vector<Vector2D<float>> new_wall_points;
	std::vector<Vector2D<float>> local_points = {wall_points[0] - position, wall_points[1] - position};
	
	float min_ray_angle = rotation - field_of_view / 2;
	float max_ray_angle = rotation + field_of_view / 2;
	Vector2D<float> negative_ray = Vector2D<float>(cos(min_ray_angle), sin(min_ray_angle));
	Vector2D<float> positive_ray = Vector2D<float>(cos(max_ray_angle), sin(max_ray_angle));
	
	float min_ray_is_f_point_inside = negative_ray.cross_product(local_points[0]);
	float min_ray_is_s_point_inside = negative_ray.cross_product(local_points[1]);
	float max_ray_is_f_point_inside = positive_ray.cross_product(local_points[0]);
	float max_ray_is_s_point_inside = positive_ray.cross_product(local_points[1]);
	
	if((max_ray_is_f_point_inside <= 0) && (min_ray_is_f_point_inside >= 0))
	{
		new_wall_points.push_back(wall_points[0]);
	}
	if((max_ray_is_s_point_inside <= 0) && (min_ray_is_s_point_inside >= 0))
	{
		new_wall_points.push_back(wall_points[1]);
	}
	
	if((min_ray_is_f_point_inside <= 0) != (min_ray_is_s_point_inside <= 0))
	{
		Line negative_line = Line(position, position + negative_ray);
		Vector2D<float> intersection_point = get_line_segment_line_intersection(wall_points[0], wall_points[1], negative_line);
		new_wall_points.push_back(intersection_point);
	}
	
	if((max_ray_is_f_point_inside <= 0) != (max_ray_is_s_point_inside <= 0))
	{
		Line positive_line = Line(position, position + positive_ray);
		Vector2D<float> intersection_point = get_line_segment_line_intersection(wall_points[0], wall_points[1], positive_line);
		new_wall_points.push_back(intersection_point);
	}
	
	return new_wall_points;
}

float Camera::set_camera_rotation(float new_rotation)
{
	rotation = new_rotation;
	return rotation;
}

Vector2D<float> Camera::set_camera_position(Vector2D<float> new_position)
{
	position = new_position;
	return position;
}
