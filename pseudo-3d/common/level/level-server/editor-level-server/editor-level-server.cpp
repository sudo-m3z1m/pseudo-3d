#include "editor-level-server.hpp"


EditorLevelServer::EditorLevelServer() : LevelServer()
{
	player_spawn_pos = Vector2D<float>(0.0f, 0.0f);
}

EditorLevelServer::EditorLevelServer(Vector2D<float> player_spawn_pos) : LevelServer()
{
	this->player_spawn_pos = player_spawn_pos;
}

EditorLevelServer::~EditorLevelServer()
{
	//save in file probably
}

void test(int& test_int)
{
	
}

ShapeComponent* EditorLevelServer::get_closest_shape(Vector2D<float>& point)
{
	ShapeComponent* closest_shape = &level_polygons[0];
	Vector2D<float> shape_center_point = closest_shape->get_center_point();
	float closest_shape_length = (shape_center_point - point).length;
	for(ShapeComponent& shape : level_polygons)
	{
		shape_center_point = shape.get_center_point();
		float to_shape_length = (shape_center_point - point).length;
		if(to_shape_length < closest_shape_length)
		{
			closest_shape_length = to_shape_length;
			closest_shape = &shape;
		}
	}
	
	return closest_shape;
}

Wall* EditorLevelServer::get_closest_shape_wall(Vector2D<float>& point, ShapeComponent* shape)
{
	std::vector<Vector2D<float>> shape_points = shape->points;
	
	Wall* closest_shape_wall = &shape->walls[0];
	std::vector<Vector2D<float>> wall_points = closest_shape_wall->get_wall_points(shape_points);
	float closest_wall_length = (wall_points[0] - point).length;
	
	for(Wall& wall : shape->walls)
	{
		wall_points = wall.get_wall_points(shape_points);
		
		Vector2D<float> points[] = {wall_points[0], wall_points[1]};
		Vector2D<float> wall_projection_point = get_line_projection_point(point, points);
		float to_wall_length = (wall_projection_point - point).length;
		if(to_wall_length < closest_wall_length)
		{
			closest_wall_length = to_wall_length;
			closest_shape_wall = &wall;
		}
	}
	
	return closest_shape_wall;
}

void EditorLevelServer::create_new_shape()
{
	ShapeComponent new_shape = ShapeComponent(POLYGON, 0, {}, {}, 0);
	add_new_polygon(new_shape);
	
	current_shape = &level_polygons[level_polygons.size() - 1];
	current_wall = &level_polygons[0].walls[0];
}


//FIXME: Algorithm is: 1. Find point in shape. 2. If already have - get index. 3. Create wall with index. 4. If point isn't new - deselect tool.
void EditorLevelServer::add_point_to_current_shape(Vector2D<float> point)
{
	if (current_shape == nullptr) return;
	
	std::vector<Vector2D<float>>& shape_points = current_shape->points;
	if (shape_points.size() == 0) return shape_points.push_back(point);
	
	int last_point_index = (int)shape_points.size() - 1;
	int new_point_index = 0;
	if(shape_points[0] == point) //TODO: Need to create search by points instead of this
	{
		add_wall_to_current_shape(last_point_index, new_point_index);
		current_shape = nullptr;
		return;
	}
	
	shape_points.push_back(point);
	new_point_index = (int)shape_points.size() - 1;
	add_wall_to_current_shape(last_point_index, new_point_index);
}

void EditorLevelServer::add_wall_to_current_shape(int last_point_index, int new_point_index)
{
	std::vector<Vector2D<float>>& shape_points = current_shape->points;
	if(shape_points.size() <= 1) return;

	Vector2D<float> normal = (shape_points[new_point_index] - shape_points[last_point_index]).get_vector_normal();
	Wall new_wall = Wall(last_point_index, new_point_index, normal, nullptr);
	current_shape->walls.push_back(new_wall);
}
