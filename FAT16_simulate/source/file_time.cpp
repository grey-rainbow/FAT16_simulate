#ifndef _FILE_TIME_
#define _FILE_TIME_

#include <time.h>
#include <windows.h>
#include "catalog.cpp"

void file_modify_time(Root_Catalog* );
//void file_build_time(Root_Catalog* );
void print_modify_time(Root_Catalog);

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

void print_modify_time(Root_Catalog catalog) {
	unsigned short second = (catalog.File_Mtime_name & 0b0000000000011111) << 1;
	unsigned short minute = (catalog.File_Mtime_name & 0b0000011111100000) >> 5;
	unsigned short hour = (catalog.File_Mtime_name &   0b1111100000000000) >> 11;
	unsigned short day = (catalog.File_Last_Modified & 0b0000000000011111);
	unsigned short month = (catalog.File_Last_Modified&0b0000000111100000) >> 5;
	unsigned short year = (catalog.File_Last_Modified& 0b1111111000000000) >> 9;
	year += 1980;
	printf("%u/%u/%u %u:%u:%u", year, month, day, hour, minute, second);
}

#endif
