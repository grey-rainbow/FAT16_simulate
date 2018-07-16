#ifndef _DISK_
#define _DISK_

#include <math.h>
#include "catalog.cpp"
#include <windows.h>
#include "file_time.cpp"

unsigned char end_DBR[] = {0x55, 0xAA};
unsigned char start_FAT[] = {0xf8, 0xff, 0xff, 0xff};
unsigned char disk_label[] = {"virtual "};

typedef struct DBR {
	unsigned short Bytes_Per_Sector;
	unsigned char Sectors_Per_Custer;
	unsigned short Reserved_Sector;
	unsigned char Number_of_FAT;
	unsigned short Root_Entries;
	unsigned short Sectors_Per_FAT;
}DBR;

long long int disk_size = 1024 * 10; //K
int block_size = 512; //byte

void calc_FAT(DBR*);
void format_disk(long long int, DBR);

void calc_FAT(DBR* virtual_dbr) {
	long long int disk_size_sector = disk_size * 1024 / virtual_dbr -> Bytes_Per_Sector;
	int temp = disk_size_sector - 1 - 32 * virtual_dbr -> Root_Entries / virtual_dbr -> Bytes_Per_Sector;
	virtual_dbr -> Sectors_Per_FAT  = ceil((temp * 1.0) / (virtual_dbr -> Sectors_Per_Custer * virtual_dbr -> Bytes_Per_Sector / 2 + 2));	
}

void format_disk(long long int disk_size, DBR virtual_dbr) {

	FILE* disk = fopen("virtual", "wb+");
	Root_Catalog Label = {{0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20},
						  {0x20, 0x20, 0x20}};

	//format DBR
	unsigned char *buffer = (unsigned char*) malloc (sizeof(char) * disk_size);
	memset(buffer, 0, sizeof(unsigned char) * disk_size);
	fwrite(buffer, 1, disk_size, disk);

	fseek(disk, 0x0b, SEEK_SET);

	fwrite(&virtual_dbr.Bytes_Per_Sector, 1, 2, disk);
	fwrite(&virtual_dbr.Sectors_Per_Custer, 1, 1, disk);
	fwrite(&virtual_dbr.Reserved_Sector, 1, 2, disk);
	fwrite(&virtual_dbr.Number_of_FAT, 1, 1, disk);
	fwrite(&virtual_dbr.Root_Entries, 1, 2, disk);

	fseek(disk, 0x01fe, SEEK_SET);
	fwrite(end_DBR, 1, 2, disk);

	//format FAT
	
	fseek(disk, virtual_dbr.Bytes_Per_Sector * virtual_dbr.Reserved_Sector, SEEK_SET);
	fwrite(start_FAT, 1, 4, disk);
	fseek(disk, virtual_dbr.Sectors_Per_FAT * virtual_dbr.Bytes_Per_Sector - 4, SEEK_CUR);
	fwrite(start_FAT, 1, 4, disk);

	//format FDT

	fseek(disk, virtual_dbr.Sectors_Per_FAT * virtual_dbr.Bytes_Per_Sector - 4, SEEK_CUR);
	
	memcpy(Label.File_Name, disk_label, 8);
	Label.Property = 0 | 0x08;
	file_modify_time(&Label);
	Label.File_First_Cluster = 0;
	Label.File_Size = 0;
	
	add_catalog(disk, Label);
	
	fclose(disk);
}

#endif
