#define PI 3.14
#define ZERO_VECTOR_2D ((Vector2D){0.0, 0.0})

#include <stdio.h>

typedef struct Vector2D
{
	float x, y;
} Vector2D;

float get_vector_length(Vector2D vector);
float get_vector_angle(Vector2D vector);
