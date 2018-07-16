#ifndef _CATALOG_
#define _CATALOG_

#include <stdio.h>

typedef struct Root_Catalog {
	unsigned char File_Name[8];
	unsigned char Extend_Name[3];
	unsigned char Property;
	unsigned char System_Reserve[10];
	unsigned short File_Mtime_name;
	unsigned short File_Last_Modified;
	unsigned short File_First_Cluster;
	unsigned int File_Size;
}Root_Catalog;

void add_catalog(FILE*, Root_Catalog );
void read_catalog(FILE*, Root_Catalog*);

void add_catalog(FILE *file, Root_Catalog catalog) {

	fwrite(&catalog.File_Name, 1, 8, file);
	fwrite(&catalog.Extend_Name, 1, 3, file);
	fwrite(&catalog.Property, 1, 1, file);
	fwrite(&catalog.System_Reserve, 1, 10, file);
	fwrite(&catalog.File_Mtime_name, 1, 2, file);
	fwrite(&catalog.File_Last_Modified, 1, 2, file);
	fwrite(&catalog.File_First_Cluster, 1, 2, file);
	fwrite(&catalog.File_Size, 1, 4, file);	
}

void read_catalog(FILE* file, Root_Catalog* catalog) {
	fread(&catalog -> File_Name, 1, 8, file);
	fread(&catalog -> Extend_Name, 1, 3, file);
	fread(&catalog -> Property, 1, 1, file);
	fread(&catalog -> System_Reserve, 1, 10, file);
	fread(&catalog -> File_Mtime_name, 1, 2, file);
	fread(&catalog -> File_Last_Modified, 1, 2, file);
	fread(&catalog -> File_First_Cluster, 1, 2, file);
	fread(&catalog -> File_Size, 1, 4, file);
}

#endif
