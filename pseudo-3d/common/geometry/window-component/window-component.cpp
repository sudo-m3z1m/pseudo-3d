#include "window-component.hpp"

WindowComponent::WindowComponent()
{
	f_sector_index = 0;
	s_sector_index = 0;
}

WindowComponent::WindowComponent(int f_sector_index, int s_sector_index)
{
	this->f_sector_index = f_sector_index;
	this->s_sector_index = s_sector_index;
	
	bottom_color = Color().get_random_color(255);
	upper_color = Color().get_random_color(255);
}
