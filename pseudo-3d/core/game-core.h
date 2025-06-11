#ifndef GAME_CORE_H
#define GAME_CORE_H

//#define SDL_MAIN_USE_CALLBACKS 1
#define PI 3.141592
#define ZERO_VECTOR_2D ((Vector2D){0.0, 0.0})
#define ONE_VECTOR_2D ((Vector2D){1.0, 1.0})
#define X_VECTOR_2D ((Vector2D){1.0, 0.0})
#define Y_VECTOR_2D ((Vector2D){0.0, 1.0})

#define MAP {\
			'0', '0', '1', ' ', ' ', ' ', ' ', ' ', \
			'0', ' ', '1', '1', '1', '1', ' ', ' ', \
			'0', ' ', ' ', ' ', ' ', ' ', '0', '0', \
			'0', ' ', ' ', ' ', ' ', ' ', ' ', '0', \
			'0', ' ', ' ', ' ', ' ', ' ', ' ', '0', \
			'0', '0', ' ', '0', '0', ' ', '1', '0', \
			' ', '1', ' ', '1', '0', ' ', '1', ' ', \
			' ', '1', ' ', '1', '0', '1', '1', ' '}
#define MAP_SIZE 8

#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <SDL3/SDL.h>
//#include <SDL3/SDL_main.h>

static float prev_ticks = 0;

typedef struct Vector2D
{
	float x, y;
} Vector2D;

typedef struct Color
{
	short r, g, b, a;
} Color;

float get_delta_ticks(void);
float get_vector_length(Vector2D vector);
float get_vector_angle(Vector2D vector);

Vector2D normalize_vector_2d(Vector2D vector);

#endif
