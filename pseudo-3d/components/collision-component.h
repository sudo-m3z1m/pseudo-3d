#ifndef COLLISION_COMPONENT_H
#define COLLISION_COMPONENT_H

#include <stdio.h>
#include <stdlib.h>

typedef struct CollisionComponent
{
	float collision_radius;
	void(*update)(void* owner);
} CollisionComponent;

CollisionComponent* initialize_collision_component(float collision_radius, void(*update)(void* owner));

#endif
