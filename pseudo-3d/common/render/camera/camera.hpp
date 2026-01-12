#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <stdio.h>
#include <vector>

#include "line.hpp"
#include "math.hpp"

class Camera
{
private:
	Line frustrum[2]; //TODO: geometrical frustrum. Maybe I can use angle frustrum
	
public:
	Vector2D<float> position;
	float field_of_view;
	float rotation;
	
	Camera();
	Camera(float field_of_view, Vector2D<float> position, float rotation);
	
	bool is_shape_in_frustrum(std::vector<Vector2D<float>> shape_points);
	float set_camera_rotation(float new_rotation);
	Vector2D<float> set_camera_position(Vector2D<float> new_position);
};

#endif
