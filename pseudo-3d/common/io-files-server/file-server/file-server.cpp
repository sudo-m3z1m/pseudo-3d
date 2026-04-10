#include "file-server.hpp"

FileServer::FileServer()
{
	
}

FileServer::FileServer(LevelServer* level_server, std::vector<const char*> textures_paths)
{
	
}

FileServer::~FileServer()
{
	
}

void FileServer::write_file(const char* file_path)
{
	std::ofstream file(file_path, std::ios::binary);
	
	if (!file) return;
	
//	file.write(file_shapes, sizeof(file_shapes));
}

void FileServer::read_file(const char* file_path)
{
	std::ifstream file(file_path, std::ios::binary);
}

void FileServer::write_shapes(std::vector<ShapeComponent>& shapes, std::ofstream& file)
{
	file_shapes = shapes;
	
	uint32_t points_size;
	uint32_t walls_size;
	uint32_t shapes_size = static_cast<uint32_t>(shapes.size());
	file.write(SAVE_CAST(shapes_size), sizeof(shapes_size));
	
	for(ShapeComponent& shape : shapes)
	{
		file.write(SAVE_CAST(shape.type), sizeof(ShapeType));
		file.write(SAVE_CAST(shape.sector_index), sizeof(shape.sector_index));
		
		points_size = static_cast<uint32_t>(shape.points.size());
		file.write(SAVE_CAST(points_size), sizeof(points_size));
		file.write(SAVE_CAST(shape.points.data()), points_size * sizeof(Vector2D<float>));
		
		walls_size = static_cast<uint32_t>(shape.walls.size());
		file.write(SAVE_CAST(walls_size), sizeof(walls_size));
		
		for(Wall& wall : shape.walls)
		{
			uint32_t f_wall_index = static_cast<uint32_t>(wall.f_p_index);
			uint32_t s_wall_index = static_cast<uint32_t>(wall.s_p_index);
			uint32_t tid = static_cast<uint32_t>(wall.tid);
			bool is_window = wall.window_component != nullptr;
			
			file.write(SAVE_CAST(f_wall_index), sizeof(f_wall_index));
			file.write(SAVE_CAST(s_wall_index), sizeof(s_wall_index));
			file.write(SAVE_CAST(tid), sizeof(tid));
			file.write(SAVE_CAST(is_window), sizeof(is_window));
			
			if (!is_window) continue;
			file.write(reinterpret_cast<char*>(wall.window_component), sizeof(wall.window_component));
		}
	}
}

std::vector<ShapeComponent> FileServer::read_shapes(std::ifstream& file)
{
	std::vector<ShapeComponent> shapes;
	uint32_t points_size, walls_size, shapes_size;
	
	file.read(SAVE_CAST(&shapes_size), sizeof(shapes_size));
	shapes.resize(shapes_size);
	
	for(uint32_t shape_index = 0; shape_index < shapes_size; shape_index++)
	{
		std::vector<Wall> walls;
		
		file.read(SAVE_CAST(&shapes[shape_index].type), sizeof(ShapeType));
		file.read(SAVE_CAST(&shapes[shape_index].sector_index), sizeof(shapes[shape_index].sector_index));
		
		file.read(SAVE_CAST(&points_size), sizeof(points_size));
		shapes[shape_index].points.resize(points_size);
		file.read(SAVE_CAST(shapes[shape_index].points.data()), points_size * sizeof(Vector2D<float>));
		
		file.read(SAVE_CAST(&walls_size), sizeof(walls_size));
		walls.resize(walls_size);
		
		for(uint32_t wall_index = 0; wall_index < walls_size; wall_index++)
		{
			bool is_window;
			
			file.read(SAVE_CAST(&walls[wall_index].f_p_index), sizeof(walls[wall_index].f_p_index));
			file.read(SAVE_CAST(&walls[wall_index].s_p_index), sizeof(walls[wall_index].s_p_index));
			file.read(SAVE_CAST(&walls[wall_index].tid), sizeof(walls[wall_index].tid));
			file.read(SAVE_CAST(&is_window), sizeof(is_window));
			
			if (!is_window) continue;
			walls[wall_index].window_component = new WindowComponent();
			file.read(reinterpret_cast<char*>(walls[wall_index].window_component), sizeof(walls[wall_index].window_component));
		}
		shapes[shape_index].walls = std::move(walls);
	}
	return shapes;
}
