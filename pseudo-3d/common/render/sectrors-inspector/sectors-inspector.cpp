#include "sectors-inspector.hpp"

SectorsInspector::SectorsInspector()
{
	sectors = nullptr;
	current_item = nullptr;
}

SectorsInspector::SectorsInspector(std::vector<Sector>* sectors, InspectorItem* item, int width, int height, Vector2D<float> viewport_size)
{
	this->sectors = sectors;
	this->current_item = item;
	this->width = width;
	this->height = height;
	screen_position = {0, viewport_size.y - height};
}

SectorsInspector::~SectorsInspector()
{
	delete current_item;
	sectors->clear();
}

int SectorsInspector::add_new_sector()
{
	Sector new_sector = Sector();
	sectors->push_back(new_sector);
	
	return (int)sectors->size() - 1;
}

void SectorsInspector::remove_sector(int sector_index)
{
	if(sector_index >= sectors->size()) return;
	sectors->erase(sectors->begin() + sector_index);
}

void SectorsInspector::pick_sector(int sector_index)
{
//	InspectorItem* sector = &sectors[sector_index];
//	current_item = sector;
}

void SectorsInspector::render()
{
	ImVec2 window_pos = {screen_position.x, screen_position.y};
	ImGui::SetNextWindowPos(window_pos);
	ImGui::SetNextWindowSize({width, height});
	
	ImGui::Begin("Inspector menu", NULL, window_flags);
	ImGui::Text(current_item->get_inspector_item_name(), 0);
	ImGui::Separator();
	
	for(int sector_index = 0; sector_index < sectors->size(); sector_index++)
	{
		if(ImGui::Button(std::to_string(sector_index).c_str()))
		{
			pick_sector(sector_index);
		}
	}
	
	ImGui::End();
}
