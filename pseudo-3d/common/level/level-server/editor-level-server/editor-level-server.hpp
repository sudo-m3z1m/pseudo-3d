#ifndef EDITOR_LEVEL_SERVER_HPP
#define EDITOR_LEVEL_SERVER_HPP

#include <stdio.h>

#include "level-server.hpp"

//TODO: Need to create flexible tools.
//TODO: Need to make walls adding more flexible.

struct PickRequest
{
	InspectorItem* item;
	float distance;
};

class EditorLevelServer : public LevelServer
{
protected:
	Vector2D<float> player_spawn_pos;
	
public:
	ShapeComponent* current_shape;
	
	EditorLevelServer();
	EditorLevelServer(Vector2D<float> player_spawn_pos);
	~EditorLevelServer() override;
	
	PickRequest get_closest_shape(Vector2D<float>& point);
	PickRequest get_closest_shape_wall(Vector2D<float>& point);
//	Vector2D<float>* get_closest_shape_point(Vector2D<float>& point, ShapeComponent* shape);
	PickRequest get_closest_camera(Vector2D<float>& point);
	
	Camera* create_new_camera();
	void create_new_shape();
	void add_point_to_current_shape(Vector2D<float> point);
	void add_wall_to_current_shape(int last_point_index, int new_point_index);
};

#endif
