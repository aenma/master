#include <stdio.h>
#include <stdlib.h>

int value(int hs, int k, char *line, char *temp){    //�� temp�� �ִ� �Լ�
	int i;

	if(hs != 0){   //hs:�� ���ο��� ���� �ڸ� ��
		k = 0;
		for(i=hs+1; line[i] != ';'; i++){   //���ٿ��� ;�� ������ ������ (�����) 
			if(line[i] != ';'){   //;�� �ƴϸ� 
				temp[k] = line[i];   //temp�� ���� �� 
				k++;
			}
			hs++;
		}
		if(line[i] == ';'){   //���ٿ��� ;�̸� 
			hs = hs+2;
		}
	}

	return hs;
}