#include "file-server.hpp"

FileServer::FileServer()
{
	level_server_ptr = nullptr;
	buffer_ptr = nullptr;
}

FileServer::FileServer(LevelServer* level_server, TextureBuffer* buffer)
{
	level_server_ptr = level_server;
	buffer_ptr = buffer;
}

FileServer::~FileServer()
{
	level_server_ptr = nullptr;
	buffer_ptr = nullptr;
}

std::string FileServer::get_texture_path()
{
	NFD::Guard guard;
	
	NFD::UniquePath texture_path;
	nfdfilteritem_t filter_item[1] = {{"Textures", "bmp"}};
	
	nfdresult_t pick_result = NFD::OpenDialog(texture_path, filter_item, 1);
	std::string path = texture_path.get();
	if(pick_result != NFD_OKAY) return nullptr;
	
	return path;
}

void FileServer::write_file(const char* file_path)
{
	std::ofstream file(file_path, std::ios::binary);
	if(!file)
	{
		std::cerr << "SYSTEM ERROR: " << strerror(errno) << std::endl;
		return;
	}
	write_textures_names(file);
	write_sectors(file);
	write_cameras(file);
	write_shapes(file);
	file.close();
}

void FileServer::read_file(const char* file_path)
{
	std::ifstream file(file_path, std::ios::binary);
	if (!file) return;
	
	std::vector<ShapeComponent> file_shapes = read_shapes(file);
	file.close();
	
	for(ShapeComponent shape : file_shapes)
	{
		level_server_ptr->add_new_polygon(shape);
	}
}

void FileServer::write_textures_names(std::ofstream& file)
{
	
}

std::vector<std::string> FileServer::read_textures_names(std::ofstream& file)
{
	std::vector<std::string> textures_names;
	return textures_names;
}

void FileServer::write_sectors(std::ofstream& file)
{
	
}

std::vector<Sector> FileServer::read_sectors(std::ofstream& file)
{
	std::vector<Sector> file_sectors;
	return file_sectors;
}

void FileServer::write_cameras(std::ofstream& file)
{
	
}

std::vector<Camera*> FileServer::read_cameras(std::ofstream& path)
{
	std::vector<Camera*> file_cameras;
	return file_cameras;
}

void FileServer::write_shapes(std::ofstream& file)
{
	std::vector<ShapeComponent> shapes = level_server_ptr->get_levels_shapes();
	
	uint32_t points_size;
	uint32_t walls_size;
	uint32_t shapes_size = static_cast<uint32_t>(shapes.size());
	file.write(SAVE_CAST(&shapes_size), sizeof(shapes_size));
	
	for(ShapeComponent& shape : shapes)
	{
		file.write(SAVE_CAST(&shape.type), sizeof(shape.type));
		file.write(SAVE_CAST(&shape.sector_index), sizeof(shape.sector_index));
		
		points_size = static_cast<uint32_t>(shape.points.size());
		file.write(SAVE_CAST(&points_size), sizeof(points_size));
		file.write(SAVE_CAST(shape.points.data()), points_size * sizeof(Vector2D<float>));
		
		walls_size = static_cast<uint32_t>(shape.walls.size());
		file.write(SAVE_CAST(&walls_size), sizeof(walls_size));
		
		for(Wall& wall : shape.walls)
		{
			uint32_t f_wall_index = static_cast<uint32_t>(wall.f_p_index);
			uint32_t s_wall_index = static_cast<uint32_t>(wall.s_p_index);
			uint32_t tid = static_cast<uint32_t>(wall.tid);
			Vector2D<float> wall_normal = wall.normal;
			bool is_window = wall.window_component != nullptr;
			
			file.write(SAVE_CAST(&f_wall_index), sizeof(f_wall_index));
			file.write(SAVE_CAST(&s_wall_index), sizeof(s_wall_index));
			file.write(SAVE_CAST(&tid), sizeof(tid));
			file.write(SAVE_CAST(&is_window), sizeof(is_window));
			file.write(SAVE_CAST(&wall_normal), sizeof(wall_normal));
			
			if (!is_window) continue;
			file.write(reinterpret_cast<char*>(wall.window_component), sizeof(WindowComponent));
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
		
		file.read(SAVE_CAST(&shapes[shape_index].type), sizeof(shapes[shape_index].type));
		file.read(SAVE_CAST(&shapes[shape_index].sector_index), sizeof(shapes[shape_index].sector_index));
		
		file.read(SAVE_CAST(&points_size), sizeof(points_size));
		shapes[shape_index].points.resize(points_size);
		file.read(SAVE_CAST(shapes[shape_index].points.data()), points_size * sizeof(Vector2D<float>));
		
		file.read(SAVE_CAST(&walls_size), sizeof(walls_size));
		walls.resize(walls_size);
		
		for(uint32_t wall_index = 0; wall_index < walls_size; wall_index++)
		{
			bool is_window;
			
			file.read(SAVE_CAST(&walls[wall_index].f_p_index), sizeof(uint32_t));
			file.read(SAVE_CAST(&walls[wall_index].s_p_index), sizeof(uint32_t));
			file.read(SAVE_CAST(&walls[wall_index].tid), sizeof(uint32_t));
			file.read(SAVE_CAST(&is_window), sizeof(is_window));
			file.read(SAVE_CAST(&walls[wall_index].normal), sizeof(Vector2D<float>));
			
			if (!is_window) continue;
			walls[wall_index].window_component = new WindowComponent();
			file.read(reinterpret_cast<char*>(walls[wall_index].window_component), sizeof(WindowComponent));
		}
		shapes[shape_index].walls = std::move(walls);
	}
	return shapes;
}
