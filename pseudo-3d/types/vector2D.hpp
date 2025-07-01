#ifndef VECTOR2D
#define VECTOR2D

#include <stdio.h>
#include "../core/game-core.h"

template <typename T>
struct Vector2D
{
public:
	T x, y;
	
	Vector2D();
	Vector2D(T x, T y);
	
	float get_vector_length();
	float get_vector_rotation();
	Vector2D<T> rotate_vector(float rotation);
	
	Vector2D<T> normalize_vector_2d();
};

#endif
