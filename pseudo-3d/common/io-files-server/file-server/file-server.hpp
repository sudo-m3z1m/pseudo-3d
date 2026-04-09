#ifndef FILE_SERVER_HPP
#define FILE_SERVER_HPP

#include <stdio.h>
#include <string>

class FileServer
{
private:
	//TODO: Just make a cast of level-server and texture buffer and save it to bin file.
	//TODO: JSON will added soon after it
	
public:
	FileServer();
	~FileServer();
//	void write_package_json(std::string name, std::string json);
//	void write_package_bin(std::string name);
	
//	void read_packages_json(std::string path);
//	void read_packages_bin(std::string path);
};

#endif
