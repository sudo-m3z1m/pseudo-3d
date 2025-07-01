#include "game-core.h"

float get_delta_ticks(void)
{
	float new_ticks = SDL_GetTicks();
	float new_delta = (new_ticks - prev_ticks) * 0.001f;
	prev_ticks = SDL_GetTicks();
	
	delta = new_delta;
	
	return new_delta;
}

float get_delta(void)
{
	return delta;
}

void get_resources_path(char* path, char* file_name)
{
	const char* base_path = SDL_GetBasePath();
	snprintf(path, 512, "%s%s", base_path, file_name);
}
