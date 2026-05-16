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

PickRequest EditorLevelServer::get_closest_shape(Vector2D<float>& point)
{
	PickRequest shape_request = {nullptr, 0};
	for(ShapeComponent& shape : level_polygons)
	{
		bool inside = false;
		std::vector<Vector2D<float>> shape_points = shape.points;
		
		for(Wall& wall : shape.walls)
		{
			std::vector<Vector2D<float>> vertices = wall.get_wall_points(shape_points);
			if (((vertices[1].y > point.y) != (vertices[0].y > point.y)) &&
				(point.x < (vertices[0].x - vertices[1].x) * (point.y - vertices[1].y) /
				 (vertices[0].y - vertices[1].y) + vertices[1].x))
			{
				inside = !inside;
			}
		}

		if(inside)
		{
			InspectorItem* closest_item = dynamic_cast<InspectorItem*>(&shape);
			shape_request = {closest_item, 0};
		}
	}
	
	return shape_request;
}

PickRequest EditorLevelServer::get_closest_shape_wall(Vector2D<float>& point)
{
	Wall* closest_shape_wall = nullptr;
	std::vector<Vector2D<float>> shape_points;
	std::vector<Vector2D<float>> wall_points;
	
	float closest_wall_length = 0.0f;
	
	for(ShapeComponent& shape : level_polygons)
	{
		shape_points = shape.points;
		for(Wall& wall : shape.walls)
		{
			wall_points = wall.get_wall_points(shape_points);
			Vector2D<float> points[] = {wall_points[0], wall_points[1]};
			
			Vector2D<float> wall_projection_point = get_line_projection_point(point, points);
			float to_wall_length = ((wall_projection_point + wall.normal * 0.01) - point).length;
			if((to_wall_length < closest_wall_length) || closest_wall_length == 0.0f)
			{
				closest_wall_length = to_wall_length;
				closest_shape_wall = &wall;
			}
		}
	}
	
	InspectorItem* closest_item = closest_shape_wall;
	PickRequest wall_request = {closest_item, closest_wall_length};
	
	return wall_request;
}

//Vector2D<float>* EditorLevelServerget_closest_shape_point(Vector2D<float>& point, ShapeComponent* shape);

PickRequest EditorLevelServer::get_closest_camera(Vector2D<float>& point)
{
	Camera* closest_camera = nullptr;
	float closest_camera_length = 0.0f;
	
	for (Camera*& camera : cameras)
	{
		float to_camera_len = (camera->position - point).length;
		if((to_camera_len < closest_camera_length) || closest_camera_length == 0.0f)
		{
			closest_camera_length = to_camera_len;
			closest_camera = camera;
		}
	}
	
	InspectorItem* closest_item = closest_camera;
	PickRequest shape_request = {closest_item, closest_camera_length};
	
	return shape_request;
}

Camera* EditorLevelServer::create_new_camera()
{
	Camera* new_camera = new Camera();
	add_camera(new_camera);
	
	return new_camera;
}

void EditorLevelServer::create_new_shape()
{
	ShapeComponent new_shape = ShapeComponent(POLYGON, 0, {}, {}, 0);
	add_new_polygon(new_shape);
	
	current_shape = &level_polygons[level_polygons.size() - 1];
}


//FIXME: Algorithm is: 1. Find point in shape. 2. If already have - get index. 3. Create wall with index. 4. If point isn't new - deselect tool.
void EditorLevelServer::add_point_to_current_shape(Vector2D<float> point)
{	
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
	Wall new_wall = Wall(last_point_index, new_point_index, normal, nullptr, 0);
	current_shape->walls.push_back(new_wall);
}
