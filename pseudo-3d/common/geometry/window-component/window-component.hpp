#ifndef WINDOW_COMPONENT_HPP
#define WINDOW_COMPONENT_HPP

#include <stdio.h>

#include "color.hpp"

class WindowComponent
{
public:
	int f_sector_index;
	int s_sector_index;
	Color bottom_color;
	Color upper_color;
	
	WindowComponent();
	WindowComponent(int f_sector_index, int s_sector_index);
};

#endif
