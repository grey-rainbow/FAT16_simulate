#include <stdio.h>
#include <stdlib.h>
#include "disk.c"
#include "file_system.c"
#include "catalog.c"

void dir(FILE* disk, DBR virtual_dbr) {

	Root_Catalog catalog;
	int count = 0;
	unsigned char buffer[12] = {'\0'};

	printf("| File Name | Extend Name | Property | File Mtime Name |\n");

	fseek(disk, virtual_dbr.Bytes_Per_Sector * (virtual_dbr.Reserved_Sector + 2 * virtual_dbr.Sectors_Per_FAT), SEEK_SET);

	//while (virtual_dbr.Root_Entries > count) {
		read_catalog(disk, &catalog);
		read_catalog(disk, &catalog);
		
		switch(catalog.Property) {
		case 0x00:
			memcpy(buffer, catalog.File_Name, 8);
			printf("|%s|", buffer);
			memset(buffer, 0x0, sizeof(unsigned char) * 12);
			memcpy(buffer, catalog.Extend_Name, 3);
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

	DBR dbr;
	Root_Catalog catalog = {"a", "txt", 0};
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

	catalog.File_Size = 1;
	
	add_file(disk, dbr, catalog, buffer);

	dir(disk, dbr);
	
	fclose(disk);
   
	return 0;
}
