#ifndef PHYSICS_SERVER_HPP
#define PHYSICS_SERVER_HPP

#include <stdio.h>
#include <SDL3/SDL.h>
#include <iostream>
#include <vector>
#include "../math/math.hpp"
#include "physics-component/physics-component.hpp"

class PhysicsServer
{
private:
	std::vector<PhysicsComponent*> physics_components;
	
public:
	~PhysicsServer();
	void add_physics_component(PhysicsComponent* component);
	
	void calculate_sector_colliding();
	void resolve_collision(PhysicsComponent* f_component, PhysicsComponent* s_component);
	Vector2D<float> get_circle_collision_vector(PhysicsComponent* f_component, PhysicsComponent* s_component);
	
//	bool collide_components(PhysicsComponent* first_component, PhysicsComponent* second_component);
	void render_physics_components(SDL_Renderer* renderer); //Temp method
};

#endif
