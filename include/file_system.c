#include <stdio.h>
#include <stdlib.h>
#include "catalog.c"
#include "disk.c"

typedef struct Node* pNode;

typedef struct Node {
	unsigned short element;
	pNode next;
}Node;

void add_file(FILE*);
void add_fold(FILE*);


int main() {

	
	
	return 0;
}
		 
void add_file(FILE* disk, DBR virtual_dbr, Root_Catalog catalog, char* buffer) {

	//modify FAT
	unsigned short temp = 0;
	unsigned int size = catalog.File_Size;
	pNode list, root;
	unsigned short count = 2, last = 2;
	Root_Catalog current_catalog;
	int file_count = 0;
	
	fseek(disk, virtual_dbr.Reserved_Sector * virtual_dbr.Bytes_Per_Sector + 4, SEEK_SET);

	list = (pNode) malloc (sizeof(Node));
	list -> next = NULL;
	root = list;
	
	while (true) {
		
		fread(&temp, 2, 1, disk);				
		if (temp == 0) {
			
			list -> element = count;

			temp = count;
			fseek(disk, -(count - last + 1) * 2, SEEK_SET);
			fwrite(&temp, 2, 1, disk);
			fseek(disk, (count - last) * 2, SEEK_SET);
			
			if (size <= virtual_dbr.Bytes_Per_Sector) {
				temp = 0xFF;
				fwrite(&temp, 2, 1, disk);				
				break;
			}
			else {
				last = count;
				size -= virtual_dbr.Bytes_Per_Sector;				
			}
			list -> next = (pNode) malloc (sizeof(Node));
			list = list -> next;
			list -> next = NULL;
		}
		count += 2;
	}

	//FAT backup

	fseek(disk, virtual_dbr.Reserved_Sector * virtual_dbr.Bytes_Per_Sector + 4 + virtual_dbr.Sectors_Per_FAT, SEEK_SET);

	last = root -> element - 1;
	
	for (list = root; list -> next; list = list -> next) {

		fseek(disk, (list -> element - last - 1) * 2, SEEK_CUR);		
		fwrite(&(list -> next -> element), 2, 1, disk);
		last = list -> element;
	}

	temp = 0xFF;
	fseek(disk, (list -> element - last - 1) * 2, SEEK_CUR);		
	fwrite(&temp, 2, 1, disk);

	catalog.File_First_Cluster = root -> element;

	//add root catalog

	fseek(disk, virtual_dbr.Bytes_Per_Sector * (virtual_dbr.Reserved_Sector + virtual_dbr.Sectors_Per_FAT * 2), SEEK_SET);
	
	while (file_count < virtual_dbr.Root_Entries) {
		
		fread(&current_catalog -> File_Name, 1, 8, disk);
		if (current_catalog[0] == '\0') {
			fseek(disk, -8, SEEK_CUR);			
			add_catalog(disk, catalog);
			break;
		}

		file_count++;
	}

	//save data

	fseek(disk, virtual_dbr.Bytes_Per_Sector * (virtual_dbr.Reserved_Sector + virtual_dbr.Sectors_Per_FAT * 2) + virtual_dbr.Root_Entries * 32, SEEK_SET);

	last = root -> element - 2;
	
	for (list = root; list; list = list -> next) {

		fseek(disk, (list -> element - last - 1) * virtual_dbr.Sectors_Per_Custer * virtual_dbr.Bytes_Per_Sector, SEEK_CUR);
		if (list -> next == NULL)
			fwrite(buffer, 1, (catalog.File_Size - 1) % virtual_dbr.Bytes_Per_Sector * virtual_dbr.Sectors_Per_Custer + 1, disk);
		else
			fwrite(buffer, 1, virtual_dbr.Bytes_Per_Sector * virtual_dbr.Sectors_Per_Custer, disk);
		last = list -> element;
	}
}
