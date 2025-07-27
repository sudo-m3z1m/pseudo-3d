#include "physics-component.hpp"

PhysicsComponent::PhysicsComponent()
{
	position = Vector2D<float>();
	shape = ShapeComponent();
}
PhysicsComponent::PhysicsComponent(ShapeComponent shape, Vector2D<float> position)
{
	this->shape = shape;
	this->position = position;
}
