#include <stdio.h>
#include <stdlib.h>
#include <math.h>

unsigned char end_DBR[] = {0x55, 0xAA};
unsigned char start_FAT[] = {0xf8, 0xff, 0xff, 0xff};

typedef struct DBR {
	unsigned short Bytes_Per_Sector;
	unsigned char Sectors_Per_Custer;
	unsigned short Reserved_Sector;
	unsigned char Number_of_FAT;
	unsigned short Root_Entries;
	unsigned short Sectors_Per_FAT;
}DBR;

typedef struct DISK {
	DBR virtual_dbr;
	int FAT_size;
	
}DISK;

long long int disk_size = 1024 * 10; //M
int block_size = 512; //byte

void format_disk(long long int, DBR);

int main() 
{
	printf("Virtual File System Start:\n");
	printf("Please Input Disk size: M");
	
	//	int size = 0;
	//	scanf("%d", &size);
	//  disk_size = size * 1024 * 1024;

	DBR dbr;
	dbr.Bytes_Per_Sector = 512;
	dbr.Sectors_Per_Custer = 1;
	dbr.Reserved_Sector = 1;
	dbr.Number_of_FAT = 2;
	dbr.Root_Entries = 512;

	format_disk(disk_size, dbr);
		
	return 0;
}

void format_disk(long long int disk_size, DBR virtual_dbr) {

	FILE* disk = fopen("virtual", "wb+");
	long long int disk_size_sector = disk_size * 1024 / virtual_dbr.Bytes_Per_Sector;
	int temp = 0;
	//format DBR

	temp = disk_size_sector - 1 - 32 * virtual_dbr.Root_Entries / virtual_dbr.Bytes_Per_Sector;
	virtual_dbr.Sectors_Per_FAT  = ceil((temp  + 4 * virtual_dbr.Sectors_Per_Custer) * 1.0 / (virtual_dbr.Bytes_Per_Sector / 16 + 2));
	
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

	
	
	
	fclose(disk);
}
