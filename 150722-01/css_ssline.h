#include <stdio.h>

struct csstable{   //css.txt ����
	char tagname[50];   //�Ӽ� 
	int tagnum;   //�Ӽ����� 
	struct csstable *next;   //���� �Ӽ�����Ŵ 
};
typedef struct csstable csstable;

struct ssline{   //���� ������ css����
	char stitle[200];  //�Ӽ� 
	char svalue[200];  //��
	char sjs[200];     //�ּ�
	int num;   //�Ӽ����� 
};
typedef struct ssline ssline;