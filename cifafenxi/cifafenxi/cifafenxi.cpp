
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//SYM�����ÿ�����ʵ����Ϊ�ڲ�����ı�ʾ��ʽ��
//ID������û�������ı�ʶ����ֵ������ʶ���ַ����Ļ��ڱ�ʾ��
//NUM������û����������
#include <iostream>
#include <vector> 
#include <string> 
#include <fstream>  
using namespace std;

#define  size  120
char symTB[29][15] = {
	"const","var","procedure","begin","end","odd","if","then","call","while","do","read","write",//0-12
	";",",",".",":=","(",")","+","-","*","/","=","#""<","<=",">",">="//13-28
};//�ؼ�������������
//����  ��ʶ��
#define NUMBER 29
#define IDENT  30
char other[] = "--";
int sym = 0;//ָ��symTB��ָ��



char idTB[30][10] = { "" };//��ʶ����
int id = 0;//��ʶ����ָ��
char numTB[100][10] = { "" };//���ֱ�
int num = 0;//���ֱ�ָ��

char ch;//��ǰ������ַ�
char strToken[15] = "";//��ǰ������ַ���
int p = 0;//��ǰ������ַ�����ָ��
int code, value;//������Ϣ

char x[15];
//******************************************
//�ж��Ƿ�Ϊ��ĸ
bool IsLetter(char letter)
{
	if ((letter>='a'&&letter<='z') || (letter>='A'&&letter<='Z'))
		return true;
	else
		return false;

}
//******************************************
//�ж��Ƿ�Ϊ���� 
bool IsDigit(char digit)
{
	if (digit>='0'&&digit<='9')
		return true;
	else
		return false;
}
//******************************************
//���ҹؼ��ֱ��ҵ��������λ��+1��
int Retract(char*te)
{
	for (int i = 0; i < 13; i++)
	{
		if (strcmp(te, symTB[i]) == 0)
		{
			return i+1;
		}
	}
	return -1;
}
//******************************************
//�ж��Ƿ�Ϊ����������ҵ�����λ��+1�����򷵻�-1
int IsSoj(char soj)
{
	
	for (int i = 13; i < 29; i++)
	{
		if (soj == symTB[i][0])
		{
			return i + 1;
		}
		
	}
	return -1;
}
//******************************************
//�ж��Ƿ�Ϊ����������ҵ�����λ��+1�����򷵻�-1
int IsSoj(char *soj)
{

	for (int i = 13; i < 29; i++)
	{
		if (strcmp(soj,symTB[i])==0)
		{
			return i + 1;
		}

	}
	return -1;
}
//******************************************
//�ӱ�ʶ�����в��ұ�ʶ�������û������룬��󷵻�ָ�루λ�ã�
int InsertId(char*te)
{
	for (int i = 0; i < id; i++)
	{
		if (strcmp(te, idTB[i]) == 0)
		{
			return i;
		}
	}
	strcpy_s(idTB[id++],strlen(te)+1, te);
	return id - 1 ;
	
}
//******************************************
//�����ֱ���в��ұ�ʶ�������û������룬��󷵻�ָ�루λ�ã�
int InsertNum(char*te)
{
	for (int i = 0; i < id; i++)
	{
		if (strcmp(te, numTB[i]) == 0)
		{
			return i;
		}
	}
	strcpy_s(numTB[num++], strlen(te) + 1, te);
	return num - 1;

}
char tran(char te)
{
	if (te >= 'A'&&te <= 'Z')
	{
		te = te + 32;
		return te;
	}
	else
		return te;
}
//******************************************
//����ע�ͣ�����дת��ΪСд
void deal(ifstream *sourfile, ofstream *destfile)
{
	char tempx;

	do
	{
		tempx = sourfile->get();
		if (tempx == '/')
		{
			char tempy = sourfile->get();
			if (tempy == '/')
			{
				while (sourfile->get() != 10);
				tempx = sourfile->get();
			}
			else if (tempy == '*')
			{
				char tempz1 = sourfile->get();
				char tempz2 = sourfile->get();

				while (!(tempz1 == '*'&&tempz2 == '/'))
				{

					tempz1 = tempz2;
					tempz2 = sourfile->get();
					if (tempz1 == EOF || tempz2 == EOF)
					{
						tempx = EOF;
						break;
					}
					
				}

			}
			else
			{

				*destfile << tran(tempx) << tran(tempy);
				
			}
		}
		else
		{
			//if (tempx != 10 && tempx != 13 && tempx != 9 && tempx != EOF )
			if (tempx != EOF)
				*destfile << tran(tempx);
		}
	} while (tempx != EOF);
	return;
}
//******************************************
//
int getsym(ifstream *sourfile, ofstream *destfile)
{
	ch = ' ';
	while (ch != EOF)
	{
		p = 0;
		sourfile->get(ch);
		while (ch == ' '||ch == 10)//���˿ո��뻻��
			ch = sourfile->get();
		if (ch == EOF)
		{
			return 1;
		}


		if (IsLetter(ch))//����������ĸ����Ҫô�ǹؼ��֣�Ҫô�Ǳ�ʶ��
		{
			strToken[p++] = ch;
			sourfile->get(ch);
			while (IsDigit(ch) || IsLetter(ch))//��������ֻ���ĸ���������ȡ
			{
				strToken[p++] = ch;
				sourfile->get(ch);
			}
			sourfile->seekg(-1, ios::cur);//����һ��
			code = Retract(strToken);//���ҹؼ��ֱ�
			if (code == -1)
			{//û�ҵ��������ʶ����
				value = InsertId(strToken);
				sprintf_s(x,100,"%-15s%-15d%-15d\n", idTB[value], IDENT, value);
				printf(x);
				*destfile << x;
			}
			else
			{
				sprintf_s(x,100,"%-15s%-15d%-15s\n", symTB[code - 1], code, other);
				printf(x);
				*destfile << x;
			}

		}
		else if (IsDigit(ch))
		{
			strToken[p++] = ch;
			sourfile->get(ch);
			while (IsDigit(ch))//��������֣��������ȡ
			{
				strToken[p++] = ch;
				sourfile->get(ch);
			}
			sourfile->seekg(-1, ios::cur);//����һ��

			value = InsertNum(strToken);
			string temp(strToken);
			int num = atoi(temp.c_str());
			sprintf_s(x, 100, "%-15s%-15d%-15d\n", numTB[value], NUMBER, value);
			printf(x);
			*destfile << x;


			



			//cout << number;
			
		}
		else if((code=IsSoj(ch))!=-1)
		{
			strToken[p++] = ch;
			if (ch == ':')
			{
				sourfile->get(ch);
				if (ch == '=')
				{
					strToken[p++] = ch;
					sprintf_s(x,100,"%-15s%-15d%-15s\n", strToken, code, other);
					printf(x);
					*destfile << x;
				}
				else
				{
					sourfile->seekg(-1, ios::cur);//����һ��
					printf("error\n");
					return -1;
				}
			}
			else if (ch == '>' || ch == '<')
			{
				sourfile->get(ch);
				if (ch == '=')
				{
					
					strToken[p++] = ch;
					code = IsSoj(strToken);
					sprintf_s(x,100,"%-15s-15d%-15s\n", strToken, code, other);
					printf(x);
					*destfile << x;
				}
				else
				{
					sourfile->seekg(-1, ios::cur);//����һ��
					sprintf_s(x,100,"%-15s%-15d%-15s\n", strToken, code, other);
					printf(x);
					*destfile << x;
				}
			}
			else
			{
				sprintf_s(x, 100, "%-15s%-15d%-15s\n", strToken, code, other);
				printf(x);
				*destfile << x;
			}
			
		}
		else
		{
			printf("error\n");
			return -1;
		}
		ch = ' ';
		memset(strToken, 0, sizeof(strToken));
		memset(x, 0, sizeof(x));
	
	}
	return 1;


}



int main()
{
	ifstream openfile("../ex1.txt");
	ofstream outfile("../ex1_1.txt", ios::out);
	if (!openfile.is_open())
	{
		cout << "δ�ɹ����ļ�" << endl;
	}
	deal(&openfile, &outfile);//
	openfile.close();
	outfile.close();


	openfile.open("../ex1_1.txt");
	outfile.open("../lex.txt", ios::out);
	if (!openfile.is_open())
	{
		cout << "δ�ɹ����ļ�" << endl;
	}
	getsym(&openfile, &outfile);//
	openfile.close();
	outfile.close();
	printf("\n\n\n\n");
	printf("id table\n");
	for (int i = 0; i < id; i++)
	{
		printf("%-15s%-15d\n", idTB[i], i);
	}
	printf("\n\n\n\n");
	printf("num table\n");
	for (int i = 0; i < num; i++)
	{
		printf("%-15s%-15d\n", numTB[i], i);
	}

	system("pause");


	return 0;
}