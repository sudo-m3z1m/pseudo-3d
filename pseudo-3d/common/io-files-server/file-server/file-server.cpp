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

std::string FileServer::set_resource_path(const char* name, const char* ext)
{
	NFD::Guard guard;
	
	NFD::UniquePath resource_path;
	nfdfilteritem_t filter_item[1] = {{name, ext}};
	
	nfdresult_t pick_result = NFD::SaveDialog(resource_path, filter_item, 1);
	if(pick_result != NFD_OKAY) return nullptr;
	std::string path = resource_path.get();
	
	return path;
}

std::string FileServer::get_resource_path(const char* name, const char* ext)
{
	NFD::Guard guard;
	
	NFD::UniquePath resource_path;
	nfdfilteritem_t filter_item[1] = {{name, ext}};
	
	nfdresult_t pick_result = NFD::OpenDialog(resource_path, filter_item, 1);
	if(pick_result != NFD_OKAY) return nullptr;
	std::string path = resource_path.get();
	
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
	
	std::vector<std::string> textures_names = read_textures_names(file);
	std::vector<Sector> sectors = read_sectors(file);
	std::vector<Camera*> cameras = read_cameras(file);
	std::vector<ShapeComponent> file_shapes = read_shapes(file);
	file.close();
	
	level_server_ptr->clear_level_data();
	buffer_ptr->clear_buffer();
	
	for(std::string name : textures_names) buffer_ptr->load_texture(name);
	for(Sector sector : sectors) level_server_ptr->add_new_sector(sector);
	for(Camera* camera : cameras) level_server_ptr->add_camera(camera);
	for(ShapeComponent shape : file_shapes) level_server_ptr->add_new_polygon(shape);
}

void FileServer::write_textures_names(std::ofstream& file)
{
	std::vector<std::string> textures_names = buffer_ptr->get_textures_names();
	uint32_t names_size = static_cast<uint32_t>(textures_names.size());
	
	file.write(SAVE_CAST(&names_size), sizeof(names_size));
	for(std::string& name : textures_names) write_string(file, name);
}

std::vector<std::string> FileServer::read_textures_names(std::ifstream& file)
{
	std::vector<std::string> textures_names;
	uint32_t names_size;
	file.read(SAVE_CAST(&names_size), sizeof(names_size));
	textures_names.resize(names_size);
	
	for(uint32_t name_index = 0; name_index < names_size; name_index++)
	{
		std::string name = read_string(file);
		textures_names[name_index] = name;
	}
	return textures_names;
}

void FileServer::write_sectors(std::ofstream& file)
{
	std::vector<Sector> sectors = level_server_ptr->sectors;
	uint32_t sectors_size = static_cast<uint32_t>(sectors.size());
	file.write(SAVE_CAST(&sectors_size), sizeof(sectors_size));
	//file.write(SAVE_CAST(sectors.data()), sectors.size() * sizeof(Sector));
	
	for(Sector& sector : sectors)
	{
		uint32_t floor_tid = static_cast<uint32_t>(sector.floor_tid);
		uint32_t ceiling_tid = static_cast<uint32_t>(sector.ceiling_tid);
		
		file.write(SAVE_CAST(&sector.floor_z), sizeof(sector.floor_z));
		file.write(SAVE_CAST(&sector.ceiling_z), sizeof(sector.ceiling_z));
		file.write(SAVE_CAST(&floor_tid), sizeof(floor_tid));
		file.write(SAVE_CAST(&ceiling_tid), sizeof(ceiling_tid));
	}
}

std::vector<Sector> FileServer::read_sectors(std::ifstream& file)
{
	std::vector<Sector> sectors;
	uint32_t sectors_size;
	file.read(SAVE_CAST(&sectors_size), sizeof(sectors_size));
	sectors.resize(sectors_size);
	//file.read(SAVE_CAST(sectors.data()), sectors_size * sizeof(Sector));
	
	for(uint32_t sector_index = 0; sector_index < sectors_size; sector_index++)
	{
		uint32_t floor_tid;
		uint32_t ceiling_tid;
		
		file.read(SAVE_CAST(&sectors[sector_index].floor_z), sizeof(sectors[sector_index].floor_z));
		file.read(SAVE_CAST(&sectors[sector_index].ceiling_z), sizeof(sectors[sector_index].ceiling_z));
		
		file.read(SAVE_CAST(&floor_tid), sizeof(floor_tid));
		file.read(SAVE_CAST(&ceiling_tid), sizeof(ceiling_tid));
		sectors[sector_index].floor_tid = floor_tid;
		sectors[sector_index].ceiling_tid = ceiling_tid;
	}
	
	return sectors;
}

void FileServer::write_cameras(std::ofstream& file)
{
	std::vector<Camera*> cameras = level_server_ptr->get_cameras();
	uint32_t cameras_size = static_cast<uint32_t>(cameras.size());
	
	file.write(SAVE_CAST(&cameras_size), sizeof(cameras_size));
	for(Camera* camera : cameras)
	{
		file.write(SAVE_CAST(&camera->position), sizeof(camera->position));
		file.write(SAVE_CAST(&camera->rotation), sizeof(camera->rotation));
		file.write(SAVE_CAST(&camera->height_z), sizeof(camera->height_z));
		file.write(SAVE_CAST(&camera->field_of_view), sizeof(camera->field_of_view));
	}
}

std::vector<Camera*> FileServer::read_cameras(std::ifstream& file)
{
	std::vector<Camera*> file_cameras;
	uint32_t cameras_size;
	
	file.read(SAVE_CAST(&cameras_size), sizeof(cameras_size));
	file_cameras.resize(cameras_size);
	
	for(uint32_t camera_index = 0; camera_index < cameras_size; camera_index++)
	{
		Vector2D<float> camera_position;
		float camera_rotation, camera_height, camera_fov;
		
		file.read(SAVE_CAST(&camera_position), sizeof(camera_position));
		file.read(SAVE_CAST(&camera_rotation), sizeof(camera_rotation));
		file.read(SAVE_CAST(&camera_height), sizeof(camera_height));
		file.read(SAVE_CAST(&camera_fov), sizeof(camera_fov));
		
		file_cameras[camera_index] = new Camera(camera_fov, camera_position, camera_rotation, camera_height, 0);
	}
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

void FileServer::write_string(std::ofstream& file, std::string& string)
{
	uint32_t string_size = static_cast<uint32_t>(string.size());
	char* string_data = string.data();
	
	file.write(SAVE_CAST(&string_size), sizeof(string_size));
	file.write(string_data, string_size);
}

std::string FileServer::read_string(std::ifstream& file)
{
	uint32_t string_size;
	file.read(SAVE_CAST(&string_size), sizeof(string_size));
	
	std::string string(string_size, '\0');
	file.read(string.data(), string_size);
	return string;
}
