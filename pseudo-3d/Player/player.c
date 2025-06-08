#include "player.h"

void initialize_player(Player** player, float speed)
{
	*player = malloc(sizeof(Player));
	
	(*player)->position = ZERO_VECTOR_2D;
	(*player)->direction = ZERO_VECTOR_2D;
	
	(*player)->speed = speed;
}
