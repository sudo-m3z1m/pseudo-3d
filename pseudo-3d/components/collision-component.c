#include "collision-component.h"

CollisionComponent* initialize_collision_component(float collision_radius, void(*update)(void* owner))
{
	CollisionComponent* new_component = malloc(sizeof(CollisionComponent));
	
	new_component->update = update;
	new_component->collision_radius = collision_radius;
	return new_component;
}
