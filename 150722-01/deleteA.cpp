#include <stdio.h>

void deleteA(char *line){   //�ٽ� ������ؼ� ��� 
	int i;
	for(i=0;line[i]!=NULL;i++){
		line[i] = NULL;
	}
}