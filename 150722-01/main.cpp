#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "css_ssline.h"   //����ü
#include "deleteA.h"   //deleteA �Լ�
#include "file_error.h"   //file_error �Լ�
#include "attr_value.h"   //attr_value �Լ�
#include "value.h"   //value �Լ�
void csscount(csstable *ct, FILE *cfile);   //css.txt ������� ����Ʈ���� �Լ�
int sort(int str, ssline sline[800], ssline tmpline[800], int bn, FILE *f2);    //���� �� �����Ͽ� ����ϴ� �Լ�

void main(int argc, char *argv[]){
	FILE *f, *f2, *f3;
	char fname[50], fnamecpy[50], fname2[50], fname3[50], temp[500] = "", line[500]="", jstmp[500] = "", *tok1;
	int a=0, i, l=0, j=0, k=0, bn=0, str=0, ch, round=0, hs=0, sn=0;   //�ٲ�Ƚ�� üũ-bn, ������ ';'�Ѱ���(';'�� �Ӽ��� ������� ����)-str
  	ssline sline[800], tmpline[800];   //����ü �迭
	int js=0, js1=0, js2=0, csscontrol=0, namectrl;   //�ּ� �ߺ� ����(js), �ּ� ���ÿ�(js1), {}�±׾ȿ� �ּ��� ������ �� ; ���� �뵵(js2)

	while(1){
		if(argc==1){   //exe���ϸ� �Է� ��
			printf(" 1 : css ���� ���� 2 : ���� \n->");
			scanf("%d", &csscontrol);
			if( csscontrol == 2 ){   //���Ḧ ������
				break;   //while�� ����
			}
			puts("--------Ȯ���ڸ� ������ ���� �� �Է�--------");
			printf("���� �� css ���� �Է�.\n->");
			scanf("%s", fname);
			puts(" ���� �� ���� �̸� ");
			printf(" 1 : %s-out.css 2 : ���� �Է� \n", fname);
			scanf("%d", &namectrl);
			switch ( namectrl ){   //����� �����̸� �з�
				case 1 :    //���ϸ�-out ���
					strcpy(fname3,fname);
					strcpy(fname2,strcat(fname3,"-out"));
					break;
				case 2 :    //���� �Է�
					printf("���� �̸� �Է�.\n->");
					scanf("%s", fname2);
					break;
			}
			strcpy(fname,strcat(fname,".css"));   //�ڿ� Ȯ���� css�� �ٿ��� ����
			strcpy(fname2,strcat(fname2,".css"));
		}
		else if(argc==2){   //exe���ϰ� �Ӽ����� ������ ���� �Է� ��(2���Է�)
			strcpy(fname, argv[1]);   //argv[0]�� exe�����̸�
			puts(" ���� �� ���� �̸� ");
			strcpy(fnamecpy, fname);   //.css Ȯ���ڱ��� �Է��� �Ǿ����Ƿ� cssȮ���ڸ� strtok�� �и��ϱ����� �̸� ����(������ �����ǹǷ�)
			tok1=strtok(fnamecpy, ".css");   //.css�� �и����� �����̸��� tok1�� ����
			printf(" 1 : %s-out.css 2 : ���� �Է� 3. ���� \n", tok1);
			scanf("%d", &namectrl);
			switch ( namectrl ){   //����� �����̸� �з�
				case 1 :    //���ϸ�-out ���
					strcpy(fname3,tok1);
					strcpy(fname2,strcat(fname3,"-out"));
					break;
				case 2 :    //���� �Է�
					printf("���� �̸� �Է�.\n->");
					scanf("%s", fname2);
					break;
				case 3:
					printf("����\n");
					exit(1);
					break;
			}
			strcpy(fname2,strcat(fname2,".css"));   //Ȯ���� ����, ���⼭ fname�� cssȮ���ڰ� �ִ°��� �̸� ������ �ξ �״�� ���
		}

		f=fopen(fname, "r");   //�Ӽ��� ������ ������ ����
		f2=fopen(fname2,"w");   //�Ӽ��� ������ �����Ȱ��� ������ ����
		f3=fopen("css.txt", "r");   //�Ӽ��� ������ �ִ� �ؽ�Ʈ����
		file_error(f, f2, f3);    //���Ͽ��� ����üũ �Լ�ȣ��

		//csstable ����
	  	csstable *ct=NULL;   //����ü������ 
	  	ct = (csstable *)malloc(sizeof(csstable));   //�����Ҵ� 
	  	ct->next=NULL;   //null�� �ʱ�ȭ 
	  	csscount(ct, f3);    //�Լ�ȣ�� 
	  	csstable *pp;   //����ü������
	
		while((ch=fgetc(f)) != EOF){   //�ѱ��ھ�
			if(ch == '{'){     // { �̸� (���ۺκ�) 
				fputc(ch, f2);    // { ���
			  	str=attr_value(ch, str, line, f);   //�Ӽ��� ���� line�� ��� �Լ�ȣ��
				printf("\n{%s}\n",line);   //line���� {}������ �Ӽ��� �� ��� ��

				//-----------------7.17 �ּ� �� ������--------------------
				k=0;
				a=-1;
				js = -1;
				deleteA(temp);
				for(i=0; line[i] != NULL; i++){
					if(line[i] == ';'){    //; ���ϳ�
						a++;
						strcpy(sline[a].sjs, "");    //�� ������ �ּ��� �ʱ�ȭ
					}
					if(line[i] == '/' && line[i+1] == '*'){   //�ּ� ���ۺκ�
						for(j = i; j<200; j++){ // 200�� �ӽ÷� �� ��.
							temp[k] = line[i];   //�ּ������� temp��
							k++;
							i++;
							if(line[i-1] == '/' && line[i-2] == '*'){   //�ּ� ���κ�
								break;
							}
						}
						if(a == -1){
							strcpy(sline[0].sjs,temp);   //����ü �ּ��� ����(0��°��)
							str = 1;
							js2 = 1;
						} else if(!strcmp(sline[a].sjs, "")){   //�� �Ӽ��� ������ ������ �� �ּ��� ������ ����(""�� �ʱ�ȭ ������ ��츸 ����)
							strcpy(sline[a].sjs, temp);      //����ü �ּ��� ����
						}
						deleteA(temp);
						k=0;
					}
				}
	
				if ( js2 == 1 ) {    //{ } �ȿ� �ּ��� 1���� ������ ��� �ڿ� ; ����
					for(i=0; line[0] != NULL; i++){
						if(line[i-1] == '/' && line[i-2] == '*'){
							line[i] = NULL;
							break;
						}
					}
				}
				k=0;
				a=0;
				//-----------------------

			  	if(str==1){   //;�� �ϳ�->�Ӽ��� ���� �ϳ��� ����
					fputs(line, f2);  //�ϳ��̹Ƿ� �񱳾��� �״�� ��� 
				} else if ( str > 1 ){   //;�� �ϳ� �̻�=>�Ӽ��� �ϳ� �̻�
				  	for(j=0; j<str; j++){
					 	deleteA(temp);

					  	//�Ӽ� ������
					  	if(hs==0 || round == 0){   //hs:�� ���ο��� ���� �ڸ� ��
						  	k=0;
						  	for(i=hs; line[i] != ':' ; i++){   //���ٿ��� :�� ������ ������ (�Ӽ����)
							  	if(line[hs] != ':'){   //�Ѷ��ο��� ���� �ڸ����� hs�ڸ��� :�� �ƴϸ�
									if(line[hs] == ';' || line[hs+1] == ';'){
										break;
									}
	
									if(line[hs] == '/' && line[hs+1] == '*'){    //�ּ��� ���
										line[hs] = ' ';     //�̹� �ּ��� �� �� �Ӽ����� ����ü�� ������ �Ǿ������Ƿ� �ּ��κ��� ����
										js1 = hs;    //���� hs�� ��ġ�� js1�� ����
										for(js=0; line[js1] != '/'; js++){       // /*�� ���������Ƿ� ���� / �� �ּ��� ���������� �ν�
											//printf("%d\n", js);    //��ĭ�̳� �̵����� Ƚ��
											line[js1] = ' ';	  //�ּ��� ���� �� ���� �ּ��� �������� ����
											js1++;  //��ĭ�� �ڷ� �̵�
										}
										line[js1] = ' ';	//������ / �� �������� ����
									}
	
									if(line[hs] != '\n' && line[hs] != '\t' && line[hs] != ' ') { 									
										temp[k] = line[hs];   //temp�� �Ӽ��� �� 
										k++;
									}
							 	}
							  	hs++;
						  	}
					  	}
						
						if(strcmp(temp,"")){
							strcpy(sline[sn].stitle, temp);   //temp�� �ִ� ���� ����ü����sline���� sn�ڸ��� stitle�� ����(�Ӽ����) 
						}
					  	printf("head : [%s]\n", temp);   //�Ӽ� 
						deleteA(temp);	//���� for������ temp�� ��� ���ؼ� ���� temp�� ��� 
						  
						//��
					  	hs=value(hs, k, line, temp);    //�� temp�� �ִ� �Լ�ȣ��
					  	strcpy(sline[sn].svalue,temp);   //temp���� ����ü����sline�� sn�ڸ��� svalue�� ����(�����)
					  	printf("value : [%s]\n", temp);   //��
					  	sn++;   //����ü�迭�� �ε��� ����
				  	}
				  	
					for(i=0; i<str; i++){   //�� �迭�� �ѹ� ����
						pp = ct;    //csstable�� ó������ �˻��ϱ����ؼ� �ʱ�ȭ, css.txt�Ӽ�����Ʈ�� ��ó�� 
						while(pp->next != NULL){   //pp�� ����Ű�� ������ �ִµ��ȿ�
							if(!strcmp(sline[i].stitle,pp->next->tagname)){   //���ڿ���- sline�� stitle(�Ӽ�)�� pp�� ����Ű�� ������ tagname(�Ӽ�)- ������
								sline[i].num = pp->next->tagnum;   //sline�� num�� pp�� ����Ű�� ������ tagnum�� ���� 
								break;
							}
							pp = pp->next;   //�� ���� 
						}
					}
				  	
				  	bn=sort(str, sline, tmpline, bn, f2);   //���� �� �����Ͽ� ����ϴ� �Լ� ȣ��
			  	}
			  	fputc(ch,f2);   // ' } ' 
		  	} else{   //�ƴϸ� �׳� �״�� ��� 
			  	fputc(ch, f2);   //�±׵� ���⼭ ��µ� 
		  	}
			deleteA(line);
			js2 = 0;
			str=0;
			sn=0;
			round=0;
			hs=0;
		}
	  	
	  	if(feof(f) != 0)    //����Ϸ� 
	    	puts("\n\n----���� OK\n\n");
		else			    //����X
	    	puts("\n\n----���� NO\n\n");
	
	 	printf("�ٲ� Ƚ�� : %d\n", bn);
		printf("�Ϸ�. ������ Ȯ���ϼ���\n\n\n");
		bn = 0;
	
		fclose(f);
		fclose(f2);
		fclose(f3);
	}
	printf("���α׷��� �����մϴ�.\n");
}

