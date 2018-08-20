#include "..\include\config.h"

uint8_t* get_disk_label()
{
	static uint8_t disk_label[] = { 'v','i','r','t','u','a','l'};
	return disk_label;
}
