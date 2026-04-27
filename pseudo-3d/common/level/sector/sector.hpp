#ifndef SEGMENT_HPP
#define SEGMENT_HPP

#include <stdio.h>
#include <vector>

#include "physics-component.hpp"
#include "inspector-item.hpp"
#include "color.hpp"

class Sector : public InspectorItem
{
public:
	std::vector<PhysicsComponent*> physics_components;
	
	float floor_z, ceiling_z;
	Color floor_color, ceiling_color;
	int floor_tid, ceiling_tid;
//	float light_level;
	
	Sector();
	Sector(float floor_z, float ceiling_z, int floor_tid, int ceiling_tid, Color floor_color, Color ceiling_color);
	~Sector();
	
	std::vector<InspectorItemProperty> get_inspector_item_properties() override;
	const char* get_inspector_item_name() override;
	
	void add_physics_component(PhysicsComponent* physics_component);
};

#endif
