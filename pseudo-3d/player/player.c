#include "player.h"

void collisions_update(void* player)
{
	Player* current_player = (Player*)player;
	const float collision_radius = current_player->collision_component->collision_radius;
	char map[] = MAP;
	
	const Vector2D player_pos = current_player->position;
	Vector2D cell_vector = (Vector2D){1, 0};
	Vector2DI next_cell_pos;
	int map_index;
	Vector2D collision_direction = ZERO_VECTOR_2D;
	
	for (size_t side_num = 0; side_num < 4; side_num++)
	{
		next_cell_pos.x = player_pos.x + cell_vector.x * collision_radius;
		next_cell_pos.y = player_pos.y + cell_vector.y * collision_radius;
		
		printf("coll cell pos:(%d, %d), player pos: (%f, %f);\n", next_cell_pos.x, next_cell_pos.y, player_pos.x, player_pos.y);
		map_index = next_cell_pos.y * MAP_SIZE + next_cell_pos.x;
		cell_vector = rotate_vector(cell_vector, PI / 2);
		
		if (map[map_index] == ' ') continue;
		collision_direction.x += player_pos.x - (next_cell_pos.x + 0.5);
		collision_direction.y += player_pos.y - (next_cell_pos.y + 0.5);
	}
	collision_direction = normalize_vector_2d(collision_direction);
	current_player->direction.x += collision_direction.x * 5;
	current_player->direction.y += collision_direction.y * 5;
}

void player_update(float delta, void* player)
{
	Player* cur_player = (Player*)player;
	
	handle_input_data(cur_player);
	cur_player->collision_component->update(cur_player);
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
	
	player->collision_component = initialize_collision_component(0.4, &collisions_update);
	
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
