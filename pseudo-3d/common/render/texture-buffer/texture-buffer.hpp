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
	std::vector<std::string> names;
	int buffer_size;
	
public:
	TextureBuffer();
	TextureBuffer(int buffer_size);
	~TextureBuffer();
	
	std::vector<std::string> get_textures_names();
	int load_texture(std::string path);
	void remove_texture(int tid);
	SDL_Surface* get_texture_surface(int tid);
	Color get_texture_pixel(int tid, Vector2D<int>& pixel_pos);
	
	Color get_texture_pixel_by_percent(float percent);
};

#endif
