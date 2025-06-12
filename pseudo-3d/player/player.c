#include "player.h"

void player_update(float delta, void* player)
{
	Player* cur_player = (Player*)player;
	
	handle_input_data(cur_player);
	update_player_raycasts(cur_player);
}

static void handle_input_data(Player* player)
{
	Vector2D direction = ZERO_VECTOR_2D;
	float delta_rotation = 0;
	
	SDL_Event event;
	SDL_PollEvent(&event);
	
	if (event.key.key == SDLK_W)
		direction.y += 1;
	if (event.key.key == SDLK_S)
		direction.y -= 1;
	if (event.key.key == SDLK_D)
		direction.x += 1;
	if (event.key.key == SDLK_A)
		direction.x -= 1;
	
	if (event.key.key == SDLK_LEFT)
		delta_rotation -= 1;
	if (event.key.key == SDLK_W)
		delta_rotation += 1;
	
	player->direction = direction;
	player->rotation += delta_rotation;
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

static void update_player_raycasts(Player* player)
{
	Raycast** raycasts = player->raycasts;
	Vector2D player_pos = player->position;
	char map[] = MAP;
	
	for (size_t raycast_index = 0; raycast_index < RAYS_COUNT; raycast_index++)
	{
		get_collision_pos(player_pos, &raycasts[raycast_index], map);
	}
}
