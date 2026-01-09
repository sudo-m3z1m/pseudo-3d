#include "camera.hpp"

Camera::Camera()
{
	angle = 0;
	position = Vector2D<float>();
	field_of_view = DEFAULT_FOV;
	
	Line* temp_frustrum = get_frustrum(field_of_view);
	frustrum[0] = temp_frustrum[0]; frustrum[1] = temp_frustrum[1];
	
	delete[] temp_frustrum;
}

Camera::Camera(float field_of_view, Vector2D<float> position, float angle)
{
	this->angle = angle;
	this->position = position;
	this->field_of_view = field_of_view;
	
	Line* temp_frustrum = get_frustrum(field_of_view);
	frustrum[0] = temp_frustrum[0]; frustrum[1] = temp_frustrum[1];
	
	delete[] temp_frustrum;
}

bool Camera::is_shape_in_frustrum()
{
	return false; //TODO: Need to consider about angles
}

float Camera::set_camera_angle(float new_angle)
{
	angle = new_angle;
	return angle;
}

Vector2D<float> Camera::set_camera_position(Vector2D<float> new_position)
{
	position = new_position;
	return position;
}
