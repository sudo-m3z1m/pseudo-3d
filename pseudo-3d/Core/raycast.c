#include "raycast.h"

void apply_length_pos(float length, Vector2D* position, Raycast** raycast)
{
	length = SDL_min(length, (*raycast)->max_length);
	
	float raycast_angle = (*raycast)->angle;
	position->x += length * cosf(raycast_angle);
	position->y += length * sinf(raycast_angle);
}

static void initialize_raycast(Raycast** raycast, float angle)
{
	Raycast* new_raycast = malloc(sizeof(raycast));
	new_raycast->angle = angle;
	new_raycast->max_length = LEVEL_SIZE * 15;
	
	*raycast = new_raycast;
}

void initialize_raycasts(Raycast** raycasts[], float spread_angle, size_t raycasts_num)
{
	float start_angle = 0 - spread_angle / 2;
	float delta_angle = spread_angle / raycasts_num;
	*raycasts = malloc(sizeof(Raycast*) * raycasts_num);
	
	for (size_t raycast_index = 0; raycast_index < raycasts_num; raycast_index++)
	{
		float raycast_angle = start_angle + delta_angle * raycast_index;
		
		initialize_raycast(&(*raycasts)[raycast_index], raycast_angle);
	}
}

static void update_raycast(Raycast** raycast, float delta_angle)
{
	Raycast* current_raycast = *raycast;
	current_raycast->angle += delta_angle;
}

void update_raycasts(Raycast** raycasts[], float delta_angle, size_t raycasts_num)
{
	for (size_t raycast_index = 0; raycast_index < raycasts_num; raycast_index++)
	{
		update_raycast(&(*raycasts)[raycast_index], delta_angle);
	}
}
