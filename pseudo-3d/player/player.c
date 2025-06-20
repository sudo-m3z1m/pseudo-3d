#include "player.h"

void collisions_update(void* player)
{
	
}

void player_update(float delta, void* player)
{
	Player* cur_player = (Player*)player;
	
	handle_input_data(cur_player);
	move(cur_player, delta);
	update_player_raycasts(cur_player);
}

static void move(Player* player, float delta)
{
	Vector2D direction = player->direction;
	const float speed = player->speed;
	const float rotation_speed = player->rotation_speed;
	
	direction = normalize_vector_2d(direction);
	direction.x *= speed * delta;
	direction.y *= speed * delta;
	
	player->rotation += player->delta_rotation * rotation_speed * delta; //TODO: Need to make it separatly
	player->position.x += direction.x;
	player->position.y += direction.y;
}

static void handle_input_data(Player* player)
{
	Vector2D direction = ZERO_VECTOR_2D;
	float delta_rotation = 0;
	
	const bool* keys = SDL_GetKeyboardState(NULL);
	
	if (keys[SDL_SCANCODE_W])
		direction.x += 1;
	if (keys[SDL_SCANCODE_S])
		direction.x -= 1;
	if (keys[SDL_SCANCODE_D])
		direction.y += 1;
	if (keys[SDL_SCANCODE_A])
		direction.y -= 1;
	
	if (keys[SDL_SCANCODE_LEFT])
		delta_rotation -= 1;
	if (keys[SDL_SCANCODE_RIGHT])
		delta_rotation += 1;
	
	player->direction.x = direction.x; player->direction.y = direction.y;
	player->direction = rotate_vector(direction, player->rotation);
	player->delta_rotation = delta_rotation;
}

Player* initialize_player(float speed, float rotation_speed, Vector2D init_position, float init_rotation)
{
	Player* player = malloc(sizeof(Player));
	
	player->position = init_position;
	player->direction = ZERO_VECTOR_2D;
	
	player->speed = speed;
	player->rotation_speed = rotation_speed;
	player->rotation = init_rotation;
	
	player->collision_component = initialize_collision_component(&collisions_update);
	
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
		get_collision_pos(player_pos, &raycasts[raycast_index], player->rotation, map);
	}
}
