#ifndef FILE_SERVER_HPP
#define FILE_SERVER_HPP

#define SAVE_CAST(x) reinterpret_cast<char*>(x)

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#include "nfd.hpp"
#include "level-server.hpp"
#include "shape-component.hpp"
#include "sector.hpp"

class FileServer
{
private:
	LevelServer* level_server;
	std::vector<ShapeComponent> file_shapes;
	std::vector<Sector> file_sectors;
	std::vector<const char*> file_textures_paths;
	
public:
	FileServer();
	FileServer(LevelServer* level_server, std::vector<const char*> textures_paths);
	~FileServer();
	
	std::string get_texture_path();
	
	void write_file(const char* file_path, std::vector<ShapeComponent>& shapes);
	void read_file(const char* file_path);
	
//	void write_file_json(const char* file_path);
//	void read_file_json(const char* file_path);
	
	void write_shapes(std::vector<ShapeComponent>& shapes, std::ofstream& file);
	std::vector<ShapeComponent> read_shapes(std::ifstream& file);
};

#endif
