#ifndef RENDERER_HPP
#define RENDERER_HPP
	
#include <stdio.h>
#include <SDL3/SDL.h>

#include "color.hpp"
#include "camera.hpp"
#include "texture-buffer.hpp"

class Renderer
{
protected:
	Camera* current_camera;
	SDL_Renderer* application_renderer;
	SDL_Window* application_window;
	
	SDL_Surface* color_buffer;
	TextureBuffer* texture_buffer;
	
	int screen_width, screen_height;
	
public:
	Renderer();
	Renderer(Camera* camera, TextureBuffer* texture_buffer, int width, int height);
	virtual ~Renderer();
	
	virtual void render();
	void draw_pixel_in_buffer(Vector2D<int> draw_pos, Color color);
	virtual void render_buffer();
};

#endif
