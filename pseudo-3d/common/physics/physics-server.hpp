#ifndef PHYSICS_SERVER_HPP
#define PHYSICS_SERVER_HPP

#include <stdio.h>
#include <SDL3/SDL.h>
#include <iostream>
#include <vector>
#include "physics-component/physics-component.hpp"

class PhysicsServer
{
private:
	std::vector<PhysicsComponent*> physics_components;
	
public:
//	PhysicsServer();
	~PhysicsServer();
	void add_physics_component(PhysicsComponent* component);
	void calculate_sector_colliding();
	bool is_components_colliding(PhysicsComponent* first_component, PhysicsComponent* second_component);
	
	void render_physics_components(SDL_Renderer* renderer); //Temp method
};

#endif
