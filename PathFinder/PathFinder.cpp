// PathFinder.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdlib.h>


#include <iostream>
using namespace std;

#define MAX_ROW_COUNT	200
#define COLUMN_COUNT	16
#define DIGIT_COUNT		3

void generatePermList(int *perm_list, int level, int &count)
{
	if( level == DIGIT_COUNT )
	{
		count++;
		return;
	}

	int i = 0;
	for(i = 1; i <= COLUMN_COUNT; i++)
	{
		if( i > 1 )
			memcpy(perm_list + DIGIT_COUNT * count,  perm_list + DIGIT_COUNT * (count - 1), sizeof(int) * level);

		perm_list[DIGIT_COUNT * count + level] = i;
		
		generatePermList(perm_list, level + 1, count);
	}
}

int* parseValue(char *val)
{
	if( val == NULL || strlen(val) < 1 )
		return NULL;

	int count = strlen(val);
	int* x = (int *)calloc(count, sizeof(int));

	int i = 0; 

	for(i = 0; i < count; i++)
		x[i] = val[i] - 48;

	return x;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int i = 0, j = 0;
	int PERM_TOTAL_COUNT  = DIGIT_COUNT;

	for(i = 0; i < DIGIT_COUNT; i++)
		PERM_TOTAL_COUNT *= COLUMN_COUNT;

	int *perm_list = (int*)calloc(PERM_TOTAL_COUNT, sizeof(int));
	if( perm_list == NULL )
		return -1;

	// generate perm list
	int count = 0;
	generatePermList(perm_list, 0, count);

	//for(i = 0; i < PERM_TOTAL_COUNT; i += DIGIT_COUNT )
	//{
	//	for(j = 0; j < DIGIT_COUNT; j++)
	//		cout << perm_list[i + j];
	//	cout << endl;
	//}

	int row_count = 12;
	char x[][COLUMN_COUNT + 1] = {
		"1213232312321122",
		"3232111213213121",
		"2111323231231213",
		"2323112323121213",
		"1213233321231231",
		"2111323132231222",
		"1123133212311123",
		"3233123132313211",
		"3112312312323121",
		"1232312312321113",
		"2132311232312123",
		"3121213232112312"
	}; 
	 
	int *xx[MAX_ROW_COUNT];
	memset(xx, 0, MAX_ROW_COUNT * sizeof(int*));

	for(i = 0; i < row_count; i++)
	{
		xx[i] = parseValue(x[i]);
	}

	free(perm_list);
	for(i = 0; i < row_count; i++)
		free(xx[i]);

	return 0;
}

