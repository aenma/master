#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int attr_value(int ch, int str, char *line, FILE *f){   //�Ӽ��� ���� line�� ��� �Լ�
	int i;

	for(i=0; ch!='}';){    //ch�� }�� ���� ����������
		while((ch=fgetc(f)) != EOF){   //�ѱ��ھ�
			if(ch==';'){    // ; �̸�(�Ӽ�:�� �ϳ�) 
				str++; // ';' �Ӽ� ����
		}
		//-----------------7/14 ���� ���� ���� �κ� ------------------
		if(ch=='}'){     // } �̸� (���κ�)
			if(line[i-1] != ';'){   //�������Ӽ��� ��� ; ���� �ȳ����� ���
				if(str != 0){
					line[i] = ';';   //; �߰�
				}
				str++;  // ';' �Ӽ� ����
			}
			break; 
		}
			if(ch != ' '){   //������ �ƴϸ�
				line[i] = ch;   //�迭�� �ѱ��ھ� 
				i++;
			}
		}		 
	}
	return str;
}