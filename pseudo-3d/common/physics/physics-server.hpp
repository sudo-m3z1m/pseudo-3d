#ifndef PHYSICS_SERVER_HPP
#define PHYSICS_SERVER_HPP

#include <stdio.h>
#include <SDL3/SDL.h>
#include <iostream>
#include <vector>
#include "../math/math.hpp"
#include "physics-component/physics-component.hpp"

enum COLLISION_TYPES
{
	CIRCLE_CIRCLE = 2,
	CIRCLE_LINE = 5,
	RECTANGLE_RECTANGLE = 18
	//And Shape - Shape but it's all what is not other
};

class PhysicsServer
{
private:
	std::vector<PhysicsComponent*> physics_components;
	
public:
	~PhysicsServer();
	void add_physics_component(PhysicsComponent* component);
	
	void calculate_sector_colliding();
	void resolve_collision(PhysicsComponent* f_component, PhysicsComponent* s_component);
	
	Vector2D<float> get_circle_circle_collision_vector(PhysicsComponent* f_component, PhysicsComponent* s_component);
	Vector2D<float> get_circle_line_collision_vector(PhysicsComponent* f_component, PhysicsComponent* s_component);
	Vector2D<float> get_rect_rect_collision_vector(PhysicsComponent* f_component, PhysicsComponent* s_component);
	Vector2D<float> get_shape_shape_collision_vector(PhysicsComponent* f_component, PhysicsComponent* s_component);
	
//	bool collide_components(PhysicsComponent* first_component, PhysicsComponent* second_component);
	void render_physics_components(SDL_Renderer* renderer); //Temp method
};

#endif
