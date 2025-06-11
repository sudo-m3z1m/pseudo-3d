#ifndef RAYCAST_H
#define RAYCAST_H

#include <stdio.h>
#include <stdlib.h>
#include <SDL3/SDL.h>
#include <stdint.h>
#include "../core/game-core.h"
#include "../game-core/updatable-component.h"

#endif

typedef struct Raycast
{
	float angle;
	float max_length;
	float length;
	bool collided;
	
	UpdatableComponent* updatable_component;
	Color color;
} Raycast;

void update(float delta);
void apply_collision_pos(Vector2D* position, Raycast** raycast, char* map);

static void initialize_raycast(Raycast** raycast, float angle);
void initialize_raycasts(Raycast** raycasts[], float spread_angle, size_t raycasts_num);

static void update_raycast(Raycast** raycast, float delta_angle);
void update_raycasts(Raycast** raycasts[], float delta_angle, size_t raycasts_num);
