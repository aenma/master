#include <stdio.h>

void deleteA(char *line){   //다시 담기위해서 비움 
	int i;
	for(i=0;line[i]!=NULL;i++){
		line[i] = NULL;
	}
}