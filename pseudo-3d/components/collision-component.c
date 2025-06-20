#include "collision-component.h"

CollisionComponent* initialize_collision_component(void(*update)(void* owner))
{
	CollisionComponent* new_component = malloc(sizeof(CollisionComponent));
	
	new_component->update = update;
	return new_component;
}
