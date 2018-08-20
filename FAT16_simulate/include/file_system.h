#pragma once
#include "catalog.h"
#include "disk.h"
#include <list>

class file_system
{
public:
	file_system(disk disk_dbr);

	file_system() = default;

	static void modify_FAT(std::fstream&, std::list<uint16_t>&);
	void add_file(catalog&, char*) const;
	static void add_fold(catalog&, char*);

private:
	disk disk_dbr_;
};
