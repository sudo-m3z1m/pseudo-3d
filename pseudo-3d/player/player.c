#include "player.h"

static void get_input_data(void)
{
	
}

//void get_raycasts_collisions(Player** player, int width, int height) //TODO: Place it to update with rotation updating
//{
//	char map[] = MAP;
//	
//	for (size_t raycast_index = 0; raycast_index < RAYCASTS_COUNT; raycast_index++)
//	{
//		const Vector2D origin = (*player)->position;
//		Raycast* current_raycast = raycasts[raycast_index];
//		
//		Vector2D last_point = (Vector2D){origin.x, origin.y};
//		apply_collision_pos(&last_point, &current_raycast, map);
//	}
//}

void player_update(float delta, void* player)
{
	
}

Player* initialize_player(float speed, Vector2D init_position, float init_rotation)
{
	Player* player = malloc(sizeof(Player));
	
	player->position = init_position;
	player->direction = ZERO_VECTOR_2D;
	
	player->speed = speed;
	player->rotation = init_rotation;
	
	player->updatable_component = malloc(sizeof(UpdatableComponent));
	player->updatable_component->update = &player_update;
	
	player->raycasts = initialize_player_raycasts();
	
	return player;
}

Raycast** initialize_player_raycasts(void)
{
	float start_angle = 0 - FOV / 2;
	float delta_angle = FOV / RAYS_COUNT;
	Raycast** raycasts = malloc(sizeof(Raycast*) * RAYS_COUNT);
	
	for (size_t raycast_index = 0; raycast_index < RAYS_COUNT; raycast_index++)
	{
		float raycast_rotation = start_angle + delta_angle * raycast_index;
		raycasts[raycast_index] = initialize_raycast(raycast_rotation);
	}
	
	return raycasts;
}
