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
	std::vector<float> points_angles = {normalize_angle_unsigned(local_points[0].get_vector_rotation()), normalize_angle_unsigned(local_points[1].get_vector_rotation())};
	
	float half_fov = field_of_view / 2;
	std::vector<float> camera_rays_angles = {normalize_angle_unsigned(rotation - half_fov), normalize_angle_unsigned(rotation + half_fov)};

	if(fabs(camera_rays_angles[1] - camera_rays_angles[0]) > PI) camera_rays_angles[1] += 2 * PI;
	
	if(points_angles[0] < camera_rays_angles[0] - PI) points_angles[0] += 2 * PI;
	if(points_angles[0] > camera_rays_angles[0] + PI) points_angles[0] -= 2 * PI;
	if(points_angles[1] < camera_rays_angles[0] - PI) points_angles[1] += 2 * PI;
	if(points_angles[1] > camera_rays_angles[0] + PI) points_angles[1] -= 2 * PI;
	
	if(fabs(points_angles[0] - points_angles[1]) > PI)
	{
		if(points_angles[0] > points_angles[1]) points_angles[1] += PI * 2;
		else points_angles[0] += PI * 2;
	}
	
	float min_angle = fmin(points_angles[0], points_angles[1]);
	float max_angle = fmax(points_angles[0], points_angles[1]);
	
	Vector2D<float> min_angle_point = min_angle == points_angles[0] ? wall_points[0] : wall_points[1];
	Vector2D<float> max_angle_point = max_angle == points_angles[0] ? wall_points[0] : wall_points[1];
	
	Vector2D<float> min_ray = Vector2D<float>(cos(camera_rays_angles[0]), sin(camera_rays_angles[0]));
	Vector2D<float> max_ray = Vector2D<float>(cos(camera_rays_angles[1]), sin(camera_rays_angles[1]));
	
	float clipped_min = fmax(min_angle, camera_rays_angles[0]);
	float clipped_max = fmin(max_angle, camera_rays_angles[1]);
	
	if (clipped_min >= clipped_max) return new_wall_points;
	
	if (clipped_min > min_angle)
	{
		Line line = Line(position, position + min_ray);
		Vector2D<float> intersection_point = get_line_segment_line_intersection(wall_points[0], wall_points[1], line);
		new_wall_points.push_back(intersection_point);
	}
	
	if (clipped_max < max_angle)
	{
		Line line = Line(position, position + max_ray);
		Vector2D<float> intersection_point = get_line_segment_line_intersection(wall_points[0], wall_points[1], line);
		new_wall_points.push_back(intersection_point);
	}
	
	if (clipped_min == min_angle) new_wall_points.push_back(min_angle_point);
	if (clipped_max == max_angle) new_wall_points.push_back(max_angle_point);
	
	return new_wall_points;
}

float Camera::set_camera_rotation(float new_rotation)
{
	rotation = normalize_angle_unsigned(new_rotation);
	return rotation;
}

Vector2D<float> Camera::set_camera_position(Vector2D<float> new_position)
{
	position = new_position;
	return position;
}
