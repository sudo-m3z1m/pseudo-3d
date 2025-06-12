#include "raycast.h"

Vector2D get_collision_pos(Vector2D position, Raycast** raycast, float player_rotation, char* map) //TODO: Need to call it from raycast_update method
{
	float current_length = 1;
	float max_length = (*raycast)->max_length;
	float raycast_rotation = player_rotation + (*raycast)->rotation;
	Vector2D global_pos = X_VECTOR_2D;
	
	while (current_length < max_length)
	{
		const Vector2D last_pos = (Vector2D){current_length * cosf(raycast_rotation), current_length * sinf(raycast_rotation)};
		global_pos.x = position.x + last_pos.x;
		global_pos.y = position.y + last_pos.y;
		
		if (global_pos.x > MAP_SIZE || global_pos.y > MAP_SIZE || global_pos.x < 0 || global_pos.y < 0)
			break;
		
		const int map_index = ((int)global_pos.y * MAP_SIZE) + (int)global_pos.x;
		if (map[map_index] != ' ')
		{
			(*raycast)->collided = true;
			(*raycast)->length = current_length;
			(*raycast)->target_pos = global_pos;
			
			return global_pos;
		}
		current_length += 0.01;
	}
	
	(*raycast)->collided = false;
	(*raycast)->length = current_length;
	(*raycast)->target_pos = global_pos;
	
	return global_pos;
}

Raycast* initialize_raycast(float rotation)
{
	Raycast* new_raycast = malloc(sizeof(Raycast));
	
	new_raycast->target_pos = X_VECTOR_2D;
	new_raycast->rotation = rotation;
	new_raycast->length = 1;
	new_raycast->max_length = MAP_SIZE; //Need to make it less
	new_raycast->collided = false;
	
	return new_raycast;
}
