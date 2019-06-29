
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//SYM：存放每个单词的类别，为内部编码的表示形式。
//ID：存放用户所定义的标识符的值，即标识符字符串的机内表示。
//NUM：存放用户定义的数。
#include <iostream>
#include <vector> 
#include <string> 
#include <fstream>  
using namespace std;

#define  size  120
char symTB[29][15] = {
	"const","var","procedure","begin","end","odd","if","then","call","while","do","read","write",//0-12
	";",",",".",":=","(",")","+","-","*","/","=","#""<","<=",">",">="//13-28
};//关键字与算符界符表
//数字  标识符
#define NUMBER 29
#define IDENT  30
char other[] = "--";
int sym = 0;//指向symTB的指针



char idTB[30][10] = { "" };//标识符表
int id = 0;//标识符表指针
char numTB[100][10] = { "" };//数字表
int num = 0;//数字表指针

char ch;//当前读入的字符
char strToken[15] = "";//当前读入的字符串
int p = 0;//当前读入的字符串的指针
int code, value;//返回信息

char x[15];
//******************************************
//判断是否为字母
bool IsLetter(char letter)
{
	if ((letter>='a'&&letter<='z') || (letter>='A'&&letter<='Z'))
		return true;
	else
		return false;

}
//******************************************
//判断是否为数字 
bool IsDigit(char digit)
{
	if (digit>='0'&&digit<='9')
		return true;
	else
		return false;
}
//******************************************
//查找关键字表，找到返回类别（位置+1）
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
//判断是否为算符或界符，找到返回位置+1，否则返回-1
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
//判断是否为算符或界符，找到返回位置+1，否则返回-1
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
//从标识符表中查找标识符，如果没有则插入，最后返回指针（位置）
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
//从数字表表中查找标识符，如果没有则插入，最后返回指针（位置）
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
//过滤注释，将大写转换为小写
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
		while (ch == ' '||ch == 10)//过滤空格与换行
			ch = sourfile->get();
		if (ch == EOF)
		{
			return 1;
		}


		if (IsLetter(ch))//读到的是字母，则要么是关键字，要么是标识符
		{
			strToken[p++] = ch;
			sourfile->get(ch);
			while (IsDigit(ch) || IsLetter(ch))//如果是数字或字母，则继续读取
			{
				strToken[p++] = ch;
				sourfile->get(ch);
			}
			sourfile->seekg(-1, ios::cur);//回退一格
			code = Retract(strToken);//查找关键字表
			if (code == -1)
			{//没找到，插入标识符表
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
			while (IsDigit(ch))//如果是数字，则继续读取
			{
				strToken[p++] = ch;
				sourfile->get(ch);
			}
			sourfile->seekg(-1, ios::cur);//回退一格

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
					sourfile->seekg(-1, ios::cur);//回退一格
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
					sourfile->seekg(-1, ios::cur);//回退一格
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
		cout << "未成功打开文件" << endl;
	}
	deal(&openfile, &outfile);//
	openfile.close();
	outfile.close();


	openfile.open("../ex1_1.txt");
	outfile.open("../lex.txt", ios::out);
	if (!openfile.is_open())
	{
		cout << "未成功打开文件" << endl;
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