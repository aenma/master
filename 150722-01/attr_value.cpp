#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int attr_value(int ch, int str, char *line, FILE *f){   //속성과 값을 line에 담는 함수
	int i;

	for(i=0; ch!='}';){    //ch가 }와 같지 않을때까지
		while((ch=fgetc(f)) != EOF){   //한글자씩
			if(ch==';'){    // ; 이면(속성:값 하나) 
				str++; // ';' 속성 갯수
		}
		//-----------------7/14 수정 오류 수정 부분 ------------------
		if(ch=='}'){     // } 이면 (끝부분)
			if(line[i-1] != ';'){   //마지막속성의 경우 ; 으로 안끝나는 경우
				if(str != 0){
					line[i] = ';';   //; 추가
				}
				str++;  // ';' 속성 갯수
			}
			break; 
		}
			if(ch != ' '){   //공백이 아니면
				line[i] = ch;   //배열에 한글자씩 
				i++;
			}
		}		 
	}
	return str;
}