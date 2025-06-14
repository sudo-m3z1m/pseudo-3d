#ifndef PLAYER_H
#define PLAYER_H

#define FOV PI / 3
#define RAYS_COUNT 1024

#include <stdio.h>
#include <stdlib.h>
#include "../core/game-core.h"
#include "../game-core/updatable-component.h"
#include "raycast.h"

typedef struct Player
{
	Vector2D position;
	Vector2D direction;
	UpdatableComponent* updatable_component;
	Raycast** raycasts;
	
	float speed;
	float rotation_speed;
	float rotation;
	float delta_rotation;
} Player;

static void move(Player* player, float delta);

void player_update(float delta, void* player);
static void handle_input_data(Player* player);
Player* initialize_player(float speed, float rotation_speed, Vector2D init_position, float init_rotation);

Raycast** initialize_player_raycasts(void);
static void update_player_raycasts(Player* player);

#endif
