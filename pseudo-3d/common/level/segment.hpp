#ifndef SEGMENT_HPP
#define SEGMENT_HPP

#include <stdio.h>
#include "../physics/physics-component/physics-component.hpp"

class Segment
{
private:
	std::vector<Segment*> connected_segments;

public:
	std::vector<PhysicsComponent*> physics_components;
	
	Segment();
	~Segment();
	
	void add_physics_component(PhysicsComponent* physics_component);
};

#endif
