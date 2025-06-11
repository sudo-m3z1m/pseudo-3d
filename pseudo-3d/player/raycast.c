#include "raycast.h"

void apply_collision_pos(Vector2D* position, Raycast** raycast, char* map)
{
	float current_length = 1;
	float max_length = (*raycast)->max_length;
	float raycast_angle = (*raycast)->angle;
	
	while (current_length < max_length)
	{
		const float last_position_x = current_length * cosf(raycast_angle);
		const float last_position_y = current_length * sinf(raycast_angle);
		
		const float global_position_x = position->x + last_position_x;
		const float global_position_y = position->y + last_position_y;
		
		if (global_position_x > MAP_SIZE || global_position_y > MAP_SIZE || global_position_x < 0 || global_position_y < 0)
			break;
		
		const int map_index = ((int)global_position_y * MAP_SIZE) + (int)global_position_x;
		if (map[map_index] != ' ')
		{
			Color new_color = (Color){255, 0, 255, 255};
			if (map[map_index] == '1') new_color = (Color){0, 255, 72, 255};
			
			(*raycast)->collided = true;
			position->x = global_position_x;
			position->y = global_position_y;
			(*raycast)->length = current_length;
			(*raycast)->color = new_color;
			return;
		}
		current_length += 0.01;
	}
	
	(*raycast)->collided = false;
	(*raycast)->length = current_length;
	position->x += current_length * cosf(raycast_angle);
	position->y += current_length * sinf(raycast_angle);
}

static void initialize_raycast(Raycast** raycast, float angle)
{
	Raycast* new_raycast = malloc(sizeof(Raycast));
	new_raycast->angle = angle;
	new_raycast->max_length = MAP_SIZE; //Need to make it less
	new_raycast->collided = false;
	
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
