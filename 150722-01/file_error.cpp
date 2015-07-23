#include <stdio.h>
#include <stdlib.h>

void file_error(FILE *f, FILE *f2, FILE *f3){
		if(f==NULL){
			printf("css파일을 열지 못하였습니다.\n");
			system("pause");
			exit(1);
		}
		if(f2==NULL){
			printf("파일을 열지 못하였습니다.\n");
			system("pause");
			exit(1);
		}
		if(f3==NULL){
			printf("우선순위 파일을 열지 못하였습니다.\n");
			system("pause");
			exit(1);
		}
}