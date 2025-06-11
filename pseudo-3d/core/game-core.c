#include "game-core.h"

float get_delta_ticks(void)
{
	float new_ticks = SDL_GetTicks();
	float delta = new_ticks - prev_ticks;
	prev_ticks = new_ticks;
	
	return delta;
}

float get_vector_length(Vector2D vector)
{
	float length = sqrt(pow(vector.x, 2) + pow(vector.y, 2));
	return length;
}

float get_vector_angle(Vector2D vector)
{
	float angle = PI / 2;
	if (vector.x == 0) return angle + PI * (int)(vector.y < 0);
	
	angle = atan(vector.y / vector.x);
	return angle;
}

Vector2D normalize_vector_2d(Vector2D vector)
{
	Vector2D new_vector;
	float vector_len = get_vector_length(vector);
	
	new_vector.x = vector.x / vector_len;
	new_vector.y = vector.y / vector_len;
	
	return new_vector;
}
