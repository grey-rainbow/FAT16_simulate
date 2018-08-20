#pragma once

#include <fstream>

class catalog
{
public:
	catalog(uint8_t file_name[], uint8_t extend_name[], const uint8_t property, uint8_t system_reserve[],
	        const uint16_t file_mtime_name, const uint16_t file_last_modified, const uint16_t file_first_cluster,
	        const unsigned file_size);
	catalog(uint8_t file_name[], uint8_t extend_name[]);
	catalog() :	property_(0),
				file_mtime_name(0),
				file_last_modified(0),
				file_first_cluster(0),
				file_size(0)
	{
		memset(file_name, 0x20, 8);
		memset(extend_name, 0x20, 3);
		memset(system_reserve, 0, 10);
	}
				


	static void add_catalog(std::fstream&, catalog&);
	static void read_catalog(std::fstream&, catalog&);

public:
	uint8_t file_name[8];
	uint8_t extend_name[3];
	uint8_t property_;
	uint8_t system_reserve[10];
	uint16_t file_mtime_name;
	uint16_t file_last_modified;
	uint16_t file_first_cluster;
	unsigned int file_size;
};
