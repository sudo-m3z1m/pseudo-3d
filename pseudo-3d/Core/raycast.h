#ifndef RAYCAST_H
#define RAYCAST_H

#include <stdio.h>
#include <SDL3/SDL.h>
#include "game-core.h"

#endif

typedef struct Raycast
{
	float angle;
	float max_length;
} Raycast;

void apply_length_pos(float length, Vector2D* position, Raycast** raycast);
