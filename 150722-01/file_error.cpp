#include <stdio.h>
#include <stdlib.h>

void file_error(FILE *f, FILE *f2, FILE *f3){
		if(f==NULL){
			printf("css������ ���� ���Ͽ����ϴ�.\n");
			system("pause");
			exit(1);
		}
		if(f2==NULL){
			printf("������ ���� ���Ͽ����ϴ�.\n");
			system("pause");
			exit(1);
		}
		if(f3==NULL){
			printf("�켱���� ������ ���� ���Ͽ����ϴ�.\n");
			system("pause");
			exit(1);
		}
}