void csscount(csstable *ct, FILE *cfile){   //css.txt������ �Ӽ����� ����ü�� ������ ����Ʈ�� ������� ���� 
	int en, i, score=1;   //�ѱ��ھ� ����-en
	char ctag[200] = "";
	csstable *cp, *temp;

	while(!feof(cfile)){   //css.txt->cfile
		en = fgetc(cfile);
		for(i=0; en != ',' && en != ' ' && en != '\n'; i++){   //en(�ѱ��ھ� ������)�� ,�� �����̳� ���Ͱ� �ƴҶ�-> �Ӽ��ϳ� �и� 
			ctag[i] = en;   //�Ӽ��ϳ��� ctag�� ��� 
			en = fgetc(cfile);   //�ٽ� �ѱ��ھ� 
		}

		cp = (csstable*)malloc(sizeof(csstable));   //����ü ������ �����Ҵ� 
		if(cp == NULL) exit(1);

		if(ctag[0] != NULL){   //ctag�� ���� ������(null�� �ƴҶ�) 
			strcpy(cp->tagname,ctag);   //ctag�� �ִ� ��(�Ӽ��ϳ�)�� ����ü�� tagname�� 
			cp->tagnum = score;   //score�ʱⰪ 1�̹Ƿ� ó������ �Ӽ��� �ϳ��ϳ� ��� ������� ����Ʈ�� ������ ����ü�� ���� 
			cp->next = NULL;

			if(ct->next == NULL){   //null�� ��-> ��ó���� �� �ֱ� ���� �Ǵ�  ������ �κ��� �� �ڿ� �߰� 
				ct->next = cp;
			} else if (ct->next != NULL){   //null�� �ƴϸ�
				temp = ct->next;   //����ü ������ temp�� ��ó���� ct->next�� ���� 
				while(temp->next != NULL){   //��ó������ ��� ���Ŀ��� 
					temp = temp->next;   //�״����� �� temp�� ����Ű�� 
				}   //�������� ���Ŀ��� 
				temp->next = cp;   //�������� ������ cp�� �߰� 
			}
			score++;   //tagnum�� ���� ������ ���� 
		}
		if(!strcmp(ctag,"orientation")){
			deleteA(ctag);
			break;
		}
		deleteA(ctag);
	}
}

