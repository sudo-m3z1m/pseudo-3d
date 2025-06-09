#ifndef GAME_CORE_H
#define GAME_CORE_H

#define PI 3.141592
#define ZERO_VECTOR_2D ((Vector2D){0.0, 0.0})
#define ONE_VECTOR_2D ((Vector2D){1.0, 1.0})
#define X_VECTOR_2D ((Vector2D){1.0, 0.0})
#define Y_VECTOR_2D ((Vector2D){0.0, 1.0})

#include <stdio.h>
#include <math.h>

typedef struct Vector2D
{
	float x, y;
} Vector2D;

float get_vector_length(Vector2D vector);
float get_vector_angle(Vector2D vector);

#endif
