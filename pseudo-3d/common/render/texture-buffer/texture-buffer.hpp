#ifndef TEXTURE_BUFFER_HPP
#define TEXTURE_BUFFER_HPP

#include <stdio.h>
#include <SDL3/SDL.h>
#include <vector>
#include <string>

#include "vector2D.hpp"
#include "color.hpp"

class TextureBuffer
{
private:
	std::vector<SDL_Surface*> buffer;
	int buffer_size;
	
public:
	TextureBuffer();
	TextureBuffer(int buffer_size);
	~TextureBuffer();
	
	int load_texture(char* path);
	void remove_texture(int tid);
	SDL_Surface* get_texture_surface(int tid);
	Color get_texture_pixel(int tid, Vector2D<int>& pixel_pos);
};

#endif
