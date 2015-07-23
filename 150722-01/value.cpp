#include <stdio.h>
#include <stdlib.h>

int value(int hs, int k, char *line, char *temp){    //값 temp에 넣는 함수
	int i;

	if(hs != 0){   //hs:한 라인에서 읽은 자리 수
		k = 0;
		for(i=hs+1; line[i] != ';'; i++){   //한줄에서 ;를 만나기 전까지 (값담기) 
			if(line[i] != ';'){   //;이 아니면 
				temp[k] = line[i];   //temp에 값이 들어감 
				k++;
			}
			hs++;
		}
		if(line[i] == ';'){   //한줄에서 ;이면 
			hs = hs+2;
		}
	}

	return hs;
}