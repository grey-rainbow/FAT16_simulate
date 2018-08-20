#pragma once

#include "catalog.h"
#include <windows.h>

inline void file_modify_time(catalog &cata)
{
	SYSTEMTIME current_time;
	GetSystemTime(&current_time);

	cata.file_mtime_name = (current_time.wHour << 11) | (current_time.wMinute << 5) | (current_time.wSecond >> 1);
	cata.file_last_modified = ((current_time.wYear - 1980) << 9) | (current_time.wMonth << 5) | (current_time.wDay);
}


inline void file_build_time(catalog & cata) {
	SYSTEMTIME current_time;
	GetSystemTime(&current_time);

	cata.system_reserve[1] = (current_time.wMilliseconds / 10);
}


inline void print_modify_time(catalog &cata)
{
	const uint16_t second = (cata.file_mtime_name & 0b0000000000011111) << 1;
	const uint16_t minute = (cata.file_mtime_name & 0b0000011111100000) >> 5;
	const uint16_t hour = (cata.file_mtime_name & 0b1111100000000000) >> 11;
	const uint16_t day = (cata.file_last_modified & 0b0000000000011111);
	const uint16_t month = (cata.file_last_modified & 0b0000000111100000) >> 5;
	uint16_t year = (cata.file_last_modified & 0b1111111000000000) >> 9;
	year += 1980;
	printf("%u/%u/%u %u:%u:%u", year, month, day, hour, minute, second);
}