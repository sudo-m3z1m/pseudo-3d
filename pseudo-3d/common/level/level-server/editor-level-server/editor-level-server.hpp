#ifndef EDITOR_LEVEL_SERVER_HPP
#define EDITOR_LEVEL_SERVER_HPP

#include <stdio.h>

#include "level-server.hpp"

class EditorLevelServer : public LevelServer
{
protected:
	ShapeComponent* current_shape;
	Vector2D<float> player_spawn_pos;
	
public:
	EditorLevelServer();
	EditorLevelServer(Vector2D<float> player_spawn_pos);
	~EditorLevelServer() override;
	
	void create_new_shape();
	void add_point_to_current_shape(Vector2D<float> point);
	void add_wall_to_current_shape(int new_point_index);
};

#endif
