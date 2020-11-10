#ifndef _RESMAP_H_
#define _RESMAP_H_

#include "os.h"

#define MAX_NAME_LEN 256
#define MAX_RESOURCES 200

//ѕроста€ структура дл€ хранени€ информации по ресурсам
struct TadsSimpleResourse
{
	char filename[MAX_NAME_LEN]; //им€ ресурса
	unsigned long offset;		 //смещение относительно начала файла
	unsigned long sz;			 //размер ресурса
};

struct TadsResourseMap
{
	struct TadsSimpleResourse res[MAX_RESOURCES]; //карта ресурсов
	int total_res;								  //общее число ресурсов
	char* file_memory;							  //указатель на файл(пам€ть)
};

extern struct TadsResourseMap resmap;
//создание карты ресурсов из файла
extern void resmap_create(osfildef *fpin, unsigned long start_pos);
//очистка карты ресурсов
extern void resmap_cleanup();

#endif /* _RESMAP_H_ */

