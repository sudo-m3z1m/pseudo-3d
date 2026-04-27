#ifndef SECTORS_INSPECTOR_HPP
#define SECTORS_INSPECTOR_HPP

#include <stdio.h>
#include <vector>

#include "imgui_impl_sdl3.h"
#include "sector.hpp"

class SectorsInspector
{
protected:
	const ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse |
	ImGuiWindowFlags_NoResize |
	ImGuiWindowFlags_NoMove |
	ImGuiWindowFlags_NoTitleBar |
	ImGuiWindowFlags_NoBringToFrontOnFocus |
	ImGuiWindowFlags_NoScrollWithMouse |
	ImGuiWindowFlags_NoScrollbar;
	
	std::vector<Sector>* sectors;
	InspectorItem** current_item;
	float width, height;
	Vector2D<float> screen_position;
	
public:
	SectorsInspector();
	SectorsInspector(std::vector<Sector>* sectors, InspectorItem** item, int width, int height, Vector2D<float> viewport_size);
	~SectorsInspector();
	
	int add_new_sector();
	void remove_sector(int sector_index);
	void pick_sector(int sector_index);
	
	void render();
};

#endif
