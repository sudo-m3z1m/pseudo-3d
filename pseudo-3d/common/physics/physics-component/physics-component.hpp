#ifndef PHYSICS_COMPONENT_HPP
#define PHYSICS_COMPONENT_HPP

#include <stdio.h>
#include "../../geometry/shape-component.hpp"
#include "../../../types/common/vector2D.hpp"

class PhysicsComponent
{
public:
	PhysicsComponent();
	PhysicsComponent(ShapeComponent shape, Vector2D<float> position);
	
	Vector2D<float> position;
	ShapeComponent shape;
	float rotation;
	//float mass; TODO: Make some physics!
};

#endif
