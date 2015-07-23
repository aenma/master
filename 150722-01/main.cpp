#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "css_ssline.h"   //구조체
#include "deleteA.h"   //deleteA 함수
#include "file_error.h"   //file_error 함수
#include "attr_value.h"   //attr_value 함수
#include "value.h"   //value 함수
void csscount(csstable *ct, FILE *cfile);   //css.txt 순서대로 리스트로한 함수
int sort(int str, ssline sline[800], ssline tmpline[800], int bn, FILE *f2);    //정렬 후 새파일에 출력하는 함수

void main(int argc, char *argv[]){
	FILE *f, *f2, *f3;
	char fname[50], fnamecpy[50], fname2[50], fname3[50], temp[500] = "", line[500]="", jstmp[500] = "", *tok1;
	int a=0, i, l=0, j=0, k=0, bn=0, str=0, ch, round=0, hs=0, sn=0;   //바뀐횟수 체크-bn, 라인의 ';'총갯수(';'로 속성이 몇개인지를 구분)-str
  	ssline sline[800], tmpline[800];   //구조체 배열
	int js=0, js1=0, js2=0, csscontrol=0, namectrl;   //주석 중복 방지(js), 주석 무시용(js1), {}태그안에 주석만 존재할 시 ; 제거 용도(js2)

	while(1){
		if(argc==1){   //exe파일만 입력 시
			printf(" 1 : css 파일 정리 2 : 종료 \n->");
			scanf("%d", &csscontrol);
			if( csscontrol == 2 ){   //종료를 누르면
				break;   //while문 종료
			}
			puts("--------확장자를 제외한 파일 명 입력--------");
			printf("정리 할 css 파일 입력.\n->");
			scanf("%s", fname);
			puts(" 저장 될 파일 이름 ");
			printf(" 1 : %s-out.css 2 : 임의 입력 \n", fname);
			scanf("%d", &namectrl);
			switch ( namectrl ){   //저장될 파일이름 분류
				case 1 :    //파일명-out 사용
					strcpy(fname3,fname);
					strcpy(fname2,strcat(fname3,"-out"));
					break;
				case 2 :    //새로 입력
					printf("파일 이름 입력.\n->");
					scanf("%s", fname2);
					break;
			}
			strcpy(fname,strcat(fname,".css"));   //뒤에 확장자 css를 붙여서 복사
			strcpy(fname2,strcat(fname2,".css"));
		}
		else if(argc==2){   //exe파일과 속성순서 정리할 파일 입력 시(2개입력)
			strcpy(fname, argv[1]);   //argv[0]은 exe파일이름
			puts(" 저장 될 파일 이름 ");
			strcpy(fnamecpy, fname);   //.css 확장자까지 입력이 되었으므로 css확장자를 strtok로 분리하기위해 미리 복사(원본이 변형되므로)
			tok1=strtok(fnamecpy, ".css");   //.css를 분리시켜 파일이름만 tok1에 담음
			printf(" 1 : %s-out.css 2 : 임의 입력 3. 종료 \n", tok1);
			scanf("%d", &namectrl);
			switch ( namectrl ){   //저장될 파일이름 분류
				case 1 :    //파일명-out 사용
					strcpy(fname3,tok1);
					strcpy(fname2,strcat(fname3,"-out"));
					break;
				case 2 :    //새로 입력
					printf("파일 이름 입력.\n->");
					scanf("%s", fname2);
					break;
				case 3:
					printf("종료\n");
					exit(1);
					break;
			}
			strcpy(fname2,strcat(fname2,".css"));   //확장자 붙임, 여기서 fname은 css확장자가 있는것을 미리 복사해 두어서 그대로 사용
		}

		f=fopen(fname, "r");   //속성의 순서를 정리할 파일
		f2=fopen(fname2,"w");   //속성의 순서가 정리된것을 저장할 파일
		f3=fopen("css.txt", "r");   //속성의 순서가 있는 텍스트파일
		file_error(f, f2, f3);    //파일오픈 에러체크 함수호출

		//csstable 생성
	  	csstable *ct=NULL;   //구조체포인터 
	  	ct = (csstable *)malloc(sizeof(csstable));   //동적할당 
	  	ct->next=NULL;   //null로 초기화 
	  	csscount(ct, f3);    //함수호출 
	  	csstable *pp;   //구조체포인터
	
		while((ch=fgetc(f)) != EOF){   //한글자씩
			if(ch == '{'){     // { 이면 (시작부분) 
				fputc(ch, f2);    // { 출력
			  	str=attr_value(ch, str, line, f);   //속성과 값을 line에 담는 함수호출
				printf("\n{%s}\n",line);   //line에는 {}사이의 속성과 값 모두 들어감

				//-----------------7.17 주석 값 빼내기--------------------
				k=0;
				a=-1;
				js = -1;
				deleteA(temp);
				for(i=0; line[i] != NULL; i++){
					if(line[i] == ';'){    //; 값하나
						a++;
						strcpy(sline[a].sjs, "");    //각 라인의 주석값 초기화
					}
					if(line[i] == '/' && line[i+1] == '*'){   //주석 시작부분
						for(j = i; j<200; j++){ // 200은 임시로 준 값.
							temp[k] = line[i];   //주석내용을 temp에
							k++;
							i++;
							if(line[i-1] == '/' && line[i-2] == '*'){   //주석 끝부분
								break;
							}
						}
						if(a == -1){
							strcpy(sline[0].sjs,temp);   //구조체 주석에 저장(0번째에)
							str = 1;
							js2 = 1;
						} else if(!strcmp(sline[a].sjs, "")){   //한 속성이 끝나기 전까지 또 주석을 만나면 무시(""로 초기화 되있을 경우만 복사)
							strcpy(sline[a].sjs, temp);      //구조체 주석에 저장
						}
						deleteA(temp);
						k=0;
					}
				}
	
				if ( js2 == 1 ) {    //{ } 안에 주석이 1개만 존재할 경우 뒤에 ; 제거
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

			  	if(str==1){   //;이 하나->속성과 값이 하나만 존재
					fputs(line, f2);  //하나이므로 비교없이 그대로 출력 
				} else if ( str > 1 ){   //;이 하나 이상=>속성이 하나 이상
				  	for(j=0; j<str; j++){
					 	deleteA(temp);

					  	//속성 나누기
					  	if(hs==0 || round == 0){   //hs:한 라인에서 읽은 자리 수
						  	k=0;
						  	for(i=hs; line[i] != ':' ; i++){   //한줄에서 :를 만나기 전까지 (속성담기)
							  	if(line[hs] != ':'){   //한라인에서 읽은 자리수인 hs자리가 :가 아니면
									if(line[hs] == ';' || line[hs+1] == ';'){
										break;
									}
	
									if(line[hs] == '/' && line[hs+1] == '*'){    //주석인 경우
										line[hs] = ' ';     //이미 주석이 다 각 속성별로 구조체에 저장이 되어있으므로 주석부분을 제거
										js1 = hs;    //현재 hs의 위치를 js1에 담음
										for(js=0; line[js1] != '/'; js++){       // /*로 시작했으므로 이후 / 는 주석의 마지막으로 인식
											//printf("%d\n", js);    //몇칸이나 이동한지 횟수
											line[js1] = ' ';	  //주석이 끝날 때 까지 주석을 공백으로 지움
											js1++;  //한칸씩 뒤로 이동
										}
										line[js1] = ' ';	//마지막 / 도 공백으로 없앰
									}
	
									if(line[hs] != '\n' && line[hs] != '\t' && line[hs] != ' ') { 									
										temp[k] = line[hs];   //temp에 속성이 들어감 
										k++;
									}
							 	}
							  	hs++;
						  	}
					  	}
						
						if(strcmp(temp,"")){
							strcpy(sline[sn].stitle, temp);   //temp에 있는 것을 구조체변수sline에서 sn자리에 stitle에 복사(속성담기) 
						}
					  	printf("head : [%s]\n", temp);   //속성 
						deleteA(temp);	//다음 for문에서 temp에 담기 위해서 먼저 temp를 비움 
						  
						//값
					  	hs=value(hs, k, line, temp);    //값 temp에 넣는 함수호출
					  	strcpy(sline[sn].svalue,temp);   //temp값을 구조체변수sline의 sn자리의 svalue에 복사(값담기)
					  	printf("value : [%s]\n", temp);   //값
					  	sn++;   //구조체배열의 인덱스 증가
				  	}
				  	
					for(i=0; i<str; i++){   //각 배열에 넘버 저장
						pp = ct;    //csstable을 처음부터 검색하기위해서 초기화, css.txt속성리스트의 맨처음 
						while(pp->next != NULL){   //pp가 가리키는 다음이 있는동안에
							if(!strcmp(sline[i].stitle,pp->next->tagname)){   //문자열비교- sline의 stitle(속성)와 pp가 가리키는 다음의 tagname(속성)- 같으면
								sline[i].num = pp->next->tagnum;   //sline의 num에 pp가 가리키는 다음의 tagnum을 넣음 
								break;
							}
							pp = pp->next;   //그 다음 
						}
					}
				  	
				  	bn=sort(str, sline, tmpline, bn, f2);   //정렬 후 새파일에 출력하는 함수 호출
			  	}
			  	fputc(ch,f2);   // ' } ' 
		  	} else{   //아니면 그냥 그대로 출력 
			  	fputc(ch, f2);   //태그도 여기서 출력됨 
		  	}
			deleteA(line);
			js2 = 0;
			str=0;
			sn=0;
			round=0;
			hs=0;
		}
	  	
	  	if(feof(f) != 0)    //변경완료 
	    	puts("\n\n----변경 OK\n\n");
		else			    //변경X
	    	puts("\n\n----변경 NO\n\n");
	
	 	printf("바뀐 횟수 : %d\n", bn);
		printf("완료. 파일을 확인하세요\n\n\n");
		bn = 0;
	
		fclose(f);
		fclose(f2);
		fclose(f3);
	}
	printf("프로그램을 종료합니다.\n");
}

void csscount(csstable *ct, FILE *cfile){   //css.txt파일의 속성들을 구조체에 저장해 리스트로 순서대로 연결 
	int en, i, score=1;   //한글자씩 저장-en
	char ctag[200] = "";
	csstable *cp, *temp;

	while(!feof(cfile)){   //css.txt->cfile
		en = fgetc(cfile);
		for(i=0; en != ',' && en != ' ' && en != '\n'; i++){   //en(한글자씩 읽은것)이 ,나 공백이나 엔터가 아닐때-> 속성하나 분리 
			ctag[i] = en;   //속성하나가 ctag에 담김 
			en = fgetc(cfile);   //다시 한글자씩 
		}

		cp = (csstable*)malloc(sizeof(csstable));   //구조체 포인터 동적할당 
		if(cp == NULL) exit(1);

		if(ctag[0] != NULL){   //ctag에 값이 있을때(null이 아닐때) 
			strcpy(cp->tagname,ctag);   //ctag에 있는 값(속성하나)을 구조체의 tagname에 
			cp->tagnum = score;   //score초기값 1이므로 처음부터 속성을 하나하나 담아 순서대로 리스트로 연결해 구조체에 저장 
			cp->next = NULL;

			if(ct->next == NULL){   //null일 때-> 맨처음에 값 넣기 시작 또는  마지막 부분일 때 뒤에 추가 
				ct->next = cp;
			} else if (ct->next != NULL){   //null이 아니면
				temp = ct->next;   //구조체 포인터 temp에 맨처음인 ct->next를 담음 
				while(temp->next != NULL){   //맨처음값이 담긴 이후에는 
					temp = temp->next;   //그다음을 또 temp가 가리키고 
				}   //마지막꺼 이후에는 
				temp->next = cp;   //마지막꺼 다음에 cp를 추가 
			}
			score++;   //tagnum에 넣을 순서를 증가 
		}
		if(!strcmp(ctag,"orientation")){
			deleteA(ctag);
			break;
		}
		deleteA(ctag);
	}
}

int sort(int str, ssline sline[800], ssline tmpline[800], int bn, FILE *f2){    //정렬 후 새파일에 출력하는 함수
	int i, j;

	//선택정렬
	/*for(i=0; i<str; i++){
		printf("before : %d\n",sline[i].num); //이전 넘버 확인용
	}*/
	for(i=0; i<str; i++){   //str->; 개수 
		for(j=i+1; j<str; j++){
			if(sline[i].num > sline[j].num){   //앞에 있는 num이 더 크면 변경 
				*tmpline = sline[i];
				sline[i] = sline[j];
				sline[j] = *tmpline;
				bn++;  //바뀐 횟수 
			}
		}
	}

	/*for(i=0; i<str; i++){
		printf("after : %d\n",sline[i].num); //정렬 후 넘버 확인용
	}*/
	//{ 내용 } 출력;
	for(i=0; i<str; i++){  //str->; 개수
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