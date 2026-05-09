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
#include "texture-buffer.hpp"
#include "shape-component.hpp"
#include "sector.hpp"

class FileServer
{
private:
	LevelServer* level_server_ptr;
	TextureBuffer* buffer_ptr;
	
public:
	FileServer();
	FileServer(LevelServer* level_server, TextureBuffer* buffer);
	~FileServer();
	
	static std::string get_texture_path();
	
	void write_file(const char* file_path);
	void read_file(const char* file_path);
	
//	void write_file_json(const char* file_path);
//	void read_file_json(const char* file_path);
	
	void write_string(std::ofstream& file, std::string& string);
	std::string read_string(std::ifstream& file);
	void write_textures_names(std::ofstream& file);
	std::vector<std::string> read_textures_names(std::ifstream& file);
	void write_sectors(std::ofstream& file);
	std::vector<Sector> read_sectors(std::ifstream& file);
	void write_cameras(std::ofstream& file);
	std::vector<Camera*> read_cameras(std::ifstream& file);
	void write_shapes(std::ofstream& file);
	std::vector<ShapeComponent> read_shapes(std::ifstream& file);
};

#endif
