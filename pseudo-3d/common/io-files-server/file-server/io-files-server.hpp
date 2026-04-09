#ifndef IO_FILES_SERVER_HPP
#define IO_FILES_SERVER_HPP

#include <stdio.h>
#include <string>

class IOFilesServer
{
private:
	
	
public:
	IOFilesServer();
	~IOFilesServer();
	
	void write_package_json(std::string name, std::string json);
//	void write_package_bin(std::string name);
	
	void read_packages_json(std::string path);
	void read_packages_bin(std::string path);
};

#endif
