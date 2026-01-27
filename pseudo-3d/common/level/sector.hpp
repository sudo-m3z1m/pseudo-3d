#ifndef SEGMENT_HPP
#define SEGMENT_HPP

#include <stdio.h>
#include "physics-component.hpp"

class Sector
{
public:
	std::vector<PhysicsComponent*> physics_components;
	
	float floor_z, ceiling_z;
//	float light_level;
//	int some_textures_top_and_bottom_indeces;
	
	Sector();
	Sector(float floor_z, float ceiling_z);
	~Sector();
	
	void add_physics_component(PhysicsComponent* physics_component);
};

#endif
