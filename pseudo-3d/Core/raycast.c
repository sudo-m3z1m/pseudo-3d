#include "raycast.h"

void apply_length_pos(float length, Vector2D* position, Raycast** raycast)
{
	length = SDL_min(length, (*raycast)->max_length);
	
	float raycast_angle = (*raycast)->angle;
	position->x += length * cosf(raycast_angle);
	position->y += length * sinf(raycast_angle);
}
