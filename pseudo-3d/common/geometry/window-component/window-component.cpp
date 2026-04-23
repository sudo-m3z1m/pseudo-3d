#include "window-component.hpp"

WindowComponent::WindowComponent()
{
	f_sector_index = 0;
	s_sector_index = 0;
	
	bottom_tid = upper_tid = 0;
}

WindowComponent::WindowComponent(int f_sector_index, int s_sector_index, int bottom_tid, int upper_tid)
{
	this->f_sector_index = f_sector_index;
	this->s_sector_index = s_sector_index;
	this->bottom_tid = bottom_tid;
	this->upper_tid = upper_tid;
}
