#include "texture-buffer.hpp"

TextureBuffer::TextureBuffer()
{
	buffer_size = 0;
	buffer = std::vector<SDL_Surface*>(buffer_size);
}

TextureBuffer::TextureBuffer(int buffer_size)
{
	this->buffer_size = buffer_size;
	buffer = std::vector<SDL_Surface*>(buffer_size);
}

TextureBuffer::~TextureBuffer()
{
	buffer.clear();
	//TODO: free_surface
}

std::vector<std::string> TextureBuffer::get_textures_names()
{
	return names;
}

int TextureBuffer::load_texture(std::string path)
{
	SDL_Surface* new_surface = SDL_LoadBMP(path.c_str());
	buffer.push_back(new_surface);
	names.push_back(path);
	
	int new_tid = (int)buffer.size() - 1;
	buffer_size = (int)buffer.size();
	
	return new_tid;
}

void TextureBuffer::remove_texture(int tid)
{
	buffer.erase(buffer.begin() + tid);
//	free_surface
}

SDL_Surface* TextureBuffer::get_texture_surface(int tid)
{
	if(tid >= buffer.size()) return nullptr;
	return buffer[tid];
}

Color TextureBuffer::get_texture_pixel(int tid, Vector2D<int>& pixel_pos)
{
	Color pixel_color;
	if (tid >= buffer.size()) return pixel_color;
	SDL_Surface* surface = buffer[tid];
	pixel_pos.x %= surface->w;
	pixel_pos.y %= surface->h;
	
	SDL_ReadSurfacePixel(surface, pixel_pos.x, pixel_pos.y, &pixel_color.r, &pixel_color.g, &pixel_color.b, &pixel_color.a);
	return pixel_color;
}
