#include "color.hpp"

const Color Color::VERTEX_COLOR(255, 255, 255, 255);
const Color Color::LINE_COLOR(240, 240, 240, 255);
const Color Color::LINE_WINDOW_COLOR(80, 255, 80, 255);
const Color Color::GRID_MAIN_COLOR(80, 80, 80, 255);
const Color Color::GRID_SECONDARY_COLOR(40, 40, 40, 255);
const Color Color::CAMERA_COLOR(255, 253, 27, 255);

Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

Color Color::get_random_color(uint8_t a)
{
	uint8_t r = rand() % 255;
	uint8_t g = rand() % 255;
	uint8_t b = rand() % 255;
	
	return Color(r, g, b, a);
}

bool Color::operator==(Color color)
{
	return ((r == color.r) && (g == color.g) && (b == color.b) && (a == color.a));
}
