#include <cstdio>
#include "../include/disk.h"
#include "../include/file_time.h"
#include "../include/catalog.h"

void dir(FILE* disk, disk virtual_dbr)
{
	catalog catalog;
	// ReSharper disable CppDeclaratorNeverUsed
	int count = 0;
	// ReSharper restore CppDeclaratorNeverUsed
	uint8_t buffer[12] = {'\0'};

	printf("| File Name | Extend Name | Property | File Mtime Name |\n");

	fseek(disk, virtual_dbr.Bytes_Per_Sector * (virtual_dbr.Reserved_Sector + 2 * virtual_dbr.Sectors_Per_FAT),
	      SEEK_SET);

	//while (virtual_dbr.Root_Entries > count) {
	read_catalog(disk, &catalog);
	read_catalog(disk, &catalog);

	switch (catalog.property_)
	{
	case 0x00:
		memcpy(buffer, catalog.file_name, 8);
		printf("|%s|", buffer);
		memset(buffer, 0x0, sizeof(uint8_t) * 12);
		memcpy(buffer, catalog.extend_name, 3);
		printf("|%s|", buffer);
		printf("| read only |");
		print_modify_time(catalog);
		printf("\n");
		break;
	default:
		break;
	}
	fseek(disk, 32, SEEK_CUR);
	//}
}

int main()
{
	printf("Virtual File System Start:\n");
	printf("Please Input Disk size: K");

	//	int size = 0;
	//	scanf("%d", &size);
	//  disk_size = size * 1024 * 1024;

	disk dbr;
	catalog catalog = {"a", "txt", 0};
	FILE* disk;
	char buffer[] = {"a"};

	dbr.Bytes_Per_Sector = 512;
	dbr.Sectors_Per_Custer = 1;
	dbr.Reserved_Sector = 1;
	dbr.Number_of_FAT = 2;
	dbr.Root_Entries = 512;
	calc_FAT(&dbr);

	format_disk(disk_size, dbr);

	disk = fopen("virtual", "rb+");

	file_modify_time(&catalog);

	catalog.file_size = 1;

	add_file(disk, dbr, catalog, buffer);

	dir(disk, dbr);

	fclose(disk);

	return 0;
}
