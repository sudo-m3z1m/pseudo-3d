#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>
#include <stdlib.h>
#include "../core/game-core.h"
#include "../game-core/updatable-component.h"
#endif

typedef struct Player
{
	Vector2D position;
	Vector2D direction;
	UpdatableComponent* updatable_component;
	float speed;
	
	float rotation;
} Player;

static void update(float delta);
static void get_input_data();
void initialize_player(Player** player, float speed, Vector2D init_position);
