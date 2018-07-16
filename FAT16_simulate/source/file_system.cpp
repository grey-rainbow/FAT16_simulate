#ifndef _FILE_SYSTEM_
#define _FILE_SYSTEM_

#include "catalog.cpp"
#include "disk.cpp"
#include "file_time.c"

typedef struct Node* pNode;

typedef struct Node {
	unsigned short element;
	pNode next;
}Node;

void modify_FAT(FILE* disk, pNode root) {

	unsigned short last = root -> element - 1;
	pNode list;
	unsigned short temp = 0;
	
	for (list = root; list -> next; list = list -> next) {

		fseek(disk, (list -> element - last - 1) * 2, SEEK_CUR);		
		fwrite(&(list -> next -> element), 2, 1, disk);
		last = list -> element;
	}

	temp = 0xFF;
	fseek(disk, (list -> element - last - 1) * 2, SEEK_CUR);		
	fwrite(&temp, 2, 1, disk);
	
}

void add_file(FILE*, DBR, Root_Catalog, char*);
void add_fold(FILE*, DBR, Root_Catalog, char*);


void add_file(FILE* disk, DBR virtual_dbr, Root_Catalog catalog, char* buffer) {

	//modify FAT
	unsigned short temp = 0;
	unsigned int size = catalog.File_Size;
	pNode list, root;
	unsigned short count = 2, last = 2;
	Root_Catalog current_catalog =  {{0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20},
									 {0x20, 0x20, 0x20}};;
	int file_count = 0;

	fseek(disk, virtual_dbr.Reserved_Sector * virtual_dbr.Bytes_Per_Sector + 4, SEEK_SET);
	list = (pNode) malloc (sizeof(Node));
	list -> next = NULL;
	root = list;
	
	while (1) {
		
		fread(&temp, 2, 1, disk);				
		if (temp == 0) {
			
			list -> element = count;
			
			if (size <= virtual_dbr.Bytes_Per_Sector) {			
				break;
			}
			else {
				size -= virtual_dbr.Bytes_Per_Sector;				
			}
			list -> next = (pNode) malloc (sizeof(Node));
			list = list -> next;
			list -> next = NULL;
		}
		count++;
	}

	fseek(disk, virtual_dbr.Reserved_Sector * virtual_dbr.Bytes_Per_Sector + 4, SEEK_SET);

	modify_FAT(disk, root);
	
	//FAT backup

	fseek(disk, (virtual_dbr.Reserved_Sector + virtual_dbr.Sectors_Per_FAT) * virtual_dbr.Bytes_Per_Sector + 4, SEEK_SET);

	modify_FAT(disk, root);

	catalog.File_First_Cluster = root -> element;

	//add root catalog

	fseek(disk, virtual_dbr.Bytes_Per_Sector * (virtual_dbr.Reserved_Sector + virtual_dbr.Sectors_Per_FAT * 2), SEEK_SET);
	
	while (file_count < virtual_dbr.Root_Entries) {
		
		//fread(&current_catalog.File_Name[0], 1, 1, disk);
		//fseek(disk, 31, SEEK_CUR);
		read_catalog(disk, &current_catalog);
		if (current_catalog.File_Name[0] == '\0') {
			//fseek(disk, -32, SEEK_CUR);			
			add_catalog(disk, catalog);
			break;
		}
		//fseek(disk, -32, SEEK_CUR);
		memset(current_catalog.File_Name, 0, 8);
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

	//free
	
	for (list = root; list; list = root) {
		root = list -> next;
		free(list);
	}
}

#endif
