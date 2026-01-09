#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <stdio.h>

#include "line.hpp"
#include "math.hpp"

class Camera
{
private:
	Line frustrum[2]; //TODO: geometrical frustrum. Maybe I can use angle frustrum
	float field_of_view;
	
	Vector2D<float> position;
	float angle;
public:
	Camera();
	Camera(float field_of_view, Vector2D<float> position, float angle);
	
	bool is_shape_in_frustrum();
	float set_camera_angle(float new_angle);
	Vector2D<float> set_camera_position(Vector2D<float> new_position);
};

#endif
