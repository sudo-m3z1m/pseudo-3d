#include "level-server.hpp"

LevelServer::LevelServer()
{
	bsp_tree = nullptr;
}

LevelServer::~LevelServer()
{
	delete bsp_tree;
}

void LevelServer::add_new_polygon(ShapeComponent new_polygon)
{
	if (new_polygon.type == DOT) return;
	
	level_polygons.push_back(new_polygon);
}

void LevelServer::create_bsp_tree()
{
	std::vector<BSPShape> bsp_shapes = generate_bsp_shapes();
	bsp_tree = new BSPNode();
	
	separate_bsp_node(bsp_tree, bsp_shapes);
}


//Returns back to front vector
std::vector<BSPShape> LevelServer::separate_shape_by_line(Line line, BSPShape shape)
{
	std::vector<BSPShape> new_shapes(2);
	
	std::vector<Vector2D<float>> shape_points = shape.points;
	std::vector<Wall> shape_walls = shape.walls;
	
	std::vector<Vector2D<float>> wall_points;
	std::vector<float> points_k(2);
	std::vector<int> points_indeces(2);
	Vector2D<float> intersection_point;
	
	for(Wall current_wall : shape_walls)
	{
		wall_points = current_wall.get_wall_points(shape_points);
		points_k[0] = is_point_on_line(wall_points[0], line);
		points_k[1] = is_point_on_line(wall_points[1], line);
		
		intersection_point = get_line_segment_line_intersection(wall_points[0], wall_points[1], line);
		
		points_indeces[0] = points_k[0] < 0;
		points_indeces[1] = points_k[1] < 0;
		
		if(points_k[0] == 0 || points_k[1] == 0)
		{
			new_shapes[points_indeces[0] + points_indeces[1]].add_new_wall(wall_points[0], wall_points[1], current_wall.normal);
			continue;
		}
		if(points_k[0] != points_k[1])
		{
			new_shapes[points_indeces[0]].add_new_wall(wall_points[0], intersection_point, current_wall.normal);
			new_shapes[points_indeces[1]].add_new_wall(intersection_point, wall_points[1], current_wall.normal);
			continue;
		}
		new_shapes[points_indeces[0]].add_new_wall(wall_points[0], wall_points[1], current_wall.normal);
	}
	
	return new_shapes;
}

void LevelServer::separate_bsp_node(BSPNode* node, std::vector<BSPShape> node_shapes)
{
	if (node_shapes.size() == 1)
	{
		node->shape = node_shapes[0];
		return;
	}
	
	BSPShape current_shape = node_shapes[0];
	
	for (Wall current_wall : current_shape.walls)
	{
		std::vector<BSPShape> front, back;
		std::vector<Vector2D<float>> wall_points = current_wall.get_wall_points(current_shape.points);
		Line separation_line = Line(wall_points[0], wall_points[1]);
		
		sort_shapes(separation_line, &front, &back, node_shapes);
		if (!front.size() || !back.size()) continue;
		
		node_shapes.clear();
		node->front = new BSPNode(); node->back = new BSPNode();
		separate_bsp_node(node->front, front);
		separate_bsp_node(node->back, back);
		break;
	}
}

void LevelServer::sort_shapes(Line line, std::vector<BSPShape>* front, std::vector<BSPShape>* back, std::vector<BSPShape> shapes)
{
	for (BSPShape current_shape : shapes)
	{
		Vector2D<float> point;
		std::vector<Vector2D<float>> shape_points = current_shape.points;
		std::vector<Wall> shape_walls = current_shape.walls;
		
		int points_side_count = 0;
		int neutral_points_count = 0;
		
		for (size_t point_index = 0; point_index < shape_points.size(); point_index++)
		{
			point = shape_points[point_index];
			const float result = is_point_on_line(point, line);
			if(result == 0)
			{
				neutral_points_count++;
				continue;
			}
			points_side_count += int(result > 0) - int(result < 0);
		}
		
		if(abs(points_side_count) + neutral_points_count != shape_points.size())
		{
//			std::vector<Vector2D<float>>* new_shape_points = separate_shape_by_line(shape_points, line);
//			BSPShape back_shape = BSPShape(new_shape_points[0]);
//			BSPShape front_shape = BSPShape(new_shape_points[1]); TODO: need to make it with walls

//			delete[] new_shape_points;
			
			std::vector<BSPShape> new_shapes = separate_shape_by_line(line, current_shape); //Works only for convex shapes
			
			back->push_back(new_shapes[0]);
			back->push_back(new_shapes[1]);
			
			continue;
		}
		if (points_side_count > 0) back->push_back(current_shape);
		if (points_side_count < 0) front->push_back(current_shape);
	}
}


std::vector<BSPShape> LevelServer::generate_bsp_shapes()
{
	std::vector<BSPShape> bsp_shapes;
	for(size_t shape_index = 0; shape_index < level_polygons.size(); shape_index++)
	{
		BSPShape new_bsp_shape = BSPShape(&level_polygons[shape_index]);
		bsp_shapes.push_back(new_bsp_shape);
	}
	return bsp_shapes;
}
