#ifndef _RESMAP_H_
#define _RESMAP_H_

#include "os.h"

#define MAX_NAME_LEN 256
#define MAX_RESOURCES 200

//������� ��������� ��� �������� ���������� �� ��������
struct TadsSimpleResourse
{
	char filename[MAX_NAME_LEN]; //��� �������
	unsigned long offset;		 //�������� ������������ ������ �����
	unsigned long sz;			 //������ �������
};

struct TadsResourseMap
{
	struct TadsSimpleResourse res[MAX_RESOURCES]; //����� ��������
	int total_res;								  //����� ����� ��������
	char* file_memory;							  //��������� �� ����(������)
};

extern struct TadsResourseMap resmap;
//�������� ����� �������� �� �����
extern void resmap_create(osfildef *fpin, unsigned long start_pos);
//������� ����� ��������
extern void resmap_cleanup();

#endif /* _RESMAP_H_ */

