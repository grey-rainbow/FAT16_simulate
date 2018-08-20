#include "../include/catalog.h"

catalog::catalog(uint8_t file_name[], uint8_t extend_name[], const uint8_t property, uint8_t system_reserve[],
                 const uint16_t file_mtime_name, const uint16_t file_last_modified, const uint16_t file_first_cluster,
                 const unsigned file_size): property_(property),
                                            file_mtime_name(file_mtime_name),
                                            file_last_modified(file_last_modified),
                                            file_first_cluster(file_first_cluster),
                                            file_size(file_size)
{
	memcpy(this->file_name, file_name, 8);
	memcpy(this->extend_name, extend_name, 3);
	memcpy(this->system_reserve, system_reserve, 10);
}

catalog::catalog(uint8_t file_name[], uint8_t extend_name[])
{
	memcpy(file_name, file_name, 8);
	memcpy(extend_name, extend_name, 3);
	memset(system_reserve, 0, 10);

}

void catalog::add_catalog(std::fstream& fs, catalog& root_catalog)
{
	char temp[9] = {'\0'};
	while (! fs.is_open())
	{
		fs.clear();
		memcpy(temp, root_catalog.file_name, 8);
		fs.open(temp, std::ios::in | std::ios::out | std::ios::binary);
	}

	fs.write(reinterpret_cast<const char*>(root_catalog.file_name), 8);
	fs.write(reinterpret_cast<const char*>(root_catalog.extend_name), 3);
	fs.write(reinterpret_cast<const char*>(&root_catalog.property_), 1);
	fs.write(reinterpret_cast<const char*>(root_catalog.system_reserve), 10);
	fs.write(reinterpret_cast<const char*>(&root_catalog.file_mtime_name), 2);
	fs.write(reinterpret_cast<const char*>(&root_catalog.file_last_modified), 2);
	fs.write(reinterpret_cast<const char*>(&root_catalog.file_first_cluster), 2);
	fs.write(reinterpret_cast<const char*>(&root_catalog.file_size), 4);
}

void catalog::read_catalog(std::fstream& fs, catalog& root_catalog)
{
	while (!fs.is_open())
	{
		fs.clear();
		throw std::logic_error("cannot open system file");
	}

	fs.read(reinterpret_cast<char*>(root_catalog.file_name), 8);
	fs.read(reinterpret_cast<char*>(root_catalog.extend_name), 3);
	fs.read(reinterpret_cast<char*>(&root_catalog.property_), 1);
	fs.read(reinterpret_cast<char*>(root_catalog.system_reserve), 10);
	fs.read(reinterpret_cast<char*>(&root_catalog.file_mtime_name), 2);
	fs.read(reinterpret_cast<char*>(&root_catalog.file_last_modified), 2);
	fs.read(reinterpret_cast<char*>(&root_catalog.file_first_cluster), 2);
	fs.read(reinterpret_cast<char*>(&root_catalog.file_size), 4);
}
