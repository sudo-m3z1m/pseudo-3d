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
		
		Vector2D<float> component_position = component->position;
		
		//TODO: Bad lines rendering
		for (size_t current_point_index = 0; current_point_index < current_shape.points.size(); current_point_index++)
		{
			size_t next_point_index = (current_point_index + 1) % current_shape.points.size();
			
			Vector2D<float> current_point = component_position + current_shape.points[current_point_index];
			Vector2D<float> next_point = component_position + current_shape.points[next_point_index];
			
			SDL_RenderLine(renderer, current_point.x, current_point.y, next_point.x, next_point.y);
		}
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
	
	ShapeComponent f_shape_component = f_component->shape, s_shape_component = s_component->shape;
	const int collision_index = (f_shape_component.type * f_shape_component.type) + (s_shape_component.type * s_shape_component.type);
	
	switch (collision_index)
	{
		case CIRCLE_CIRCLE:
			collision_vector = get_circle_circle_collision_vector(f_component, s_component);
			break;
		case CIRCLE_LINE:
		case CIRCLE_RECTANGE:
		case CIRCLE_POLYGON:
			collision_vector = get_circle_line_collision_vector(f_component, s_component);
			break;
		case RECTANGLE_RECTANGLE:
			collision_vector = get_rect_rect_collision_vector(f_component, s_component);
			break;
		default:
			collision_vector = get_polygon_polygon_collision_vector(f_component, s_component);
			break;
	}
	f_component->position += collision_vector;
}

Vector2D<float> PhysicsServer::get_circle_circle_collision_vector(PhysicsComponent* f_component, PhysicsComponent* s_component)
{
	Vector2D<float> f_position = f_component->position, s_position = s_component->position;
	float f_radius = f_component->shape.radius, s_radius = s_component->shape.radius;
	
	Vector2D<float> circles_vector = s_position - f_position;
	if (circles_vector.length - f_radius >= s_radius) return Vector2D<float>(); //TODO: Stupid condition. Need to make displacement earlier
	
	float dispacement = circles_vector.length - f_radius - s_radius;
	
	circles_vector = circles_vector.normalize_vector_2d() * dispacement;
	
	return circles_vector;
}

Vector2D<float> PhysicsServer::get_circle_line_collision_vector(PhysicsComponent* f_component, PhysicsComponent* s_component)
{
	Vector2D<float> collision_vector = Vector2D<float>();
	const float circle_radius = f_component->shape.radius;
	
	Vector2D<float> circle_position = f_component->position + f_component->shape.points[0];
	
	ShapeComponent s_shape = s_component->shape;
	Vector2D<float> s_component_position = s_component->position;
	const size_t points_count = s_shape.points.size() - (1 * (int)(s_shape.type == LINE));
	for (size_t f_point_index = 0; f_point_index < points_count; f_point_index++)
	{
		const size_t s_point_index = (f_point_index + 1) % s_shape.points.size();
		
		Vector2D<float> line_points[] = {s_component_position + s_shape.points[f_point_index], s_component_position + s_shape.points[s_point_index]};
		Vector2D<float> projection_point = get_line_projection_point(circle_position, line_points);
		
		Vector2D<float> to_projection_vector = projection_point - circle_position;
		
		if (to_projection_vector.length >= circle_radius) continue;
		
		Vector2D<float> line_normal = s_shape.get_line_normal((int)f_point_index, (int)s_point_index);
		const float normal_direction = line_normal * to_projection_vector;
		line_normal = line_normal * -get_sign(normal_direction);
		
		collision_vector += line_normal * (circle_radius - to_projection_vector.length);
	}
	
	return collision_vector;
}

Vector2D<float> PhysicsServer::get_rect_rect_collision_vector(PhysicsComponent* f_component, PhysicsComponent* s_component)
{
	return Vector2D<float>();
}

Vector2D<float> PhysicsServer::get_polygon_polygon_collision_vector(PhysicsComponent* f_component, PhysicsComponent* s_component)
{
	return Vector2D<float>();
}
