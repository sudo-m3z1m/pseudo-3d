#include "player.h"

static void update(float delta)
{
	
}

void initialize_player(Player** player, float speed, Vector2D init_position)
{
	*player = malloc(sizeof(Player));
	
	(*player)->position = init_position;
	(*player)->direction = ZERO_VECTOR_2D;
	
	(*player)->speed = speed;
}
