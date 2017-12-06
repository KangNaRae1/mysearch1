#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <windows.h>
#include <string.h>

int search(char keyword[], char text[],int size)
{
	int n = 0;
	int i, j;
	int check;
	int length = strlen(keyword) - 1;
	for (i = 0;i+length<size; i = i - length + 1)
	{
		check = 0;
		for (j = 0; j < length; j++)
		{
			if (text[i] == keyword[j])
			{
				check++;
			}
			i++;
		}
		if (check == length)
		{
			n++;
			i = i + length;
		}
	}
	return n;
}

int arrange(int n, char name[])
{
	int i;
	int length = strlen(name);
	for (i = length + 1; i >0; i--)
	{
		name[i] = name[i - 2];
	}
	name[0] = n;
	name[1] = length;

	return length + 2;
}

int main()
{
	WIN32_FIND_DATA FindData;
	HANDLE hFind;
	FILE *fp;
	char enterpath[100];
	char path[100];
	char keyword[20];
	char name[5000][20];
	char loca[100];
	char interchange;
	char text[10000];
	int size[5000];
	int n, length, swap;
	int row = 0;
	int i, j, k;
	float KB;

	printf(" 검색할 파일들의 폴더 경로 입력 ex)C:\\aaa\\bbb\\dataset\n :");
	fgets(enterpath, sizeof(enterpath), stdin);
	enterpath[strlen(enterpath) - 1] = '\0';
	strcpy(path, enterpath);
	strcpy(loca, enterpath);
	strcat(path, "\\*.txt");
	
	printf("\n 검색할 키워드 입력: ");
	fgets(keyword, sizeof(keyword), stdin);
	_strlwr(keyword);

	

	hFind = FindFirstFile((LPCSTR)path, &FindData);
	if (hFind == INVALID_HANDLE_VALUE)
		return 0;

	do
	{
		strcpy(name[row], FindData.cFileName);
		strcpy(loca, enterpath);
		strcat(loca, "\\");
		strcat(loca, name[row]);
		fp = fopen(loca, "r");
		fseek(fp, 0L, SEEK_END);
		size[row] = ftell(fp);
		rewind(fp);
		fread(text, size[row], 1, fp);
		_strlwr(text);
		n = search(keyword, text, size[row]);
		length = arrange(n, name[row]);
		row++;
		fclose(fp);
	} while (FindNextFile(hFind, &FindData));


	FindClose(hFind);

	for (i = 0; i < row; i++)
	{
		for (j = i + 1; j < row; j++)
		{
			if (name[i][0] < name[j][0])
			{
				for (k = 0; k < length; k++)
				{
					interchange = name[i][k];
					name[i][k] = name[j][k];
					name[j][k] = interchange;
				}
				swap = size[i];
				size[i] = size[j];
				size[j] = swap;
			}
			if ((name[i][0] == name[j][0]) && (size[i] > size[j]))
			{
				for (k = 0; k < length; k++)
				{
					interchange = name[i][k];
					name[i][k] = name[j][k];
					name[j][k] = interchange;
				}
				swap = size[i];
				size[i] = size[j];
				size[j] = swap;
			}
		}
	}

	printf("\n-----------------------------------------------------------\n");
	for (i = 0; i < row; i++)
	{
		if (name[1][0] == 0)
			printf(" 검색결과를 찾을 수 없습니다.\n\n");
		if (name[i][0] == 0)
		{
			printf("\n");
			return 0;
		}

		for (j = 2; j<name[i][1] + 2; j++)
		{
			printf("%c", name[i][j]);
		}
		KB = (float)size[i] / 1024;

		printf("(%.2fKB)에서 %d개의 검색결과를 찾았습니다.", KB, name[i][0]);
		printf("\n");
	}
	printf("\n");
	system("pause");
	return 0;
}
