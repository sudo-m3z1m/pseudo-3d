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

void EditorLevelServer::create_new_shape()
{
	ShapeComponent new_shape = ShapeComponent(POLYGON, 0, {}, {}, 0);
	add_new_polygon(new_shape);
	
	current_shape = &level_polygons[level_polygons.size() - 1];
}

void EditorLevelServer::add_point_to_current_shape(Vector2D<float> point)
{
	std::vector<Vector2D<float>>& shape_points = current_shape->points;
	if (shape_points.size() == 0) return shape_points.push_back(point);
	
	int last_point_index = (int)shape_points.size() - 1;
	int new_point_index = 0;
	if(shape_points[last_point_index] != point)
	{
		shape_points.push_back(point);
		new_point_index = (int)shape_points.size() - 1;
	}
	add_wall_to_current_shape(new_point_index);
}

void EditorLevelServer::add_wall_to_current_shape(int new_point_index)
{
	std::vector<Vector2D<float>>& shape_points = current_shape->points;
	if(shape_points.size() <= 1) return;
	
	int last_point_index = (int)shape_points.size() - 1;
	Vector2D<float> normal = (shape_points[new_point_index] - shape_points[last_point_index]).get_vector_normal();
	Wall new_wall = Wall(last_point_index, new_point_index, normal, nullptr);
	current_shape->walls.push_back(new_wall);
}
