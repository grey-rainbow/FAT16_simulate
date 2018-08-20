#include "disk.cpp"
#include "../include/file_time.h"
#include "../include/file_system.h"

void file_system::add_file(catalog &file_catalog, char* buffer) const
{
	std::fstream fs(reinterpret_cast<const char*>(get_disk_label()), std::ios::in | std::ios::out | std::ios::binary);

	//modify FAT
	uint16_t temp = 0;
	unsigned int size = file_catalog.file_size;
	uint16_t count = 2;
	catalog current_catalog ;
	int file_count = 0;
	std::list<uint16_t> file_list;

	fs.seekg(disk_dbr_.reserved_sector * disk_dbr_.bytes_per_sector + 4, std::fstream::beg);

	while (true)
	{
		fs.read(reinterpret_cast<char*>(temp), 2);
		if (! temp)
		{
			file_list.push_back(count);

			if (size <= disk_dbr_.bytes_per_sector) break;
			size -= disk_dbr_.bytes_per_sector;
		}

		count++;
	}

	fs.seekp(disk_dbr_.reserved_sector * disk_dbr_.bytes_per_sector + 4, std::fstream::beg);

	modify_FAT(fs, file_list);

	//FAT backup
	fs.seekp((disk_dbr_.reserved_sector + disk_dbr_.sectors_per_FAT) * disk_dbr_.bytes_per_sector + 4,
			 std::fstream::beg);

	modify_FAT(fs, file_list);

	//add root catalog
	file_catalog.file_first_cluster = file_list.front();

	fs.seekg(disk_dbr_.bytes_per_sector * (disk_dbr_.reserved_sector + disk_dbr_.sectors_per_FAT * 2),
	         std::fstream::beg);
	fs.seekp(disk_dbr_.bytes_per_sector * (disk_dbr_.reserved_sector + disk_dbr_.sectors_per_FAT * 2),
			 std::fstream::beg);

	while (file_count < disk_dbr_.root_entries)
	{
		catalog::read_catalog(fs, current_catalog);
		if (current_catalog.file_name[0] == '\0')
		{
			catalog::add_catalog(fs, file_catalog);
			break;
		}
		memset(current_catalog.file_name, 0, 8);
		file_count++;
	}

	//save data
	fs.seekp(disk_dbr_.bytes_per_sector * (disk_dbr_.reserved_sector + disk_dbr_.sectors_per_FAT * 2) +
	      disk_dbr_.root_entries * 32, std::fstream::beg);

	uint16_t last = file_list.front() - 2;

	for (auto iter = file_list.begin(); iter != file_list.end();)
	{
		fs.seekp((*iter - last - 1) * disk_dbr_.sectors_per_custer * disk_dbr_.bytes_per_sector,
		      std::fstream::cur);
		last = *iter;
		if (++iter == file_list.end())
			fs.write(buffer,
			       (file_catalog.file_size - 1) % disk_dbr_.bytes_per_sector * disk_dbr_.sectors_per_custer + 1);
		else
			fs.write(buffer, disk_dbr_.bytes_per_sector * disk_dbr_.sectors_per_custer);
	}

	//free
	file_list.clear();
} 

file_system::file_system(disk disk_dbr):
	disk_dbr_(disk_dbr)
{
}

void file_system::add_fold(catalog&, char*)
{
}

void file_system::modify_FAT(std::fstream &fs, std::list<uint16_t> &file_list)
{
	uint16_t last = file_list.front() - 1;
	auto iter = file_list.begin();
	for (; iter != file_list.end(); ++iter)
	{
		fs.seekp((*iter - last - 1) * 2, std::fstream::cur);
		fs.write(reinterpret_cast<const char*>(*(++iter)), 2);
		--iter;
		last = *iter;
	}

	const uint16_t temp = 0xFF;
	fs.seekp((*iter - last - 1) * 2, std::fstream::cur);
	fs.write(reinterpret_cast<const char*>(temp), 2);
}
