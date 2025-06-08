#include <stdio.h>
#include <stdlib.h>
#include "../Core/game-core.h"

typedef struct Player
{
	Vector2D position;
	Vector2D direction;
	float speed;
	
	float angle;
} Player;

void initialize_player(Player** player, float speed);
