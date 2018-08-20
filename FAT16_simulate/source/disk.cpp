#include "../include/disk.h"
#include "../include/config.h"
#include "../include/file_time.h"

const uint8_t end_DBR[] = {0x55, 0xAA};
const uint8_t start_FAT[] = {0xf8, 0xff, 0xff, 0xff};

const long long int disk_size = 1024 * 10; //K
const int block_size = 512; //byte

void disk::calc_FAT()
{
	const long long int disk_size_sector = disk_size * 1024 / bytes_per_sector;
	const int temp = disk_size_sector - 1 - 32 * root_entries / bytes_per_sector;
	sectors_per_FAT = ceil(
		static_cast<double>(temp) / (static_cast<double>(sectors_per_custer) * bytes_per_sector / 2 + 2));
}

void disk::format_disk() const
{
	std::fstream fs(reinterpret_cast<const char*>(get_disk_label()), std::ios::in | std::ios::out | std::ios::binary);
	catalog label;

	//format disk
	const auto buffer = new char[disk_size + 1];
	memset(buffer, 0, sizeof(char *) * disk_size);
	fs.write(buffer, disk_size);

	fs.seekp(0x0b, std::fstream::beg);

	fs.write(reinterpret_cast<const char*>(&bytes_per_sector), 2);
	fs.write(reinterpret_cast<const char*>(&sectors_per_custer), 1);
	fs.write(reinterpret_cast<const char*>(&reserved_sector), 2);
	fs.write(reinterpret_cast<const char*>(&number_of_FAT), 1);
	fs.write(reinterpret_cast<const char*>(&root_entries), 2);

	fs.seekp(0x01fe, std::fstream::beg);
	fs.write(reinterpret_cast<const char*>(end_DBR), 2);

	//format FAT
	fs.seekp(bytes_per_sector * reserved_sector, std::fstream::beg);
	fs.write(reinterpret_cast<const char*>(start_FAT), 4);
	fs.seekp(sectors_per_FAT * bytes_per_sector - 4, std::fstream::beg);
	fs.write(reinterpret_cast<const char*>(start_FAT), 4);

	//format FDT
	fs.seekp(sectors_per_FAT * bytes_per_sector - 4, std::fstream::beg);

	memcpy(label.file_name, get_disk_label(), 8);
	label.property_ = 0 | 0x08;
	file_modify_time(label);
	label.file_first_cluster = 0;
	label.file_size = 0;
	
	catalog::add_catalog(fs, label);

	fs.close();
}
