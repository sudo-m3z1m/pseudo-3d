#include "physics-server.hpp"

void PhysicsServer::render_physics_components(SDL_Renderer* renderer)
{
	ShapeComponent current_shape;
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	for (PhysicsComponent* component : physics_components)
	{
		current_shape = component->shape;
		
		if (current_shape.type == CIRCLE)
		{
			const float resolution = 480;
			const float degree_rad = (2 * PI) / resolution;
			float angle = 0;
			
			Vector2D<float> pos = component->position + current_shape.points[0];
			Vector2D<float> draw_point = Vector2D<float>(current_shape.radius, 0.0f);
			
			while (angle < 2 * PI)
			{
				draw_point = draw_point.rotate_vector(angle);
				SDL_RenderPoint(renderer, pos.x + draw_point.x, pos.y + draw_point.y);
				
				angle += degree_rad;
			}
			
			continue;
		}
		
		const Vector2D<float> f_point = component->position + current_shape.points[0];
		const Vector2D<float> s_point = component->position + current_shape.points[1];
		
		SDL_RenderLine(renderer, f_point.x, f_point.y, s_point.x, s_point.y);
	}
}

PhysicsServer::~PhysicsServer()
{
	const size_t components_size = physics_components.size();
	
	for (size_t component_index = 0; component_index < components_size; component_index++)
	{
		delete physics_components[component_index];
	}
}

void PhysicsServer::add_physics_component(PhysicsComponent* component)
{
	if (component->shape.type == DOT) return;
	physics_components.push_back(component);
}

void PhysicsServer::calculate_sector_colliding()
{
	const size_t components_size = physics_components.size();
	
	for (size_t f_component_index = 0; f_component_index < components_size; f_component_index++)
	{
		for (size_t s_component_index = f_component_index; s_component_index < components_size; s_component_index++)
		{
			if (f_component_index == s_component_index) continue;
			
			resolve_collision(physics_components[f_component_index], physics_components[s_component_index]);
		}
	}
}

void PhysicsServer::resolve_collision(PhysicsComponent* f_component, PhysicsComponent* s_component)
{
	Vector2D<float> collision_vector = Vector2D<float>();
	
	switch (f_component->shape.type)
	{
		case CIRCLE:
			collision_vector = get_circle_collision_vector(f_component, s_component);
			break;
			
		default:
			break;
	}
	f_component->position += collision_vector;
}

Vector2D<float> PhysicsServer::get_circle_collision_vector(PhysicsComponent* f_component, PhysicsComponent* s_component)
{
	Vector2D<float> collision_vector = Vector2D<float>();
	const float circle_radius = f_component->shape.radius;
	
	Vector2D<float> circle_position = f_component->position + f_component->shape.points[0];
	Vector2D<float> line_points[] = {s_component->position + s_component->shape.points[0], s_component->position + s_component->shape.points[1]};
	Vector2D<float> projection_point = get_line_projection_point(circle_position, line_points);
	
	Vector2D<float> to_projection_vector = projection_point - circle_position;
	
	if (to_projection_vector.length < circle_radius)
	{
		Vector2D<float> line_normal = s_component->shape.get_line_normal(0, 1);
		const float normal_direction = line_normal * to_projection_vector;
		line_normal = line_normal * -get_sign(normal_direction);
		
		collision_vector = line_normal * (circle_radius - to_projection_vector.length);
	}
	
	return collision_vector;
}
