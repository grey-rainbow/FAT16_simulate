#pragma once

#include <windows.h>
#include "catalog.h"

/**
* \brief create a file named virtual and then format it
*/
struct disk
{
	//DBR
	uint16_t bytes_per_sector;
	uint8_t sectors_per_custer;
	uint16_t reserved_sector;
	uint8_t number_of_FAT;
	uint16_t root_entries;
	uint16_t sectors_per_FAT;
	//
	void calc_FAT();
	void format_disk() const;

	disk()
	{
		calc_FAT();
		format_disk();
	}
};