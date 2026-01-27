#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <stdio.h>
#include <iostream>
#include <vector>

#include "line.hpp"
#include "math.hpp"
#include "shape-component.hpp"
#include "line.hpp"

class Camera
{
private:
	ShapeComponent* frustrum;
	
public:
	Vector2D<float> position;
	float height_z;
	float field_of_view;
	float rotation;
	
	Camera();
	Camera(float field_of_view, Vector2D<float> position, float rotation, float height_z);
	
	bool is_shape_in_frustrum(std::vector<Vector2D<float>> shape_points);
	bool is_point_in_frustrum(Vector2D<float> point);
	std::vector<Vector2D<float>> clip_wall_by_frustrum(std::vector<Vector2D<float>> wall_points);
	
	float set_camera_rotation(float new_rotation);
	Vector2D<float> set_camera_position(Vector2D<float> new_position);
};

#endif
