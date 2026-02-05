#include "color.hpp"

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
