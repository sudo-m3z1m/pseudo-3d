#ifndef WINDOW_COMPONENT_HPP
#define WINDOW_COMPONENT_HPP

#include <stdio.h>
#include <json.hpp>

#include "color.hpp"

using json = nlohmann::json;

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
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(WindowComponent, f_sector_index, s_sector_index);

#endif
