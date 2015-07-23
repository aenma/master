#include <stdio.h>

struct csstable{   //css.txt 颇老
	char tagname[50];   //加己 
	int tagnum;   //加己鉴辑 
	struct csstable *next;   //促澜 加己啊府糯 
};
typedef struct csstable csstable;

struct ssline{   //鉴辑 沥府且 css颇老
	char stitle[200];  //加己 
	char svalue[200];  //蔼
	char sjs[200];     //林籍
	int num;   //加己鉴辑 
};
typedef struct ssline ssline;