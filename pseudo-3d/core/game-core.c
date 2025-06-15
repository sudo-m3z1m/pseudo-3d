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

float get_vector_length(Vector2D vector)
{
	float length = sqrt(pow(vector.x, 2) + pow(vector.y, 2));
	return length;
}

float get_vector_rotation(Vector2D vector)
{
	float angle = PI / 2;
	if (vector.x == 0) return angle + PI * (int)(vector.y < 0);
	
	angle = atanf(vector.y / vector.x);
	angle += PI * (int)(vector.x < 0);
	return angle;
}

Vector2D normalize_vector_2d(Vector2D vector)
{
	Vector2D new_vector = (Vector2D){0.0f, 0.0f};
	float vector_len = get_vector_length(vector);
	
	if(vector_len == 0)
		return new_vector;
	
	new_vector.x = vector.x / vector_len;
	new_vector.y = vector.y / vector_len;
	
	return new_vector;
}

Vector2D rotate_vector(Vector2D vector, float rotation)
{
	const float length = get_vector_length(vector);
	const float current_rotation = get_vector_rotation(vector);
	const Vector2D rotated_vector = (Vector2D){length * cosf(current_rotation + rotation), length * sinf(current_rotation + rotation)};
	
	return rotated_vector;
}

void get_resources_path(char* path, char* file_name)
{
	const char* base_path = SDL_GetBasePath();
	snprintf(path, 512, "%s%s", base_path, file_name);
}
