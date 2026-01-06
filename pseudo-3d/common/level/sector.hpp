#ifndef SEGMENT_HPP
#define SEGMENT_HPP

#include <stdio.h>
#include "../physics/physics-component/physics-component.hpp"

class Sector
{
private:
	std::vector<Sector*> connected_segments;

public:
	std::vector<PhysicsComponent*> physics_components;
	
	Sector();
	~Sector();
	
	void add_physics_component(PhysicsComponent* physics_component);
};

#endif