int sort(int str, ssline sline[800], ssline tmpline[800], int bn, FILE *f2){    //���� �� �����Ͽ� ����ϴ� �Լ�
	int i, j;

	//��������
	/*for(i=0; i<str; i++){
		printf("before : %d\n",sline[i].num); //���� �ѹ� Ȯ�ο�
	}*/
	for(i=0; i<str; i++){   //str->; ���� 
		for(j=i+1; j<str; j++){
			if(sline[i].num > sline[j].num){   //�տ� �ִ� num�� �� ũ�� ���� 
				*tmpline = sline[i];
				sline[i] = sline[j];
				sline[j] = *tmpline;
				bn++;  //�ٲ� Ƚ�� 
			}
		}
	}

	/*for(i=0; i<str; i++){
		printf("after : %d\n",sline[i].num); //���� �� �ѹ� Ȯ�ο�
	}*/
	//{ ���� } ���;
	for(i=0; i<str; i++){  //str->; ����
		if(strcmp(sline[i].stitle,"") && strcmp(sline[i].svalue,"")){						
			fputs(sline[i].stitle,f2);						
			fputc(':',f2);					
			fputs(sline[i].svalue,f2);
			fputc(';',f2);
		}
		if(strcmp(sline[i].sjs, "")){
			fputs(sline[i].sjs,f2);
		}
	}

	for(i=0; i<str; i++){
		strcpy(sline[i].stitle,"");
		strcpy(sline[i].svalue,"");
		strcpy(sline[i].sjs,"");
		sline[i].num = NULL;
	}

	return bn;
}