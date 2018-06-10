#ifndef _FILE_TIME_
#define _FILE_TIME_

#include <time.h>
#include <windows.h>
#include "catalog.c"

void file_modify_time(Root_Catalog* );
//void file_build_time(Root_Catalog* );

void file_modify_time(Root_Catalog* catalog){
    SYSTEMTIME currentTime;
    GetSystemTime(&currentTime);
	
	catalog -> File_Mtime_name = (currentTime.wHour << 11) | (currentTime.wMinute << 5) | (currentTime.wSecond >> 1);
	catalog -> File_Last_Modified = ((currentTime.wYear - 1980) << 9) | (currentTime.wMonth << 5) | (currentTime.wDay);
}
/*
void file_build_time(Root_Catalog* catalog) {
	SYSTEMTIME currentTime;
	GetSystemTime(&currentTime);

	catalog.System_Reserve[1] = (currentTime.wMilliseconds / 10);
}
*/
		 
#endif
