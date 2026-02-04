#ifndef COLOR_HPP
#define COLOR_HPP

#include <stdint.h>
#include <stdio.h>
#include <cstdlib>

class Color
{
public:
	uint8_t r, g, b, a;
	Color(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0, uint8_t a = 1);
	
	Color get_random_color(uint8_t a);
};

#endif
