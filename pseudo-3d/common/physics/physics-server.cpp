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
			
			Vector2D<float> pos = current_shape.points[0];
			Vector2D<float> draw_point = Vector2D<float>(current_shape.radius, 0.0f);
			
			while (angle < 2 * PI)
			{
				draw_point = draw_point.rotate_vector(angle);
				SDL_RenderPoint(renderer, pos.x + draw_point.x, pos.y + draw_point.y);
				
				angle += degree_rad;
			}
			
			continue;
		}
		
		SDL_RenderLine(renderer, current_shape.points[0].x, current_shape.points[0].y, current_shape.points[1].x, current_shape.points[1].y);
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

Vector2D<float> PhysicsServer::get_line_projection_point(PhysicsComponent* dot_component, PhysicsComponent* line_component)
{
	ShapeComponent dot_shape = dot_component->shape, line_shape = line_component->shape;
	
	Vector2D<float> line_vector = line_shape.points[1] - line_shape.points[0]; //Method of global position is awful, need smth better
	Vector2D<float> components_vector = dot_shape.points[0] - line_shape.points[0];
	
	float line_length = powf(line_vector.x, 2) + powf(line_vector.y, 2);
	
	float dot_product = line_vector * components_vector;
	
	const float line_projection_k = std::fminf(line_length, std::fmaxf(0, dot_product)) / line_length;
	Vector2D<float> projection_point = line_shape.points[0] + line_vector * line_projection_k;
	
	return projection_point;
}

void PhysicsServer::calculate_sector_colliding()
{
	const size_t components_size = physics_components.size();
	bool is_colliding = false;
	
	for (size_t f_component_index = 0; f_component_index < components_size; f_component_index++)
	{
		for (size_t s_component_index = 0; s_component_index < components_size; s_component_index++)
		{
			if (f_component_index == s_component_index) continue;
			if (physics_components[f_component_index]->shape.type != CIRCLE || physics_components[s_component_index]->shape.type != LINE) continue;
			
			is_colliding = collide_components(physics_components[f_component_index], physics_components[s_component_index]);
			std::cout << f_component_index << ' ' << s_component_index << " is colliding: " << is_colliding << std::endl;
		}
	}
}

bool PhysicsServer::collide_components(PhysicsComponent* first_component, PhysicsComponent* second_component)
{
	const float circle_radius = first_component->shape.radius;
	
	//TODO: it's not counting physics_component position only shape dotes.
	Vector2D<float> projection_point = get_line_projection_point(first_component, second_component);
	
	float collide_distance = (projection_point - first_component->shape.points[0]).length;
	
	if (collide_distance < circle_radius)
	{
//		Vector2D<float> line_normal = second_component->shape.get_line_normal(0, 1);
		Vector2D<float> line_normal = (first_component->shape.points[0] - projection_point).normalize_vector_2d();
		float knockback_strength = circle_radius - collide_distance;
		first_component->shape.points[0] = first_component->shape.points[0] + (line_normal * knockback_strength);
		
		return true;
	}
	
	return false;
}
