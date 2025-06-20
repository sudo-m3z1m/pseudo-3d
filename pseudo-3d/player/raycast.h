#ifndef RAYCAST_H
#define RAYCAST_H

#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#include <stdint.h>
#include "../core/game-core.h"
#include "../components/updatable-component.h"

typedef struct Raycast
{
	Vector2D target_pos;
	
	float rotation;
	float max_length;
	float length;
	bool collided;
	
	int texture_index;
	UpdatableComponent* updatable_component;
} Raycast;

void raycast_update(float delta, void* raycast);
Vector2D get_collision_pos(Vector2D position, Raycast** raycast, float player_rotation, char* map);

Raycast* initialize_raycast(float rotation);

#endif
